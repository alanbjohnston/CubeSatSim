// Copyright (c) 2018 Brandenburg Tech, LLC
// All right reserved.
//
// THIS SOFTWARE IS PROVIDED BY BRANDENBURG TECH, LLC AND CONTRIBUTORS
// ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BRANDENBURT TECH, LLC
// AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <axradio/axradioinit_p.h>
#include <axradio/axradiomode_p.h>
#include <axradio/axradiorx_p.h>
#include <axradio/axradiotx_p.h>
#include <ctype.h>
#include <generated/configtx.h>
#include <spi/ax5043spi_p.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FALSE (0)
#define TRUE (1)

#define MAX_MESSAGE_LENGTH (197)
#define MAX_INPUT_LENGTH (1024)
#define MAX_WORD_LENGTH (192)

extern uint8_t axradio_rxbuffer[];

void receive(void);
void transmit(uint8_t *word, size_t length);
void toPigLatin(uint8_t *input);
void readWord(uint8_t *word, unsigned int wordLength, uint8_t **input);
void wordToPigLatin(uint8_t *word, unsigned int wordLength);
void wordToPigLatinVowel(uint8_t *word, unsigned int srcLoc, uint8_t workWord[],
        unsigned int destLoc, unsigned int wordLength);
void wordToPigLatinConsonant(uint8_t firstLetter, uint8_t *word, unsigned int srcLoc,
        uint8_t *workWord, unsigned int destLoc, unsigned int wordLength);

enum RadioState {UnknownState, RxState, TxState};
enum RadioState currentState = UnknownState;

enum ReceiveState {WaitingForNewPacket, WaitingForPacketCounter1,
   WaitingForPacketCounter2, WaitingForMessageLength1,
   WaitingForMessageLength2, WaitingForMessage,
   WaitingForChecksum1, WaitingForChecksum2};

int main(void)
{
    uint8_t retVal;

    // Configure SPI bus to AX5043
    setSpiChannel(SPI_CHANNEL);
    setSpiSpeed(SPI_SPEED);
    initializeSpi();

    // Initialize the AX5043
    retVal = axradio_init();
    if (retVal == AXRADIO_ERR_NOCHIP) {
        fprintf(stderr, "ERROR: No AX5043 RF chip found\n");
        exit(EXIT_FAILURE);
    }
    if (retVal != AXRADIO_ERR_NOERROR) {
        fprintf(stderr, "ERROR: Unable to initialize AX5043\n");
        exit(EXIT_FAILURE);
    }

    printf("INFO: Found and initialized AX5043\n");

    printf("This utility receives text from another transmitter,\n");
    printf("translates it to pig Latin, then transmits the translation\n");

    receive();

    return 0;
}

void receive(void) {
    uint8_t retVal;
    uint16_t packetNumber;
    uint16_t messageLength;
    uint16_t checksum;
    uint8_t ch;

    // Allocate space for the translation. I'm choosing to use an array of
    // "reasonable" length
    uint8_t input[MAX_INPUT_LENGTH];
    uint16_t currentLetter = 0;
    input[currentLetter] = '\0';

    enum ReceiveState currentReceiveState = WaitingForNewPacket;

    for (;;) {
        // Enter receive mode only if not already in receive mode
        if (currentState != RxState) {
            retVal = mode_rx();
            if (retVal != AXRADIO_ERR_NOERROR) {
                fprintf(stderr, "ERROR: Unable to enter RX mode\n");
                exit(EXIT_FAILURE);
            }
            currentState = RxState;
        }

        retVal = receive_packet();
        if (retVal > 0) {
            uint8_t counter = 0;
            while (retVal-- > 0) {
                switch(currentReceiveState) {
                case WaitingForNewPacket:
                    printf("Pkt Len: %d\n", (int)axradio_rxbuffer[counter++]);
                    currentReceiveState = WaitingForPacketCounter1;
                    break;
                case WaitingForPacketCounter1:
                    packetNumber =  (int)axradio_rxbuffer[counter++];
                    currentReceiveState = WaitingForPacketCounter2;
                    break;
                case WaitingForPacketCounter2:
                    packetNumber |= (uint16_t)(axradio_rxbuffer[counter++] << 8);
                    printf("Pkt Num: %d\n", (int)packetNumber);
                    currentReceiveState = WaitingForMessageLength1;
                    break;
                case WaitingForMessageLength1:
                    messageLength =  (int)axradio_rxbuffer[counter++];
                    currentReceiveState = WaitingForMessageLength2;
                    break;
                case WaitingForMessageLength2:
                    messageLength |= (uint16_t)(axradio_rxbuffer[counter++] << 8);
                    printf("Msg Len: %d\n", (int)messageLength);
                    currentReceiveState = WaitingForMessage;

                    currentLetter = 0;
                    input[currentLetter] = '\0';
                    printf("receive: ");

                    break;
                case WaitingForMessage:
                    ch = axradio_rxbuffer[counter++];

                    input[currentLetter] = ch;
                    currentLetter++;
                    input[currentLetter] = '\0';

                    if (ch != '\n') {
                        printf("%c", ch);
                    }
                    else {
                        //printf(" ");
                        printf("\n");

                        toPigLatin(input);

                        currentReceiveState = WaitingForChecksum1;
                    }
                    break;
                case WaitingForChecksum1:
                    checksum =  (int)axradio_rxbuffer[counter++];
                    currentReceiveState = WaitingForChecksum2;
                    break;
                case WaitingForChecksum2:
                    checksum |= (uint16_t)(axradio_rxbuffer[counter++] << 8);
                    printf("Chksum: %d\n", (int)checksum);
                    currentReceiveState = WaitingForNewPacket;
                    break;
                default:
                    fprintf(stderr, "ERROR: Unknown state in receive state machine\n");
                    exit(EXIT_FAILURE);
                    break;
                }
            }
        }

        usleep(25000);
    }
}

void transmit(uint8_t *word, size_t length) {
    uint8_t retVal;

    // allocate space for the buffer
    static uint8_t packet[MAX_MESSAGE_LENGTH + 1];
    static uint16_t pkt_counter = 0;

    ++pkt_counter;

    // Calculate the number of reserved bytes at the beginning of the packet
    size_t reserved_space = 0;

    // if transmitting a packet counter, reserve two bytes
    if (framing_insert_counter) {
        reserved_space += 2;
    }

    // reserve two bytes for the overall length of the packet including the
    // packet counter, if present, and the field containing the length
    reserved_space += 2;

    size_t msg_length = 0;

    // get a message to transmit.
    //msg_length = get_message(&packet[reserved_space], (MAX_MESSAGE_LENGTH + 1) - reserved_space);
    msg_length =
        length <= (MAX_MESSAGE_LENGTH + 1) - reserved_space ? length : (MAX_MESSAGE_LENGTH + 1) - reserved_space;
    strncpy((char*)&packet[reserved_space], (char*)word, msg_length);

    // if message consists only of a newline, terminate
    if (msg_length <= 1) {
        return;
    }

    if (framing_insert_counter) {
        packet[framing_counter_pos] = (uint8_t)(pkt_counter & 0xFF);
        packet[framing_counter_pos+1] = (uint8_t)((pkt_counter>>8) & 0xFF);

        // include the message length
        packet[framing_counter_pos+2] = (uint8_t)(msg_length & 0xFF);
        packet[framing_counter_pos+3] = (uint8_t)((msg_length>>8) & 0xFF);
    }
    else { // only include the message length
        packet[framing_counter_pos] = (uint8_t)(msg_length & 0xFF);
        packet[framing_counter_pos+1] = (uint8_t)((msg_length>>8) & 0xFF);
    }

    // Enter transmit mode only if not already in receive mode
    if (currentState != TxState) {
        retVal = mode_tx();
        if (retVal != AXRADIO_ERR_NOERROR) {
            fprintf(stderr, "ERROR: Unable to enter TX mode\n");
            exit(EXIT_FAILURE);
        }
        currentState = TxState;
    }

    //printf("INFO: Sending another packet...\n");
    //printf("DEBUG: msg_length = %d\n", msg_length);
    //printf("DEBUG: reserved_space = %d\n", reserved_space);
    retVal = transmit_packet(&remoteaddr_tx, packet, (uint16_t)(msg_length + reserved_space));
    if (retVal != AXRADIO_ERR_NOERROR) {
        fprintf(stderr, "ERROR: Unable to transmit a packet\n");
        exit(EXIT_FAILURE);
    }
    usleep(100000);
}

void toPigLatin(uint8_t *input) {
    //printf("input: %s\n", input);

    // Allocate space for a word. I'm choosing to use an array of
    // "reasonable" length
    uint8_t word[MAX_WORD_LENGTH];
    word[0] = '\0';

    readWord(word, MAX_WORD_LENGTH, &input);

    //printf("word: %s\n", word);

    // While there are still words...
    while (strlen((char*)word) > 0) {
        // convert the word to pig latin
        wordToPigLatin(word, MAX_WORD_LENGTH);
        strcat((char*)word, "\n");
        printf("%s", word);
        transmit(word, strlen((char*)word));

        // get the next word
        readWord(word, MAX_WORD_LENGTH, &input);

        //printf("word: %s\n", word);
    }

}

void readWord(uint8_t *word, unsigned int wordLength, uint8_t **input) {
    // initialize the word to zero length
    unsigned int currentLoc = 0;

    if (wordLength == 0) {
        printf("ERROR: Internal error. No buffer allocated for word\n");
        return;
    }

    word[currentLoc] = '\0';

    // Examine the first character.
    uint8_t ch;
    ch = **input;
    (*input)++;

    // If it is a newline, we have no more words, so
    // read and throw away the newline. That's all we need
    // to do
    if (ch == '\n') {
        // do nothing
    }
    else {
        int foundNonWhiteSpace = FALSE;

        // loop until we find our first non-white space
        // character and then the first white-space character
        // after that. That means we found a word and
        // the end of that word
        while (((!foundNonWhiteSpace || !isspace(ch))) && ch != '\n') {
            if (wordLength == 0) {
                printf("WARNING: Word is too long for buffer. Breaking up the word.\n");
                return;
            }

            if (!isspace(ch)) {
                foundNonWhiteSpace = TRUE;
            }

            word[currentLoc++] = ch;
            word[currentLoc] = '\0';

            // Get the next letter
                    ch = **input;
                    (*input)++;
        }

        // Push the white space character back into the input buffer
            (*input)--;
    }
}

void wordToPigLatin(uint8_t *word, unsigned int wordLength) {
    // allocate working space for a word
    uint8_t *workWord = malloc(wordLength);
    workWord[0] = '\0';

    unsigned int srcLoc = 0;
    unsigned int destLoc = 0;

    // copy over any leading whitespace
    while(word[srcLoc] && srcLoc < wordLength && isspace(word[srcLoc])) {
        workWord[destLoc++] = word[srcLoc++];
        workWord[destLoc] = '\0';
    }

    // Look at the next character
    uint8_t ch = word[srcLoc];
    // If it is a vowel...
    if (tolower(ch) == 'a' || tolower(ch) == 'e' || tolower(ch) == 'i' ||
            tolower(ch) == 'o' || tolower(ch) == 'u') {
        // process a word beginning with a vowel
        wordToPigLatinVowel(word, srcLoc, workWord, destLoc, wordLength);
    }
    // If it is a consonant...
    else if (isalpha(ch)) {
        // save that letter
        uint8_t firstLetter = ch;
        srcLoc++;

        // Take care of the case of a one letter word
        if (!word[srcLoc]) {
            // Save the first character, unchanged to the destination
            workWord[destLoc++] = ch;
            workWord[destLoc] = '\0';

            // Append "ay"
            strcat((char*)workWord, "ay");
        }
        // else we have more than one letter
        else {
            // Process a word beginning with a consonant
            wordToPigLatinConsonant(firstLetter, word, srcLoc, workWord,
                    destLoc, wordLength);
        }
    }
    // else just copy over the word
    else {
        strcat((char*)workWord, (char*)&word[srcLoc]);
    }

    // Copy the workspace into our word
    strncpy((char*)word, (char*)workWord, wordLength);
    // Ensure it is null-terminated under all circumstances
    if (strlen((char*)workWord) < wordLength - 1) {
        word[strlen((char*)workWord)] = '\0';
    }
    else {
        word[wordLength - 1] = '\0';
    }

    // Remember to free up the temporary workspace
    free(workWord);

}

void wordToPigLatinVowel(uint8_t *word, unsigned int srcLoc, uint8_t workWord[],
        unsigned int destLoc, unsigned int wordLength) {
    // copy over the word
    while (word[srcLoc] && srcLoc < wordLength) {
        workWord[destLoc++] = word[srcLoc++];
        workWord[destLoc] = '\0';
    }
    // append "way"
    strcat((char *)workWord, "way");
}

void wordToPigLatinConsonant(uint8_t firstLetter, uint8_t *word, unsigned int srcLoc,
        uint8_t *workWord, unsigned int destLoc, unsigned int wordLength) {
    // Copy over the next letter, properly capitalizing it
    if (isupper(firstLetter)) {
        workWord[destLoc++] = (uint8_t)toupper(word[srcLoc++]);
        workWord[destLoc] = '\0';
    } else {
        workWord[destLoc++] = (uint8_t)tolower(word[srcLoc++]);
        workWord[destLoc] = '\0';
    }
    while (word[srcLoc] && srcLoc < wordLength) {
        workWord[destLoc++] = word[srcLoc++];
        workWord[destLoc] = '\0';
    }
    // Append what was previously the first letter
    workWord[destLoc++] = (uint8_t)tolower(firstLetter);
    workWord[destLoc] = '\0';
    // Append "ay"
    strcat((char*)workWord, "ay");
}
