/*
 *  A sample application transmitting 1k2 AFSK in X.25 format
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
/*	    
	FILE* file = popen("mpcmd show data 2>&1", "r");

      char cmdbuffer[1000];
      fgets(cmdbuffer, 1000, file);
      pclose(file);
      printf("buffer is :%s\n", cmdbuffer);

 
      char mopower[64][14];
      char * data2;
      int i = 0;
      data2 = strtok (cmdbuffer," ");

      while (data2 != NULL)

      {
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
	int tlm_2c = (int)((time - timestamp) / 15) % 100; 
	printf("Relative time: %ld seconds 2C: %d  2C: %d%d\n", time - timestamp,tlm_2c, upper_digit(tlm_2c), lower_digit(tlm_2c));

        float vbat;
        vbat = strtof(mopower[VBATT], NULL);
        printf(" vbat: %f \n", vbat);
        int tlm_3a = (int)((vbat * 10) - 65.5);
	int tlm_6b = 0, tlm_2b = 99;
	
        printf("TLM 3A = %d \n", tlm_3a);

	printf("Config = %d %x \n", config, config);
	    
       // Read current from I2C bus
    	i2cDevice = wiringPiI2CSetup (0x40) ;
       	int result = wiringPiI2CWriteReg16(i2cDevice, 0x00, config);
        printf("\n\n1 I2C result: %d\n", result);
        result = wiringPiI2CWriteReg16(i2cDevice, 0x05, 4096);
        printf("Write result: %d\n", result);
        int currentValue = wiringPiI2CReadReg16(i2cDevice, 0x04);
        printf("Current: %d\n\n", currentValue);

    	i2cDevice = wiringPiI2CSetup (0x41) ;
        result = wiringPiI2CWriteReg16(i2cDevice, 0x00, config);
        printf("\n\n2 I2C result: %d\n", result);
        result = wiringPiI2CWriteReg16(i2cDevice, 0x05, 4096);
        printf("Write result: %d\n", result);
        currentValue = wiringPiI2CReadReg16(i2cDevice, 0x04);
        printf("Current: %d\n\n", currentValue);

    	i2cDevice = wiringPiI2CSetup (0x44) ;
     	result = wiringPiI2CWriteReg16(i2cDevice, 0x00, config);
        printf("\n\n3 I2C result: %d\n", result);
    	result = wiringPiI2CWriteReg16(i2cDevice, 0x05, 4096);
        printf("Write result: %d\n", result);
        currentValue = wiringPiI2CReadReg16(i2cDevice, 0x04);
        printf("Current: %d\n\n", currentValue);

    	i2cDevice = wiringPiI2CSetup (0x45) ;
	result = wiringPiI2CWriteReg16(i2cDevice, 0x00, config);
        printf("\n\n4 I2C result: %d\n", result);
  	result = wiringPiI2CWriteReg16(i2cDevice, 0x05, 4096);
        printf("Write result: %d\n", result);
        currentValue = wiringPiI2CReadReg16(i2cDevice, 0x04);
        printf("Current: %d\n\n\n", currentValue);

        int tlm_1b = (int) (98.5 - currentValue/400);
        printf("TLM 1B = %d \n\n", tlm_1b);
	int tlm_1a = 0, tlm_1c = 98, tlm_1d = 98, tlm_2a = 98;
*/
//  Reading 5V voltage and current
      char cmdbuffer[1000];
      int tlm_3a = 0, tlm_6b = 0, tlm_2b = 99;

      file = popen("sudo python /home/pi/CubeSatSim/python/readcurrent.py 2>&1", "r"); 

      fgets(cmdbuffer, 1000, file);
      pclose(file);
      printf("Current buffer is:%s\n", cmdbuffer);

      char battery[3][14];
      i = 0;
      data2 = strtok (cmdbuffer," ");

      while (data2 != NULL)

      {
        strcpy(battery[i], data2);
        printf ("battery[%d]=%s\n",i,battery[i]);
        data2 = strtok (NULL, " ");
        i++;
      }
	int tlm_3b = (int)(strtof(battery[0], NULL) * 10.0);
	int tlm_2d = (int)(50.0 + strtof(battery[1], NULL)/40.0);
	printf(" 2D: %d 3B: %d\n", tlm_2d, tlm_3b);

        int tempValue = wiringPiI2CReadReg16(tempSensor, 0); 
        printf("Read: %x\n", tempValue);

        uint8_t upper = (uint8_t) (tempValue >> 8);
        uint8_t lower = (uint8_t) (tempValue & 0xff);
        float temp = (float)lower + ((float)upper / 0x100);
        printf("upper: %x lower: %x temp: %f\n", upper, lower, temp); 
       
        int tlm_4a = (int)((95.8 - temp)/1.48 + 0.5);
        printf(" 4A: %d \n", tlm_4a);
        
        int tlm_6d = 49 + rand() % 3; 
       
       char tlm_str[1000];

       printf("%d %d %d %d %d %d %d %d %d %d %d %d %d \n", tlm_1a, tlm_1b, tlm_1c, tlm_1d, tlm_2a, tlm_2b, tlm_2c, tlm_2d, tlm_3a, tlm_3b, tlm_4a, tlm_6b, tlm_6d); 
       sprintf(tlm_str, "\x03\x0fhi hi 1%d%d 1%d%d 1%d%d 1%d%d 2%d%d 2%d%d 2%d%d 2%d%d 3%d%d 3%d%d 300 300 4%d%d 400 400 400 400 500 500 500 500 600 6%d%d 600 6%d%d\n", 
		upper_digit(tlm_1a), lower_digit(tlm_1a), 
		upper_digit(tlm_1b), lower_digit(tlm_1b), 
		upper_digit(tlm_1c), lower_digit(tlm_1c), 
		upper_digit(tlm_1d), lower_digit(tlm_1d), 
		upper_digit(tlm_2a), lower_digit(tlm_2a), 
		upper_digit(tlm_2b), lower_digit(tlm_2b), 
		upper_digit(tlm_2c), lower_digit(tlm_2c), 
		upper_digit(tlm_2d), lower_digit(tlm_2d), 
		upper_digit(tlm_3a), lower_digit(tlm_3a), 
		upper_digit(tlm_3b), lower_digit(tlm_3b), 
		upper_digit(tlm_4a), lower_digit(tlm_4a), 
		upper_digit(tlm_6b), lower_digit(tlm_6b), 
		upper_digit(tlm_6d), lower_digit(tlm_6d)); 
       printf("%s\n",tlm_str);



        memcpy(data, tlm_str, strnlen(tlm_str, 256));
        ret = ax25_tx_frame(&hax25, &hax5043, data, strnlen(tlm_str, 256));
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
