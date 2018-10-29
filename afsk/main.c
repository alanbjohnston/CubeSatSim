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
void config_x25();
void trans_x25();
long int timestamp = 0;
int tempSensor, xPlusSensor, yPlusSensor, zPlusSensor, battCurrentSensor;

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
	
    tempSensor = wiringPiI2CSetupInterface("/dev/i2c-3", 0x48);
    srand((unsigned int)(wiringPiI2CReadReg16(tempSensor, 0)));   
	
    setSpiChannel(SPI_CHANNEL);
    setSpiSpeed(SPI_SPEED);
    initializeSpi();

    int ret;
    uint8_t data[1024];
    // 0x03 is a UI frame
    // 0x0F is no Level 3 protocol
    // rest is dummy CubeSatSim telemetry in AO-7 format 	
    // const char *str = "\x03\x0fhi hi 101 102 103 104 202 203 204 205 303 304 305 306 404 405 406 407 408 505 506 507 508 606 607 608 609\n";

    int devId = 0x40; // +X Panel current
    int i2cDevice = wiringPiI2CSetup (devId) ;

    int tempSensor = wiringPiI2CSetupInterface("/dev/i2c-3", 0x48);
   
    srand((unsigned int)(wiringPiI2CReadReg16(tempSensor, 0)));   
    
    /* Infinite loop */
    for (;;) {
        sleep(2);
    	
	// send X.25 packet

    	init_rf();

    	ax25_init(&hax25, (uint8_t *) "CQ", '2', (uint8_t *) "DX", '2',
    		AX25_PREAMBLE_LEN,
   		 AX25_POSTAMBLE_LEN);
        
	printf("INFO: Transmitting X.25 packet\n");
	    
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

	/*
//  Read MoPower UPS data
      FILE* file = popen("mpcmd show data 2>&1", "r");
      char cmdbuffer[1000];
      fgets(cmdbuffer, 1000, file);
      pclose(file);
//      printf("buffer is :%s\n", cmdbuffer);

      char mopower[64][14];
      char * data2;
      int i = 0;
      data2 = strtok (cmdbuffer," ");
      while (data2 != NULL) {
          strcpy(mopower[i], data2);
//        printf ("mopwer[%d]=%s\n",i,mopower[i]);
          data2 = strtok (NULL, " ");
          i++;
      }
      printf("Battery voltage = %s UPTIME_SEC %s UCTEMP %s \n", 
	  mopower[VBATT], mopower[UPTIME_SEC], mopower[UCTEMP]);

      long int time =  atoi(mopower[UPTIME_SEC]);

      if (timestamp == 0)
	 	timestamp = time;

//	int tlm_2c = (int)((time - timestamp) / 15) % 100; 
	tlm[2][C] = (int)((time - timestamp) / 15) % 100; 
	
//	printf("Relative time: %ld seconds 2C: %d  2C: %d%d\n", time - timestamp,tlm_2c, upper_digit(tlm_2c), lower_digit(tlm_2c));

        float vbat;
        vbat = strtof(mopower[VBATT], NULL);
//        printf(" vbat: %f \n", vbat);
//        int tlm_3a = (int)((vbat * 10) - 65.5);
	tlm[3][A] = (int)((vbat * 10) - 65.5);
//	int tlm_6b = 0, tlm_2b = 99;
	tlm[6][B] = 0 ;
	tlm[2][B] = 99;
//        printf("TLM 3A = %d \n", tlm_3a);

       // Read current from I2C bus

  //      printf("\n\nI2C result: %d\n", i2cDevice);
//        printf("Read: %d\n", wiringPiI2CRead(i2cDevice)) ;

//        int result = wiringPiI2CWriteReg16(xPlusSensor, 0x05, 4096);
//        printf("Write result: %d\n", result);
*
        int xCurrentValue = wiringPiI2CReadReg16(xPlusSensor, 0x04);
        int yCurrentValue = wiringPiI2CReadReg16(yPlusSensor, 0x04);
        int zCurrentValue = wiringPiI2CReadReg16(zPlusSensor, 0x04);
        int battCurrentValue = wiringPiI2CReadReg16(battCurrentSensor, 0x04);
        printf("Currents: %d %d %d %d \n\n", xCurrentValue, yCurrentValue, zCurrentValue, battCurrentValue);
*
	
//        int tlm_1b = (int) (98.5 - currentValue/400);
//	tlm[1][A] = (int) (98.5 - battCurrentValue/400);
//	tlm[1][B] = (int) (98.5 - xCurrentValue/400);
//	tlm[1][C] = (int) (98.5 - yCurrentValue/400);
//	tlm[1][D] = (int) (98.5 - zCurrentValue/400);
//        printf("TLM 1B = %d \n\n", tlm_1b);
//	int tlm_1a = 0, tlm_1c = 98, tlm_1d = 98, tlm_2a = 98;

*/
//  Reading 5V voltage and current
	     
      char cmdbuffer[1000];
      FILE* file = popen("sudo python /home/pi/CubeSatSim/python/readcurrent.py 2>&1", "r"); 
      fgets(cmdbuffer, 1000, file);
      pclose(file);
 //     printf("Current buffer is:%s\n", cmdbuffer);

      char ina219[16][20];  // voltage, currents, and power from the INA219 current sensors x4a, x40, x41, x44, and x45.
      int i = 0;
      char * data2 = strtok (cmdbuffer," ");

      while (data2 != NULL) {
        strcpy(ina219[i], data2);
        printf ("ina219[%d]=%s\n",i,ina219[i]);
        data2 = strtok (NULL, " ");
        i++;
      }
	printf("1B: ina219[%d]: %s val: %f \n", SENSOR_40 + CURRENT, ina219[SENSOR_40 + CURRENT], strtof(ina219[SENSOR_40 + CURRENT], NULL));

	tlm[1][B] = (int) (99.5 - strtof(ina219[SENSOR_40 + CURRENT], NULL)/10);  // +X current [4]
	tlm[1][D] = (int) (99.5 - strtof(ina219[SENSOR_41 + CURRENT], NULL)/10);  // +Y current [7]
	tlm[1][C] = (int) (99.5 - strtof(ina219[SENSOR_44 + CURRENT], NULL)/10);  // +Z current [10] (actually -X current, AO-7 didn't have a Z solar panel?)

//	int tlm_3b = (int)(strtof(ina219[0], NULL) * 10.0);
//	int tlm_2d = (int)(50.0 + strtof(ina219[SENSOR_4A + VOLTAGE], NULL)/40.0);
	tlm[3][B] = (int)(strtof(ina219[SENSOR_4A + VOLTAGE], NULL) * 10.0);      // 5V supply to Pi
	tlm[2][D] = (int)(50.5 + strtof(ina219[SENSOR_45 + CURRENT], NULL)/10.0);   // NiMH Battery current
//	printf(" 2D: %d 3B: %d\n", tlm_2d, tlm_3b);
	
	printf("1A: ina219[%d]: %s val: %f \n", SENSOR_4A + CURRENT, ina219[SENSOR_4A + CURRENT], strtof(ina219[SENSOR_4A + CURRENT], NULL));
	   
        tlm[1][A] = (int)(strtof(ina219[SENSOR_4A + CURRENT], NULL) / 15 + 0.5);  // Current of 5V supply to Pi
	
        int tempValue = wiringPiI2CReadReg16(tempSensor, 0); 
//        printf("Read: %x\n", tempValue);

        uint8_t upper = (uint8_t) (tempValue >> 8);
        uint8_t lower = (uint8_t) (tempValue & 0xff);

        float temp = (float)lower + ((float)upper / 0x100);
        printf("upper: %x lower: %x temp: %f\n", upper, lower, temp); 

//        int tlm_4a = (int)((95.8 - temp)/1.48 + 0.5);
	tlm[4][A] = (int)((95.8 - temp)/1.48 + 0.5);
//        printf(" 4A: %d \n", tlm_4a);
	
//        tlm[5][A] = (int)((95.8 - (atoi(mopower[UCTEMP]) - 30))/1.48 + 0.5);
        printf(" 5A: %d \n", tlm[5][A]);

//        int tlm_6d = 49 + rand() % 3; 
	tlm[6][D] = 49 + rand() % 3; 


    int k, j;
    for (k = 1; k < 7; k++) {
        for (j = 1; j < 5; j++) {
	printf(" %2d ",	tlm[k][j]);
	}
    printf("\n");
    }	
       return 0;
}
