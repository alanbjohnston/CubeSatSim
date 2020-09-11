/*
 *  Displays voltage and current sensors for CubeSatSim
 *
 */

#include <fcntl.h>                              
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>

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

int test_i2c_bus(int bus);
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
 	  if (access("/dev/i2c-0", W_OK | R_OK) >= 0)  {   // Test if I2C Bus 0 is present			
	  	printf("/dev/i2c-0 is present\n\n");	    
    	  	char result[128];		
    	  	char command[50] = "timeout 10 i2cdetect -y ";
    	 	strcat (command, "0");
//     	 	printf("Command: %s \n", command);
    	 	FILE *i2cdetect = popen(command, "r");
	
    	 	while (fgets(result, 128, i2cdetect) != NULL) {
    	 		;
//       	 	printf("result: %s", result);
    	 	}	
    	 	int error = pclose(i2cdetect)/256;
//      	 	printf("%s error: %d \n", &command, error);
    	 	if (error != 0) 
    	 	{	
    	 		printf("ERROR: %s bus has a problem \n  Check I2C wiring and pullup resistors \n", "0");
			strcpy(busStr,"1 -1");
    		}													
		else	    
	  		strcpy(busStr,"1 0");	
	  } else
	  {
    	 	printf("ERROR: %s bus has a problem \n  Check software to see if enabled \n", "0");
		strcpy(busStr,"1 -1");  
	  }
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
			
			print("Result: %d \n",test_i2c_bus(3));
			
 			if (access("/dev/i2c-11", W_OK | R_OK) >= 0)  {   // Test if I2C Bus 11 is present			
				printf("/dev/i2c-11 is present\n\n");
				
    				char result[128];		
    				char command[50] = "timeout 10 i2cdetect -y ";
    				strcat (command, "11");
// 			   	printf("Command: %s \n", command);
    				FILE *i2cdetect = popen(command, "r");
	
    				while (fgets(result, 128, i2cdetect) != NULL) {
					;
//        				printf("result: %s", result);
   				 }	
    				int error = pclose(i2cdetect)/256;
//    				printf("%s error: %d \n", &command, error);
   				if (error != 0) 
   				{	
	    				printf("ERROR: %s bus has a problem \n  Check I2C wiring and pullup resistors \n", "11");
					strcpy(busStr,"1 -1");
    				}													
				else
					strcpy(busStr,"1 11");
  			} else {
    				char result[128];		
    				char command[50] = "timeout 10 i2cdetect -y ";
    				strcat (command, "3");
// 			   	printf("Command: %s \n", command);
    				FILE *i2cdetect = popen(command, "r");
	
    				while (fgets(result, 128, i2cdetect) != NULL) {
					;
//        				printf("result: %s", result);
   				 }	
    				int error = pclose(i2cdetect)/256;
//    				printf("%s error: %d \n", &command, error);
   				if (error != 0) 
   				{	
	    				printf("ERROR: %s bus has a problem \n  Check I2C wiring and pullup resistors \n", "3");
					strcpy(busStr,"1 -1");
    				}
				else
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
			
 	  if (access("/dev/i2c-0", W_OK | R_OK) >= 0)  {   // Test if I2C Bus 0 is present			
	  	printf("/dev/i2c-0 is present\n\n");	    
    	  	char result[128];		
    	  	char command[50] = "timeout 10 i2cdetect -y ";
    	 	strcat (command, "0");
//     	 	printf("Command: %s \n", command);
    	 	FILE *i2cdetect = popen(command, "r");
	
    	 	while (fgets(result, 128, i2cdetect) != NULL) {
    	 		;
//       	 	printf("result: %s", result);
    	 	}	
    	 	int error = pclose(i2cdetect)/256;
//      	 	printf("%s error: %d \n", &command, error);
    	 	if (error != 0) 
    	 	{	
    	 		printf("ERROR: %s bus has a problem \n  Check I2C wiring and pullup resistors \n", "0");
			strcpy(busStr,"1 -1");
    		}													
		else	    
	  		strcpy(busStr,"1 0");	
	  } else
	  {
    	 	printf("ERROR: %s bus has a problem \n  Check software to see if enabled \n", "0");
		strcpy(busStr,"1 -1");  
	  }
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
		    if ((current[count1] < 0) && (current[count1] > -1))
			 current[count1] *= (-1.0);
//    #ifdef DEBUG_LOGGING
//		    printf("current: %f\n", current[count1]);
//    #endif
		    token = strtok(NULL, space);	
		}
	  }
    }	
  printf("\n");
	
  printf("+X  | % 4.2f V % 5.0f mA \n", voltage[map[PLUS_X]], current[map[PLUS_X]]);
  printf("+Y  | % 4.2f V % 5.0f mA \n", voltage[map[PLUS_Y]], current[map[PLUS_Y]]);
  printf("+Z  | % 4.2f V % 5.0f mA \n", voltage[map[PLUS_Z]], current[map[PLUS_Z]]);
  printf("-X  | % 4.2f V % 5.0f mA \n", voltage[map[MINUS_X]], current[map[MINUS_X]]);
  printf("-Y  | % 4.2f V % 5.0f mA \n", voltage[map[MINUS_Y]], current[map[MINUS_Y]]);
  printf("-Z  | % 4.2f V % 5.0f mA \n",  voltage[map[MINUS_Z]], current[map[MINUS_Z]]);
  printf("Bat | % 4.2f V % 5.0f mA \n", voltage[map[BAT]], current[map[BAT]]);
  printf("Bus | % 4.2f V % 5.0f mA \n\n", voltage[map[BUS]], current[map[BUS]]);		

  return 0;
}

int test_i2c_bus(int bus)
{
	int output = 1;
	char *busString = "/dev/i2c-3";
	
	if (access(busString, W_OK | R_OK) >= 0)  {   // Test if I2C Bus 0 is present			
//	  	printf("bus is present\n\n");	    
    	  	char result[128];		
    	  	char command[50] = "timeout 10 i2cdetect -y ";
    	 	strcat (command, itoa(bus));
//     	 	printf("Command: %s \n", command);
    	 	FILE *i2cdetect = popen(command, "r");
	
    	 	while (fgets(result, 128, i2cdetect) != NULL) {
    	 		;
//       	 	printf("result: %s", result);
    	 	}	
    	 	int error = pclose(i2cdetect)/256;
//      	 	printf("%s error: %d \n", &command, error);
    	 	if (error != 0) 
    	 	{	
    	 		printf("ERROR: %d bus has a problem \n  Check I2C wiring and pullup resistors \n", bus);
			output = -1;
    		}													
	} else
	{
    	 	printf("ERROR: %d bus has a problem \n  Check software to see if enabled \n", bus);
		output = -1; 
	}
	return(output);	
}
