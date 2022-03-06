/*
 *  Displays voltage and current sensors for CubeSatSim
 *
 *  uses python3 code ina219.py
 *
 */

#include "main.h"

int main(int argc, char *argv[]) {
  int debug = OFF;	
  if (argc > 1) {
    if ( * argv[1] == 'd') {
      debug = ON;
    }
  }

  printf("CubeSatSim v1.2 INA219 Voltage and Current Telemetry\n");  
  map[MINUS_X] = MINUS_Y;
  map[PLUS_Z] = MINUS_X;	
  map[MINUS_Y] = PLUS_Z;			
  snprintf(busStr, 10, "%d %d", test_i2c_bus(1), test_i2c_bus(3));
	
//  Reading I2C voltage and current sensors
//   printf("Starting\n");
	    
   strcpy(pythonStr, pythonCmd);
   strcat(pythonStr, busStr);
   strcat(pythonConfigStr, pythonStr);
   strcat(pythonConfigStr, " s");

   char cmdbuffer[1000];
   FILE *file1 = sopen(pythonConfigStr);  // try new function
   fgets(cmdbuffer, 1000, file1);
   if (debug == ON)
   {
	printf("New Bus String: %s \n", busStr);
	fprintf(stderr, "pythonConfigStr: %s \n", pythonConfigStr);
   	fprintf(stderr, "pythonStr result: %s\n", cmdbuffer);	
   }
	
   int count1;
   char *token;
		
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
  fclose(file1);

  return 0;
}

int test_i2c_bus(int bus)
{
	int output = bus; // return bus number if OK, otherwise return -1
	char busDev[20] = "/dev/i2c-";
	char busS[5];
	snprintf(busS, 5, "%d", bus);
	strcat (busDev, busS);	
//	printf("Bus Dev String: %s \n", busDev);
	
	if (access(busDev, W_OK | R_OK) >= 0)  {   // Test if I2C Bus is present
//	  	printf("bus is present\n\n");	    
    	  	char result[128];		
    	  	const char command_start[] = "timeout 10 i2cdetect -y ";
		char command[50];
		strcpy (command, command_start);
    	 	strcat (command, busS);
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
	return(output);	// return bus number or -1 if there is a problem with the bus
}

// code by https://stackoverflow.com/questions/25161377/open-a-cmd-program-with-full-functionality-i-o/25177958#25177958

    FILE *sopen(const char *program)
    {
        int fds[2];
        pid_t pid;

        if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) < 0)
            return NULL;

        switch(pid=vfork()) {
        case -1:    /* Error */
            close(fds[0]);
            close(fds[1]);
            return NULL;
        case 0:     /* child */
            close(fds[0]);
            dup2(fds[1], 0);
            dup2(fds[1], 1);
            close(fds[1]);
            execl("/bin/sh", "sh", "-c", program, NULL);
            _exit(127);
        }
        /* parent */
        close(fds[1]);
        return fdopen(fds[0], "r+");
    }
