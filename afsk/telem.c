/*
 *  Generates telemetry for CubeSat Simulator
 *
 *  Copyright Alan B. Johnston
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
 *
 *  INA219 Raspberry Pi wiringPi code is based on Adafruit Arduino wire code
 *  from https://github.com/adafruit/Adafruit_INA219.
 */

#include <fcntl.h>                              
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "status.h"
#include "ax5043.h"
#include "ax25.h"
#include "spi/ax5043spi.h"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <time.h>
#include <math.h>
#include "Adafruit_INA219.h" // From Adafruit INA219 library for Arduino

#define A 1
#define B 2
#define C 3
#define D 4

#define PLUS_X 0
#define PLUS_Y 1
#define BAT 2
#define BUS 3
#define MINUS_X 4
#define MINUS_Y 5
#define PLUS_Z 6
#define MINUS_Z 7

#define OFF -1
#define ON 1

const char pythonCmd[] = "python3 /home/pi/CubeSatSim/python/voltcurrent.py ";
char pythonStr[100], pythonConfigStr[100], busStr[10];
int map[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

int main(int argc, char *argv[]) {
	
  if (argc > 1) {
	  ;
  }

  wiringPiSetup ();
		
  printf("\n");
	
    pinMode (2, INPUT);
    pullUpDnControl (2, PUD_UP);

    if (digitalRead(2) != HIGH)
    {
	  printf("vB3 with TFB Present\n");
  	  map[BUS] = MINUS_Z;
  	  map[BAT] = BUS;
  	  map[PLUS_Z] = BAT;
  	  map[MINUS_Z] = PLUS_Z;
	  strcpy(busStr,"1 0");		
    } 
    else
    {
  	pinMode (3, INPUT);
  	pullUpDnControl (3, PUD_UP);

  	if (digitalRead(3) != HIGH)
  	{
	  printf("vB4 Present\n");
	  map[BAT] = BUS;
	  map[BUS] = BAT;
  	  strcpy(busStr,"1 0");
  	}
	else
	{
		pinMode (26, INPUT);
  		pullUpDnControl (26, PUD_UP);

  		if (digitalRead(26) != HIGH)
  		{
  			printf("vB5 Present\n");  // Don't print normal board detection
			
 			if (access("/dev/i2c-11", W_OK | R_OK) >= 0)  {   // Test if I2C Bus 11 is present			
				printf("/dev/i2c-11 is present\n\n");		
				strcpy(busStr,"1 11");
  			} else {
				strcpy(busStr,"1 3");
 			}	  
  		}
		else
		{
			printf("VB3 Present\n");
  			map[BUS] = MINUS_Z;
  			map[BAT] = BUS;
  			map[PLUS_Z] = BAT;
  			map[MINUS_Z] = PLUS_Z;
			strcpy(busStr,"1 0");			
  		}
	  }
    }	
	
//  Reading I2C voltage and current sensors
//   printf("Starting\n");

   strcpy(pythonStr, pythonCmd);
   strcat(pythonStr, busStr);
   strcat(pythonConfigStr, pythonStr);
   strcat(pythonConfigStr, " c");
	
   FILE* file1 = popen(pythonConfigStr, "r");
   char cmdbuffer[1000];
   fgets(cmdbuffer, 1000, file1);
//   printf("pythonStr result: %s\n", cmdbuffer);
   pclose(file1);	
	
   int count1;
   char *token;
	
   FILE* file = popen(pythonStr, "r");
   fgets(cmdbuffer, 1000, file);
//  printf("result: %s\n", cmdbuffer);
    pclose(file);
	
    const char space[2] = " ";
    token = strtok(cmdbuffer, space);

    float voltage[9], current[9];	
    memset(voltage, 0, sizeof(voltage));
    memset(current, 0, sizeof(current));	 
	  
    for (count1 = 0; count1 < 8; count1++)
    {
	    if (token != NULL)
	    {
	        voltage[count1] = atof(token);				      
//    #ifdef DEBUG_LOGGING
//		 printf("voltage: %f ", voltage[count1]);
//    #endif
		token = strtok(NULL, space);	
	    	if (token != NULL)
	    	{
	            current[count1] = atof(token);
//    #ifdef DEBUG_LOGGING
//		    printf("current: %f\n", current[count1]);
//    #endif
		    token = strtok(NULL, space);	
		}
	  }
    }	
  printf("\n");
	
  printf("+X  | % 4.2fV % 6.1fmA \n", voltage[map[PLUS_X]], current[map[PLUS_X]]);
	
  printf("+Y  | % 4.2fV % 6.1fmA \n", voltage[map[PLUS_Y]], current[map[PLUS_Y]]);
	
  //sensorV  = config_sensor("/dev/i2c-0", 0x40, 400);
  //readingV = read_sensor_data(sensorV);
  printf("+Z  | % 4.2fV % 6.1fmA \n", voltage[map[PLUS_Z]], current[map[PLUS_Z]]);
	
//  sensorV = config_sensor("/dev/i2c-0", 0x41, 400);
//  readingV = read_sensor_data(sensorV);
  printf("-X  |  % 4.2fV % 6.1fmA  \n", voltage[map[MINUS_X]], current[map[MINUS_X]]);
		
//  sensorV = config_sensor("/dev/i2c-0", 0x44, 400);
//  readingV = read_sensor_data(sensorV);
  printf("-Y  | % 4.2fV % 6.1fmA \n", voltage[map[[MINUS_Y]], current[map[MINUS_Y]]);
		
//sensorV = config_sensor("/dev/i2c-0", 0x45, 400); 
//  readingV = read_sensor_data(sensorV);
  printf("-Z  | % 4.2fV % 6.1fmA \n",  voltage[map[MINUS_Z]], current[map[MINUS_Z]]);

//  sensorV     = config_sensor("/dev/i2c-1", 0x45, 400);
//  readingV = read_sensor_data(sensorV);
  printf("Bat | % 4.2fV % 6.1fmA  \n", voltage[map[BAT]], current[map[BAT]]);
		
 // sensorV     = config_sensor("/dev/i2c-1", 0x44, 400);
 // readingV = read_sensor_data(sensorV);
  printf("Bus |  % 4.2fV % 6.1fmA  \n", voltage[map[BUS]], current[map[BUS]]);
*/		
  printf("\n\n");

  return 0;
}
