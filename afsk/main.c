/*
 *  Transmits CubeSat Telemetry at 440MHz in AO-7 format
 *
 *  Copyright Alan B. Johnston
 *
 *  Portions Copyright (C) 2018 Jonathan Brandenburg
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "status.h"
#include "ax5043.h"
#include "ax25.h"
#include "spi/ax5043spi.h"
#include <wiringPiI2C.h>
#include <time.h>

#define VBATT 15
#define ADC5 17
#define ADC6 18
#define ADC7 19
#define ADC8 20
#define TIME 8
#define UCTEMP 30
#define UPTIME_SEC 8 
#define A 1
#define B 2
#define C 3
#define D 4

#define SENSOR_40 0
#define SENSOR_41 3
#define SENSOR_44 6
#define SENSOR_45 9
#define SENSOR_4A 12
#define VOLTAGE 0
#define CURRENT 1
#define POWER 2
#define VBATT 15

ax5043_conf_t hax5043;
ax25_conf_t hax25;

static void init_rf();
int get_tlm(int tlm[][5]);
long int timestamp;
void config_x25();
void trans_x25();
//long int timestamp;
int tempSensor; 

int upper_digit(int number);
int lower_digit(int number);

uint16_t config = (0x2000 | 0x1800 | 0x0180 | 0x0018 | 0x0007 );

int main(void) {
    
    int tlm[7][5];
    int i, j;
    for (i = 1; i < 7; i++) {
        for (j = 1; j < 5; j++) {
		tlm[i][j] = 0;
	}
    }
    timestamp = time(NULL);
	
    tempSensor = wiringPiI2CSetupInterface("/dev/i2c-3", 0x48); 
	
    setSpiChannel(SPI_CHANNEL);
    setSpiSpeed(SPI_SPEED);
    initializeSpi();

    int ret;
    uint8_t data[1024];
        
    /* Infinite loop */
    for (;;) {
        sleep(2);
    	
	// send X.25 packet

    	init_rf();

    	ax25_init(&hax25, (uint8_t *) "CQ", '2', (uint8_t *) "DX", '2',
    		AX25_PREAMBLE_LEN,
   		 AX25_POSTAMBLE_LEN);
        
	printf("INFO: Getting TLM Data\n");
	    
	get_tlm(tlm);

	printf("INFO: Preparing X.25 packet\n");
	
	char str[1000];
	char tlm_str[1000];
	
	char header_str[] = "\x03\x0fhi hi ";
	strcpy(str, header_str);
	
        int channel;
	for (channel = 1; channel < 7; channel++) {
//        printf("%d %d %d %d \n", tlm[channel][1], tlm[channel][2], tlm[channel][3], tlm[channel][4]); 
          sprintf(tlm_str, "%d%d%d %d%d%d %d%d%d %d%d%d ", 
	  channel, upper_digit(tlm[channel][1]), lower_digit(tlm[channel][1]),
 	  channel, upper_digit(tlm[channel][2]), lower_digit(tlm[channel][2]), 
	  channel, upper_digit(tlm[channel][3]), lower_digit(tlm[channel][3]), 
	  channel, upper_digit(tlm[channel][4]), lower_digit(tlm[channel][4]));
//	  printf("%s \n",tlm_str);
	  strcat(str, tlm_str);
	}	
        
	printf("INFO: Transmitting X.25 packet\n");

        memcpy(data, str, strnlen(str, 256));
        ret = ax25_tx_frame(&hax25, &hax5043, data, strnlen(str, 256));
        if (ret) {
            fprintf(stderr,
                    "ERROR: Failed to transmit AX.25 frame with error code %d\n",
                    ret);
            exit(EXIT_FAILURE);
        }
        ax5043_wait_for_transmit();
        if (ret) {
            fprintf(stderr,
                    "ERROR: Failed to transmit entire AX.25 frame with error code %d\n",
                    ret);
            exit(EXIT_FAILURE);
	}
    }

    return 0;
}

static void init_rf() {
    int ret;
    ret = ax5043_init(&hax5043, XTAL_FREQ_HZ, VCO_INTERNAL);
    if (ret != PQWS_SUCCESS) {
        fprintf(stderr,
                "ERROR: Failed to initialize AX5043 with error code %d\n", ret);
        exit(EXIT_FAILURE);
    }
}

//  Returns lower digit of a number which must be less than 99
//
int lower_digit(int number) {

	int digit = 0;
	if (number < 100) 
		digit = number - ((int)(number/10) * 10);
	else
		printf("ERROR: Not a digit in lower_digit!\n");
	return digit;
}

// Returns upper digit of a number which must be less than 99
//
int upper_digit(int number) {

	int digit = 0;
	if (number < 100) 
		digit = (int)(number/10);
	else
		printf("ERROR: Not a digit in upper_digit!\n");
	return digit;
}
int get_tlm(int tlm[][5]) {

//  Reading I2C voltage and current sensors	
	
      char cmdbuffer[1000];
      FILE* file = popen("sudo python /home/pi/CubeSatSim/python/readcurrent.py 2>&1", "r"); 
      fgets(cmdbuffer, 1000, file);
      pclose(file);
      printf("I2C Sensor data: %s\n", cmdbuffer);

      char ina219[16][20];  // voltage, currents, and power from the INA219 current sensors x4a, x40, x41, x44, and x45.
      int i = 0;
      char * data2 = strtok (cmdbuffer," ");

      while (data2 != NULL) {
          strcpy(ina219[i], data2);
  //        printf ("ina219[%d]=%s\n",i,ina219[i]);
          data2 = strtok (NULL, " ");
          i++;
      }
//	printf("1B: ina219[%d]: %s val: %f \n", SENSOR_40 + CURRENT, ina219[SENSOR_40 + CURRENT], strtof(ina219[SENSOR_40 + CURRENT], NULL));

	tlm[1][A] = (int)(strtof(ina219[SENSOR_4A + CURRENT], NULL) / 15 + 0.5);  // Current of 5V supply to Pi
	tlm[1][B] = (int) (99.5 - strtof(ina219[SENSOR_40 + CURRENT], NULL)/10);  // +X current [4]
	tlm[1][D] = (int) (99.5 - strtof(ina219[SENSOR_41 + CURRENT], NULL)/10);  // +Y current [7]
	tlm[1][C] = (int) (99.5 - strtof(ina219[SENSOR_44 + CURRENT], NULL)/10);  // +Z current [10] (actually -X current, AO-7 didn't have a Z solar panel?)
	
	tlm[2][B] = 99;
	tlm[2][C] = (int)((time(NULL) - timestamp) / 15) % 100; 
	tlm[2][D] = (int)(50.5 + strtof(ina219[SENSOR_45 + CURRENT], NULL)/10.0);   // NiMH Battery current
	
	tlm[3][A] = (int)((strtof(ina219[SENSOR_45 + VOLTAGE], NULL) * 10) - 65.5);
	tlm[3][B] = (int)(strtof(ina219[SENSOR_4A + VOLTAGE], NULL) * 10.0);      // 5V supply to Pi
		   	
        int tempValue = wiringPiI2CReadReg16(tempSensor, 0); 
//        printf("Read: %x\n", tempValue);
        uint8_t upper = (uint8_t) (tempValue >> 8);
        uint8_t lower = (uint8_t) (tempValue & 0xff);
        float temp = (float)lower + ((float)upper / 0x100);

	tlm[4][A] = (int)((95.8 - temp)/1.48 + 0.5);
		
	tlm[6][B] = 0 ;
	tlm[6][D] = 49 + rand() % 3; 

// Display tlm
    int k, j;
    for (k = 1; k < 7; k++) {
        for (j = 1; j < 5; j++) {
	printf(" %2d ",	tlm[k][j]);
	}
    printf("\n");
    }	
       return 0;
}
