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
int lower_digit(int number);
int upper_digit(int number);
int encode_digit(uint8_t *msg, int number);
void config_cw();
int encode_tlm(uint8_t *buffer, int channel, int val1, int val2, int val3, int val4, int avail);
int encode_header(uint8_t *buffer, int avail);
int add_dash(uint8_t *msg, int number); 
int add_dot(uint8_t *msg, int number); 
int add_space(uint8_t *msg);

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

    retVal = mode_tx();
    if (retVal != AXRADIO_ERR_NOERROR) {
         fprintf(stderr, "ERROR: Unable to enter TX mode\n");
         exit(EXIT_FAILURE);
    }

    config_cw();

    // allocate space for the buffer
    static uint8_t packet[MAX_MESSAGE_LENGTH + 1];
     
    int channel;
    int msg_length;

    while(1) {

	for (channel = 0; channel < 7; channel++) {

        if (channel == 0) {
            msg_length = encode_header(&packet[0], 
			 (MAX_MESSAGE_LENGTH + 1));
       
            printf("\nINFO: Sending TLM header\n");

        } else {
            msg_length = encode_tlm(&packet[0], channel,
		 channel, channel+1, channel+2, channel+3,
			 (MAX_MESSAGE_LENGTH + 1));
       
            printf("\nINFO: Sending TLM channel %d \n", channel);
        }
        printf("DEBUG: msg_length = %d\n", msg_length);

        retVal = transmit_packet(&remoteaddr_tx, packet,
			 (uint16_t)(msg_length));
        if (retVal != AXRADIO_ERR_NOERROR) {
            fprintf(stderr, "ERROR: Unable to transmit a packet\n");
            exit(EXIT_FAILURE);
	}
//	sleep(1);

	usleep(200000);
        }
    }
}
int encode_header(uint8_t *buffer, int avail) {

    int count = 0;
    count += add_space(&buffer[count]);
    count += add_space(&buffer[count]);

    count += add_dash(&buffer[count], 1);		// c 
    count += add_dot(&buffer[count], 1);		
    count += add_dash(&buffer[count], 1);		 
    count += add_dot(&buffer[count], 1);		
    count += add_space(&buffer[count]);
    
    count += add_dash(&buffer[count], 2);		// q 
    count += add_dot(&buffer[count], 1);		 
    count += add_dash(&buffer[count], 1);		
    count += add_space(&buffer[count]);
    
    count += add_space(&buffer[count]);

    count += add_dot(&buffer[count], 4);		// h
    count += add_space(&buffer[count]);
    
    count += add_dot(&buffer[count], 2);		// i
    count += add_space(&buffer[count]);
    
    count += add_space(&buffer[count]);
    
    count += add_dot(&buffer[count], 4);		// h
    count += add_space(&buffer[count]);
    
    count += add_dot(&buffer[count], 2);		// i
    count += add_space(&buffer[count]);

    count += add_space(&buffer[count]);
    count += add_space(&buffer[count]);

    return count;
}


int encode_tlm(uint8_t *buffer, int channel, int val1, int val2, int val3, int val4, int avail) {

    int count = 0;

    count += add_space(&buffer[count]);
    count += add_space(&buffer[count]);
    count += add_space(&buffer[count]);
    count += add_space(&buffer[count]);
 
    count += encode_digit(&buffer[count], channel);
    count += encode_digit(&buffer[count], upper_digit(val1));
    count += encode_digit(&buffer[count], lower_digit(val1));

    count += add_space(&buffer[count]);

    count += encode_digit(&buffer[count], channel);
    count += encode_digit(&buffer[count], upper_digit(val2));
    count += encode_digit(&buffer[count], lower_digit(val2));

    count += add_space(&buffer[count]);

    count += encode_digit(&buffer[count], channel);
    count += encode_digit(&buffer[count], upper_digit(val3));
    count += encode_digit(&buffer[count], lower_digit(val3));

    count += add_space(&buffer[count]);

    count += encode_digit(&buffer[count], channel);
    count += encode_digit(&buffer[count], upper_digit(val4));
    count += encode_digit(&buffer[count], lower_digit(val4));

    count += add_space(&buffer[count]);

    //printf("DEBUG count: %d avail: %d \n", count, avail);
    if (count > avail) {
       buffer[avail-1] = 0;
       count = avail-1;
       printf("DEBUG count > avail!\n");
    }
return count;
}

int encode_digit(uint8_t *buffer, int digit) {
	int count = 0;
	switch(digit)
	{
		case 0:
			count += add_dash(&buffer[count], 5);		// 0
			count += add_space(&buffer[count]);

			break;

		case 1:
			count += add_dot(&buffer[count], 1);		// 1
			count += add_dash(&buffer[count], 4);		
			count += add_space(&buffer[count]);
    
			break;

		case 2:
			count += add_dot(&buffer[count], 2);		// 2
			count += add_dash(&buffer[count], 3);		
			count += add_space(&buffer[count]);

			break;

		case 3:
			count += add_dot(&buffer[count], 3);		// 3
			count += add_dash(&buffer[count], 2);		
			count += add_space(&buffer[count]);
			
			break;

		case 4:
			count += add_dot(&buffer[count], 4);		// 4
			count += add_dash(&buffer[count], 1);		
			count += add_space(&buffer[count]);
			
			break;

		case 5:
			count += add_dot(&buffer[count], 5);		// 5
			count += add_space(&buffer[count]);
			
			break;

		case 6:
			count += add_dash(&buffer[count], 1);		// 6
			count += add_dot(&buffer[count], 4);		
			count += add_space(&buffer[count]);
			
			break;

		case 7:

			count += add_dash(&buffer[count], 2);		// 7
			count += add_dot(&buffer[count], 3);		
			count += add_space(&buffer[count]);

			break;

		case 8:
			count += add_dash(&buffer[count], 3);		// 8
			count += add_dot(&buffer[count], 2);		
			count += add_space(&buffer[count]);

			break;

		case 9:
			count += add_dash(&buffer[count], 4);		// 9
			count += add_dot(&buffer[count], 1);		
			count += add_space(&buffer[count]);
		
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
	ax5043WriteReg(0x167,0x50); // 0x25); // 0x50); // 0x08); // 0x20);
	
	ax5043WriteReg(0x161,0);
	ax5043WriteReg(0x162,0x20);
	
	long txRate;
	txRate = ax5043ReadReg(0x167) + 256 * ax5043ReadReg(0x166) + 65536 * ax5043ReadReg(0x165);
	printf("Tx Rate %x %x %x \n", ax5043ReadReg(0x165), ax5043ReadReg(0x166), ax5043ReadReg(0x167)); 
	long fskDev;
	fskDev = ax5043ReadReg(0x163) + 256 * ax5043ReadReg(0x162) + 65536 * ax5043ReadReg(0x161);

	ax5043WriteReg(0x37,(uint8_t)((ax5043ReadReg(0x37) + 4)));  // Increase FREQA

	printf("Tx Rate: %ld FSK Dev: %ld \n", txRate, fskDev);
	
	ax5043WriteReg(0x10,0);	// ASK

	printf("Modulation: %x \n", (int)ax5043ReadReg(0x10));
	printf("Frequency A: 0x%x %x %x %x \n",(int)ax5043ReadReg(0x34),(int)ax5043ReadReg(0x35),(int)ax5043ReadReg(0x36),(int)ax5043ReadReg(0x37));

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


int add_space(uint8_t *msg) {
    	msg[0] = 0x00;
	return 1;	
}

int add_dash(uint8_t *msg, int number) {
	int j;
	int counter = 0;

	for (j=0; j < number; j++) {
     		msg[counter++] = 0xff;
		msg[counter++] = 0x0f;
	}
	return counter;	
}

int add_dot(uint8_t *msg, int number) {
	int counter = 0;
	int j;
	for (j=0; j < number; j++) {
     		msg[counter++] = 0x0f;
	}
	return counter;	
}
