// Copyright (c) 2018 Brandenburg Tech, LLC
// All rights reserved.

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>

#include "../ax5043/ax5043init.h"
#include "../spi/ax5043spi.h"
#include "../ax5043/ax5043mode.h"
#include "../ax5043/ax5043rx.h"
#include "../ax5043/ax5043tx.h"
#include "../generated/configrx.h"
#include "../generated/configtx.h"

#define MAX_MESSAGE_LENGTH (197)

extern uint8_t axradio_rxbuffer[];

void *receive(void *arg);
void *transmit(void *arg);
int get_message(uint8_t *buffer, int avail);

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

    int result;

    sem_t ax5043_sem;
    result = sem_init(&ax5043_sem, 0, 1);
    if (result != 0) {
        fprintf(stderr, "ERROR: Unable to create semaphore with error %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }


    pthread_t receive_thread;
    result = pthread_create(&receive_thread, NULL, receive, (void *)&ax5043_sem);
    if (result != 0) {
        fprintf(stderr, "ERROR: Unable to spawn receive thread with error %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    pthread_t transmit_thread;
    result = pthread_create(&transmit_thread, NULL, transmit, (void *)&ax5043_sem);
    if (result != 0) {
        fprintf(stderr, "ERROR: Unable to spawn transmit thread with error %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }


    void *transmit_result;
    result = pthread_join(transmit_thread, &transmit_result);
    if (result != 0) {
        fprintf(stderr, "ERROR: Unable to wait for transmit thread to finish with error %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    sem_destroy(&ax5043_sem);

    return 0;
}

void *receive(void *arg) {
    sem_t *sem;
    sem = (sem_t *)arg;

    uint8_t retVal;
    uint16_t packetNumber;
    uint16_t messageLength;
    uint16_t checksum;
    char ch;

    enum ReceiveState currentReceiveState = WaitingForNewPacket;

    for (;;) {
        int result;

        result = sem_wait(sem);
        if (result != 0) {
            fprintf(stderr, "Failed to wait on semaphore with error %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }


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
                    printf("Pkt Len: %d ", (int)axradio_rxbuffer[counter++]);
                    currentReceiveState = WaitingForPacketCounter1;
                    break;
                case WaitingForPacketCounter1:
                    packetNumber =  (int)axradio_rxbuffer[counter++];
                    currentReceiveState = WaitingForPacketCounter2;
                    break;
                case WaitingForPacketCounter2:
                    packetNumber |= (int)axradio_rxbuffer[counter++] << 8;
                    printf("Pkt Num: %d ", (int)packetNumber);
                    currentReceiveState = WaitingForMessageLength1;
                    break;
                case WaitingForMessageLength1:
                    messageLength =  (int)axradio_rxbuffer[counter++];
                    currentReceiveState = WaitingForMessageLength2;
                    break;
                case WaitingForMessageLength2:
                    messageLength |= (int)axradio_rxbuffer[counter++] << 8;
                    printf("Msg Len: %d ", (int)messageLength);
                    currentReceiveState = WaitingForMessage;
                    break;
                case WaitingForMessage:
                    ch = (char)axradio_rxbuffer[counter++];
                    if (ch != '\n') {
                        printf("%c", ch);
                    }
                    else {
                        printf(" ");
                        currentReceiveState = WaitingForChecksum1;
                    }
                    break;
                case WaitingForChecksum1:
                    checksum =  (int)axradio_rxbuffer[counter++];
                    currentReceiveState = WaitingForChecksum2;
                    break;
                case WaitingForChecksum2:
                    checksum |= (int)axradio_rxbuffer[counter++] << 8;
                    printf("(Chksum: %d)\n", (int)checksum);
                    currentReceiveState = WaitingForNewPacket;
                    break;
                default:
                    fprintf(stderr, "ERROR: Unknown state in receive state machine\n");
                    exit(EXIT_FAILURE);
                    break;
                }
            }

            fflush(stdout);

        }

        result = sem_post(sem);
        if (result != 0) {
            fprintf(stderr, "Failed to post on semaphore with error %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        usleep(25000);
    }

    return NULL;
}

void *transmit(void *arg) {
    sem_t *sem;
    sem = (sem_t *)arg;

    uint8_t retVal;

    for (;;) {
        int result;

        // allocate space for the buffer
        static uint8_t packet[MAX_MESSAGE_LENGTH + 1];
        uint16_t pkt_counter;

        ++pkt_counter;

        // Calculate the number of reserved bytes at the beginning of the packet
        int reserved_space = 0;

        // if transmitting a packet counter, reserve two bytes
        if (framing_insert_counter) {
            reserved_space += 2;
        }

        // reserve two bytes for the overall length of the packet including the
        // packet counter, if present, and the field containing the length
        reserved_space += 2;

        // get a message to transmit.
        int msg_length = get_message(&packet[reserved_space], (MAX_MESSAGE_LENGTH + 1) - reserved_space);

        // if message consists only of a newline, terminate
        if (msg_length <= 1) {
            break;
        }

        if (framing_insert_counter) {
            packet[framing_counter_pos] = pkt_counter & 0xFF ;
            packet[framing_counter_pos+1] = (pkt_counter>>8) & 0xFF;

            // include the message length
            packet[framing_counter_pos+2] = msg_length & 0xFF ;
            packet[framing_counter_pos+3] = (msg_length>>8) & 0xFF;
        }
        else { // only include the message length
            packet[framing_counter_pos] = msg_length & 0xFF ;
            packet[framing_counter_pos+1] = (msg_length>>8) & 0xFF;
        }

        result = sem_wait(sem);
        if (result != 0) {
            fprintf(stderr, "Failed to wait on semaphore with error %s\n", strerror(errno));
            exit(EXIT_FAILURE);
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
        retVal = transmit_packet(&remoteaddr_tx, packet, msg_length + reserved_space);
        if (retVal != AXRADIO_ERR_NOERROR) {
            fprintf(stderr, "ERROR: Unable to transmit a packet\n");
            exit(EXIT_FAILURE);
        }

        result = sem_post(sem);
        if (result != 0) {
            fprintf(stderr, "Failed to post on semaphore with error %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

	usleep(200000);
    }

    return NULL;
}

int get_message(uint8_t *buffer, int avail) {

    static int instructionsPrinted = 0;

    // obtain a line of text. We state the message is limited to avail-2 to
    // leave space for the newline and the null terminator
    if (!instructionsPrinted) {
        printf("Please enter your message, up to %d characters\n   (empty message to terminate the program):\n", avail-2);
        instructionsPrinted = 1;
    }
    fgets((char *)buffer, avail, stdin);

    // check for end-of-file (for redirecting stdin)
    if (feof(stdin)) {
        buffer[0] = '\n';
        buffer[1] = 0;
    }

    // If the newline isn't present, the message is too long

    //printf("DEBUG: ***%s***\n", buffer);

    if (buffer[strlen((char *)buffer) - 1] != '\n') {
        buffer[strlen((char *)buffer) - 1] = '\n';
        printf("WARNING: message too long. It will be truncated to\n%s", (char *)buffer);

        // If the newline isn't in the buffer, read characters from the keyboard buffer to the newline
        int c;
        do {
            c = getchar();
        } while (c != '\n' && c != EOF);
    }

    //printf("DEBUG: ***%s***\n", buffer);

    return strlen((char *)buffer);
}
