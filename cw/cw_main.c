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
#include <generated/configtx.h>
#include <pthread.h>
#include <semaphore.h>
#include <spi/ax5043spi_p.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAX_MESSAGE_LENGTH (197)

extern uint8_t axradio_rxbuffer[];
void *transmit(void *arg);
int get_message(uint8_t *buffer, int avail);
int get_cw(uint8_t *buffer, int avail);
int add_dot(uint8_t *msg, int number);
int add_dash(uint8_t *msg, int number);
int add_space(uint8_t *msg, int number);
int lower_digit(int number);
int upper_digit(int number);
int encode_digit(uint8_t *msg, int number);
void config_cw();
 
enum RadioState {UnknownState, RxState, TxState};
enum RadioState currentState = UnknownState;

enum ReceiveState {WaitingForNewPacket, WaitingForPacketCounter1,
   WaitingForPacketCounter2, WaitingForMessageLength1,
   WaitingForMessageLength2, WaitingForMessage,
   WaitingForChecksum1, WaitingForChecksum2};

static uint8_t on_value = 0xff;
static uint8_t off_value = 0x00;
int spacing = 1;  // integer number of octets for a dot


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

void *transmit(void *arg) {
    sem_t *sem;
    sem = (sem_t *)arg;

    uint8_t retVal;
/*
        int x;
	for (x = 0; x < 0x20; x++)
        {
                printf("Register %x contents: %x\n",x,(int)ax5043ReadReg(x));
        }

        printf("Register Dump complete");
*/    
     for (;;) {
        int result;

        // allocate space for the buffer
        static uint8_t packet[MAX_MESSAGE_LENGTH + 1];
        //uint16_t pkt_counter;

       // ++pkt_counter;
        
     
	int reserved_space = 0;

        int msg_length = get_cw(&packet[reserved_space], (MAX_MESSAGE_LENGTH + 1) - reserved_space);

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

        printf("INFO: Sending another packet...\n");
        printf("DEBUG: msg_length = %d\n", msg_length);
        printf("DEBUG: reserved_space = %d\n", reserved_space);

	while(1) {

/*
        int x;
        for (x = 0; x < 0x20; x++)
        {
                printf("Register %x contents: %x\n",x,(int)ax5043ReadReg(x));
        }

        printf("Register Dump complete");
*/

/*
        printf("Register write to clear framing and crc\n");
	ax5043WriteReg(0x12,0);

        printf("Register write to disable fec\n");
	ax5043WriteReg(0x18,0);

        printf("Register write \n");
	ax5043WriteReg(0x165,0);

	ax5043WriteReg(0x166,0);
	ax5043WriteReg(0x167,0x50); // 0x08); // 0x20);
	
	ax5043WriteReg(0x161,0);
	ax5043WriteReg(0x162,0x20);
	
	long txRate;
	txRate = ax5043ReadReg(0x167) + 256 * ax5043ReadReg(0x166) + 65536 * ax5043ReadReg(0x165);
	printf("Tx Rate %x %x %x \n", ax5043ReadReg(0x165), ax5043ReadReg(0x166), ax5043ReadReg(0x167)); 
	long fskDev;
	fskDev = ax5043ReadReg(0x163) + 256 * ax5043ReadReg(0x162) + 65536 * ax5043ReadReg(0x161);

	ax5043WriteReg(0x37,(ax5043ReadReg(0x37) + 4));  // Increase FREQA

	printf("Tx Rate: %ld FSK Dev: %ld \n", txRate, fskDev);
	
	ax5043WriteReg(0x10,0);	// ASK

	printf("Modulation: %x \n", (int)ax5043ReadReg(0x10));
	printf("Frequency A: 0x%x %x %x %x \n",(int)ax5043ReadReg(0x34),(int)ax5043ReadReg(0x35),(int)ax5043ReadReg(0x36),(int)ax5043ReadReg(0x37));
*/

/* HERE */

/*
        int x;
	for (x = 0; x < 0x20; x++)
        {
                printf("Register %x contents: %x\n",x,(int)ax5043ReadReg(x));
        }

        printf("Register Dump complete");
*/    

		config_cw();

        	retVal = transmit_packet(&remoteaddr_tx, packet, msg_length + reserved_space);
        	if (retVal != AXRADIO_ERR_NOERROR) {
            		fprintf(stderr, "ERROR: Unable to transmit a packet\n");
            	exit(EXIT_FAILURE);
		}
	sleep(10);

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

int get_cw(uint8_t *buffer, int avail) {

    int count = 0;
/*
count += add_space(&buffer[count], 10);

count += add_dash(&buffer[count], 1);		// c 
count += add_dot(&buffer[count], 1);		
count += add_dash(&buffer[count], 1);		 
count += add_dot(&buffer[count], 1);		
count += add_space(&buffer[count], 3);
   
count += add_dash(&buffer[count], 2);		// q 
count += add_dot(&buffer[count], 1);		 
count += add_dash(&buffer[count], 1);		
count += add_space(&buffer[count], 7);
 
count += add_dot(&buffer[count], 4);		// h
count += add_space(&buffer[count], 3);

count += add_dot(&buffer[count], 2);		// i
count += add_space(&buffer[count], 7);

count += add_dot(&buffer[count], 4);		// h
count += add_space(&buffer[count], 3);

count += add_dot(&buffer[count], 2);		// i
count += add_space(&buffer[count], 7);
*/

int tlm_1a = 42;
int tlm_1b = 35;

count += encode_digit(&buffer[count], 1);
count += encode_digit(&buffer[count], upper_digit(tlm_1a));
count += encode_digit(&buffer[count], lower_digit(tlm_1a));

count += add_space(&buffer[count], 7);

count += encode_digit(&buffer[count], 1);
count += encode_digit(&buffer[count], upper_digit(tlm_1b));
count += encode_digit(&buffer[count], lower_digit(tlm_1b));

count += add_space(&buffer[count], 14);

count += encode_digit(&buffer[count], 1);
count += encode_digit(&buffer[count], upper_digit(tlm_1a++));
count += encode_digit(&buffer[count], lower_digit(tlm_1a));

count += add_space(&buffer[count], 7);
/*
count += encode_digit(&buffer[count], 1);
count += encode_digit(&buffer[count], upper_digit(tlm_1b++));
count += encode_digit(&buffer[count], lower_digit(tlm_1b));

count += add_space(&buffer[count], 14);

count += encode_digit(&buffer[count], 2);
count += encode_digit(&buffer[count], upper_digit(tlm_1a++));
count += encode_digit(&buffer[count], lower_digit(tlm_1a));

count += add_space(&buffer[count], 7);

count += encode_digit(&buffer[count], 2);
count += encode_digit(&buffer[count], upper_digit(tlm_1b++));
count += encode_digit(&buffer[count], lower_digit(tlm_1b));

count += add_space(&buffer[count], 14);

count += encode_digit(&buffer[count], 2);
count += encode_digit(&buffer[count], upper_digit(tlm_1a++));
count += encode_digit(&buffer[count], lower_digit(tlm_1a));

count += add_space(&buffer[count], 7);

count += encode_digit(&buffer[count], 2);
count += encode_digit(&buffer[count], upper_digit(tlm_1b++));
count += encode_digit(&buffer[count], lower_digit(tlm_1b));

count += add_space(&buffer[count], 14);

*/
    printf("DEBUG count: %d avail: %d \n", count, avail);
    if (count > avail) {
    	buffer[avail-1] = 0;
    	count = avail-1;
	printf("DEBUG count > avail!\n");
    }
   // printf("DEBUG get_cw: ***%s***\n", buffer);

    //return strlen((char *)buffer);
    return count;
}

int add_dash(uint8_t *msg, int number) {
	int counter = 0;
	int i,j;
	for (j=0; j < number; j++) {
		for (i=0; i < spacing * 3; i++) {
			msg[counter++] = on_value;
		}
		counter += add_space(&msg[counter], 1);
	}
	return counter;
}

int add_dot(uint8_t *msg, int number) {
	int counter = 0;
	int i,j;
	for (j=0; j < number; j++) {
		for (i=0; i < spacing; i++) {
			msg[counter++] = on_value;
		}

		counter += add_space(&msg[counter], 1);
	}
	return counter;
}

int add_space(uint8_t *msg, int number) {
	int j;
	int counter = 0;
	for (j=0; j < number * spacing; j++) {
		msg[counter++] = off_value;
	}
	return counter;
}

int encode_digit(uint8_t *buffer, int digit) {
	int count = 0;
	switch(digit)

	{
		case 0:
			count += add_dash(&buffer[count], 5);		// 0
			count += add_space(&buffer[count], 3);

			break;

		case 1:
			count += add_dot(&buffer[count], 1);		// 1
			count += add_dash(&buffer[count], 4);		
			count += add_space(&buffer[count], 3);
    
			break;

		case 2:
			count += add_dot(&buffer[count], 2);		// 2
			count += add_dash(&buffer[count], 3);		
			count += add_space(&buffer[count], 3);

			break;

		case 3:
			count += add_dot(&buffer[count], 3);		// 3
			count += add_dash(&buffer[count], 2);		
			count += add_space(&buffer[count], 3);
			
			break;

		case 4:
			count += add_dot(&buffer[count], 4);		// 4
			count += add_dash(&buffer[count], 1);		
			count += add_space(&buffer[count], 3);
			
			break;

		case 5:
			count += add_dot(&buffer[count], 5);		// 5
			count += add_space(&buffer[count], 3);
			
			break;

		case 6:
			count += add_dash(&buffer[count], 1);		// 6
			count += add_dot(&buffer[count], 4);		
			count += add_space(&buffer[count], 3);
			
			break;

		case 7:

			count += add_dash(&buffer[count], 2);		// 7
			count += add_dot(&buffer[count], 3);		
			count += add_space(&buffer[count], 3);

			break;

		case 8:
			count += add_dash(&buffer[count], 3);		// 8
			count += add_dot(&buffer[count], 2);		
			count += add_space(&buffer[count], 3);

			break;

		case 9:
			count += add_dash(&buffer[count], 4);		// 9
			count += add_dot(&buffer[count], 1);		
			count += add_space(&buffer[count], 3);
		
			break;

		default:
			printf("ERROR: Not a digit!\n");
			return 0;
	}
	return count;
}
int lower_digit(int number) {

	int digit = 0;

	if (number < 100) 
		digit = number - ((int)(number/10) * 10);
	else
		printf("ERROR: Not a digit in lower_digit!\n");

	return digit;
}

int upper_digit(int number) {

	int digit = 0;

	if (number < 100) 
		digit = (int)(number/10);
	else
		printf("ERROR: Not a digit in upper_digit!\n");

	return digit;
}

void config_cw() {

        printf("Register write to clear framing and crc\n");
	ax5043WriteReg(0x12,0);

        printf("Register write to disable fec\n");
	ax5043WriteReg(0x18,0);

        printf("Register write \n");
	ax5043WriteReg(0x165,0);

	ax5043WriteReg(0x166,0);
	ax5043WriteReg(0x167,0x50); // 0x08); // 0x20);
	
	ax5043WriteReg(0x161,0);
	ax5043WriteReg(0x162,0x20);
	
	long txRate;
	txRate = ax5043ReadReg(0x167) + 256 * ax5043ReadReg(0x166) + 65536 * ax5043ReadReg(0x165);
	printf("Tx Rate %x %x %x \n", ax5043ReadReg(0x165), ax5043ReadReg(0x166), ax5043ReadReg(0x167)); 
	long fskDev;
	fskDev = ax5043ReadReg(0x163) + 256 * ax5043ReadReg(0x162) + 65536 * ax5043ReadReg(0x161);

	ax5043WriteReg(0x37,(ax5043ReadReg(0x37) + 4));  // Increase FREQA

	printf("Tx Rate: %ld FSK Dev: %ld \n", txRate, fskDev);
	
	ax5043WriteReg(0x10,0);	// ASK

	printf("Modulation: %x \n", (int)ax5043ReadReg(0x10));
	printf("Frequency A: 0x%x %x %x %x \n",(int)ax5043ReadReg(0x34),(int)ax5043ReadReg(0x35),(int)ax5043ReadReg(0x36),(int)ax5043ReadReg(0x37));

/* HERE */

/*
        int x;
	for (x = 0; x < 0x20; x++)
        {
                printf("Register %x contents: %x\n",x,(int)ax5043ReadReg(x));
        }

        printf("Register Dump complete");
*/    
	return;

}
