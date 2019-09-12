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

//#include <unistd.h>                             //Needed for I2C port
#include <fcntl.h>                              //Needed for I2C port
//#include <sys/ioctl.h>                  //Needed for I2C port
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
#include "ina219.h"


#define CALLSIGN "" // Put your callsign here!
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

#define RPITX 1  // Use rpitx instead of ax5043 transmitter

uint32_t tx_freq_hz = 440310000;
uint32_t tx_channel = 0;

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
int charging = 0;

uint16_t config = (0x2000 | 0x1800 | 0x0180 | 0x0018 | 0x0007 );

int x_fd;	// I2C bus 0 address 0x40
int x_powerMultiplier;
int x_currentDivider;
int x_calValue_x;
int y_fd;	// I2C bus 0 address 0x41
int z_fd; 	// I2C bos 0 address 0x44


int main(void) {

  wiringPiSetup () ;
  pinMode (0, OUTPUT) ;
  int blink;
  for (blink = 1; blink < 4 ;blink++)
  {
    digitalWrite (0, HIGH) ; delay (500) ;
    digitalWrite (0,  LOW) ; delay (500) ;
  }
    digitalWrite (0, HIGH) ; 
    
    if (!RPITX)
    {
      setSpiChannel(SPI_CHANNEL);
      setSpiSpeed(SPI_SPEED);
      initializeSpi();
    }
	
    int tlm[7][5];
    memset(tlm, 0, sizeof tlm);

    timestamp = time(NULL);
	
    int file_i2c;
    //char *filenam1e = (char*)"/dev/i2c-3";
    if ((file_i2c = open("/dev/i2c-3", O_RDWR)) < 0)
    {
            fprintf(stderr,"ERROR: /dev/ic2-3 bus not present\n");
            tempSensor = -1;
    } else
    {
            tempSensor = wiringPiI2CSetupInterface("/dev/i2c-3", 0x48);
	    if (wiringPiI2CReadReg16(tempSensor, 0) < 0)
		    tempSensor = -1;
    }

//    fprintf(stderr,"tempSensor: %d \n",tempSensor);	

    int arduinoI2C;
    if ((arduinoI2C = open("/dev/i2c-0", O_RDWR)) < 0)
    {
	    fprintf(stderr,"ERROR: /dev/i2c-0 bus not present\n");    
    } else {    
	arduinoI2C = wiringPiI2CSetupInterface("/dev/i2c-0", 0x4B);
// 	fprintf(stderr,"arduinoI2C: %d\n", arduinoI2C);
        if (arduinoI2C > 0) {
    	  if(wiringPiI2CReadReg16(arduinoI2C,0) < 0) {
       	    arduinoI2C = -1;  // Disable reading of Arduino payload information 
	    fprintf(stderr,"Arduino payload not present\n");
	  }
        } else {
	  fprintf(stderr,"Arduino payload not present\n");
       	}
    }

// new INA219 current reading code

    x_calValue_x = 8192;
    x_powerMultiplier = 1;
    x_currentDivider = 20;
    config = INA219_CONFIG_BVOLTAGERANGE_16V |
                 INA219_CONFIG_GAIN_40MV |
                 INA219_CONFIG_BADCRES_12BIT |
                 INA219_CONFIG_SADCRES_12BIT_4S_2130US |
               //INA219_CONFIG_SADCRES_12BIT_1S_532US |
                 INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

     if ((file_i2c = open("/dev/i2c-0", O_RDWR)) < 0)
     {
            fprintf(stderr,"ERROR: /dev/ic2-0 bus not present\n");
            x_fd = -1;  // Disable reading -X, -Y, and -Z telemetry
	    y_fd = -1;
	    z_fd = -1;
     } else
     {  
         x_fd  = wiringPiI2CSetupInterface("/dev/i2c-0", 0x40);
//         fprintf(stderr,"Opening of -X fd %d\n", x_fd);
         y_fd  = wiringPiI2CSetupInterface("/dev/i2c-0", 0x41);
//        printf("Opening of -Y fd %d\n", y_fd);
        z_fd  = wiringPiI2CSetupInterface("/dev/i2c-0", 0x44);
//        printf("Opening of -Z fd %d\n", z_fd);
    }
	
    int ret = 0;
    uint8_t data[1024];

    tx_freq_hz -= tx_channel * 50000;

    if (!RPITX)
    {
    	init_rf();

//    ax25_init(&hax25, (uint8_t *) "CubeSatSim", '2', (uint8_t *) CALLSIGN, '2',
       ax25_init(&hax25, (uint8_t *) "CQ", '1', (uint8_t *) CALLSIGN, '1',
    		AX25_PREAMBLE_LEN,
   		 AX25_POSTAMBLE_LEN);
    }    
        
    /* Infinite loop */
    for (;;) {
        sleep(1);
    	
	fprintf(stderr,"INFO: Getting TLM Data\n");
	    
	get_tlm(tlm);

	fprintf(stderr,"INFO: Preparing X.25 packet\n");
	
	char str[1000];
	char tlm_str[1000];
	
	char header_str[] = "\x03\xf0hi hi ";
	char header_str2[] = "echo -e \'KU2Y>CQ:hi hi ";
	char footer_str[] = "\' | gen_packets -o telem.wav - && cat telem.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/CubeSatSim/rpitx/rpitx -i- -m RF -f 434.9e3 &>/dev/null";
	    
	if (RPITX)
	  strcpy(str, header_str2);
        else		
	  strcpy(str, header_str);
	
        int channel;
	for (channel = 1; channel < 7; channel++) {
//        printf("%d %d %d %d \n", tlm[channel][1], tlm[channel][2], tlm[channel][3], tlm[channel][4]); 
          sprintf(tlm_str, "%d%d%d %d%d%d %d%d%d %d%d%d ", 
	  channel, upper_digit(tlm[channel][1]), lower_digit(tlm[channel][1]),
 	  channel, upper_digit(tlm[channel][2]), lower_digit(tlm[channel][2]), 
	  channel, upper_digit(tlm[channel][3]), lower_digit(tlm[channel][3]), 
	  channel, upper_digit(tlm[channel][4]), lower_digit(tlm[channel][4]));
	  printf("%s \n",tlm_str);
	  strcat(str, tlm_str);
	}

        if (arduinoI2C > 0) {  /* Read Arduino payload */
	  for(int reg = 0; reg < 4; reg++) {
	    sprintf(tlm_str, " %04x",wiringPiI2CReadReg16(arduinoI2C,reg));
	    printf("%s \n",tlm_str);
            strcat(str,tlm_str); /* Append payload telemetry */		
	    usleep(100000);
	  }
	}

    	digitalWrite (0, LOW); 
		
	fprintf(stderr,"INFO: Transmitting X.25 packet\n");
	    
        if (RPITX)
	{
//	  char cmdbuffer[1000];
	  strcat(str,footer_str;
          sleep(2);
	  fprintf(stderr, "String to execute: %s\n", str);
          sleep(2);
	  FILE* file2 = popen(str, "r"); 
//     	  fgets(cmdbuffer, 999, file2);
          sleep(2);
      	  pclose(file2);
//	printf("Response: %s\n", cmdbuffer);
	  fprintf(stderr, "Response\n");
          sleep(2);
	}
	else
	{
	  fprintf(stderr,"Transmitting via AX5043\n");
          memcpy(data, str, strnlen(str, 256));
          ret = ax25_tx_frame(&hax25, &hax5043, data, strnlen(str, 256));
          if (ret) {
            fprintf(stderr,
                    "ERROR: Failed to transmit AX.25 frame with error code %d\n",
                    ret);
            exit(EXIT_FAILURE);
          }
          ax5043_wait_for_transmit();
	}
          sleep(2);
	digitalWrite (0, HIGH);
		
/*
      	FILE* file2 = popen("/home/pi/mopower/mpcmd LED_STAT=0", "r"); 
      	fgets(cmdbuffer, 999, file2);
      	pclose(file2);

//      	printf("LED state: %s\n", cmdbuffer);
*/
        if (ret) {
            fprintf(stderr,
                    "ERROR: Failed to transmit entire AX.25 frame with error code %d\n",
                    ret);
            exit(EXIT_FAILURE);
	}
          sleep(2);
	fprintf(stderr, "Continuing \n");
          sleep(2);
    }

    return 0;
}

static void init_rf() {
    int ret;
    fprintf(stderr,"Initializing AX5043\n");

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
		fprintf(stderr,"ERROR: Not a digit in lower_digit!\n");
	return digit;
}

// Returns upper digit of a number which must be less than 99
//
int upper_digit(int number) {

	int digit = 0;
	if (number < 100) 
		digit = (int)(number/10);
	else
		fprintf(stderr,"ERROR: Not a digit in upper_digit!\n");
	return digit;
}
int get_tlm(int tlm[][5]) {
	

//  Reading I2C voltage and current sensors	
	
      char cmdbuffer[1000];
      FILE* file = popen("sudo python /home/pi/CubeSatSim/python/readcurrent.py 2>&1", "r"); 
      fgets(cmdbuffer, 999, file);
      pclose(file);
      fprintf(stderr,"I2C Sensor data: %s\n", cmdbuffer);

      char ina219[16][20];  // voltage, currents, and power from the INA219 current sensors x4a, x40, x41, x44, and x45.
      int i = 0;
      char * data2 = strtok (cmdbuffer," ");

      while (data2 != NULL) {
          strcpy(ina219[i], data2);
  //        printf ("ina219[%d]=%s\n",i,ina219[i]);
          data2 = strtok (NULL, " ");
          i++;
      }
	
  // Reading MoPower telemetry info
/*	
      file = popen("/home/pi/mopower/mpcmd show data", "r"); 

      fgets(cmdbuffer, 999, file);

      pclose(file);
//      printf("MoPower data: %s\n", cmdbuffer);

    char mopower[64][14];
//  char str[] ="- This, a sample string.";
    char * pch;
//  printf ("Splitting string \"%s\" into tokens:\n",str);
//  pch = strtok (str," ");
    i = 0;
    pch = strtok (cmdbuffer," ,.-");
    while (pch != NULL)
    {
      strcpy(mopower[i], pch);
//      printf ("mopwer[%d]=%s\n",i,mopower[i]); // pch);
      pch = strtok (NULL, " ");
      i++;
    }
    printf("Battery voltage = %s\n", mopower[16]);	
    if (strtof(mopower[17],NULL) > -0.5) {
	charging = 1;
        printf("Charging on\n");
    }
    else {
 	charging = 0;
        printf("Charging off\n");

    }
*/	
// read i2c current sensors //
    double current = 0, power = 0, y_current = 0, y_power = 0, z_current = 0, z_power = 0;	
    if (x_fd != -1) {	
	wiringPiI2CWriteReg16(x_fd, INA219_REG_CALIBRATION, x_calValue_x);
	wiringPiI2CWriteReg16(x_fd, INA219_REG_CONFIG, config);	
	wiringPiI2CWriteReg16(x_fd, INA219_REG_CALIBRATION, x_calValue_x);
	current  = wiringPiI2CReadReg16(x_fd, INA219_REG_CURRENT) / x_currentDivider;
	power  = wiringPiI2CReadReg16(x_fd, INA219_REG_POWER) * x_powerMultiplier;	
	wiringPiI2CWriteReg16(y_fd, INA219_REG_CALIBRATION, x_calValue_x);
	wiringPiI2CWriteReg16(y_fd, INA219_REG_CONFIG, config);	
	wiringPiI2CWriteReg16(y_fd, INA219_REG_CALIBRATION, x_calValue_x);
	y_current  = wiringPiI2CReadReg16(y_fd, INA219_REG_CURRENT) / x_currentDivider;
	y_power  = wiringPiI2CReadReg16(y_fd, INA219_REG_POWER) * x_powerMultiplier;
	wiringPiI2CWriteReg16(z_fd, INA219_REG_CALIBRATION, x_calValue_x);
	wiringPiI2CWriteReg16(z_fd, INA219_REG_CONFIG, config);	
	wiringPiI2CWriteReg16(z_fd, INA219_REG_CALIBRATION, x_calValue_x);
	z_current  = wiringPiI2CReadReg16(z_fd, INA219_REG_CURRENT) / x_currentDivider;
	z_power  = wiringPiI2CReadReg16(z_fd, INA219_REG_POWER) * x_powerMultiplier;
    }	
	printf("-X 0x40 current %4.2f power %4.2f -Y 0x41 current %4.2f power %4.2f -Z 0x44 current %4.2f power %4.2f \n",
	       current, power, y_current, y_power, z_current, z_power);
	
//	printf("1B: ina219[%d]: %s val: %f \n", SENSOR_40 + CURRENT, ina219[SENSOR_40 + CURRENT], strtof(ina219[SENSOR_40 + CURRENT], NULL));

	tlm[1][A] = (int)(strtof(ina219[SENSOR_4A + CURRENT], NULL) / 15 + 0.5) % 100;  // Current of 5V supply to Pi
	tlm[1][B] = (int) (99.5 - strtof(ina219[SENSOR_40 + CURRENT], NULL)/10) % 100;  // +X current [4]
	tlm[1][C] = (int) (99.5 - current/10) % 100;  			// X- current [10] 
	tlm[1][D] = (int) (99.5 - strtof(ina219[SENSOR_41 + CURRENT], NULL)/10) % 100;  // +Y current [7]
	
	tlm[2][A] = (int) (99.5 - y_current/10) % 100;  			// -Y current [10] 
	tlm[2][B] = (int) (99.5 - strtof(ina219[SENSOR_44 + CURRENT], NULL)/10) % 100;  // +Z current [10] // was 70/2m transponder power, AO-7 didn't have a Z panel
	tlm[2][C] = (int) (99.5 - z_current/10) % 100;  			// -Z current (was timestamp)
	
//	tlm[2][C] = (int)((time(NULL) - timestamp) / 15) % 100; 
	tlm[2][D] = (int)(50.5 + strtof(ina219[SENSOR_45 + CURRENT], NULL)/10.0) % 100;   // NiMH Battery current
	
	tlm[3][A] = abs((int)((strtof(ina219[SENSOR_45 + VOLTAGE], NULL) * 10) - 65.5) % 100);
	tlm[3][B] = (int)(strtof(ina219[SENSOR_4A + VOLTAGE], NULL) * 10.0) % 100;      // 5V supply to Pi
		   	
        if (tempSensor != -1) {
		int tempValue = wiringPiI2CReadReg16(tempSensor, 0); 
//		printf("Read: %x\n", tempValue);
	        uint8_t upper = (uint8_t) (tempValue >> 8);
        	uint8_t lower = (uint8_t) (tempValue & 0xff);
        	float temp = (float)lower + ((float)upper / 0x100);

		tlm[4][A] = (int)((95.8 - temp)/1.48 + 0.5) % 100;
	}	
	FILE *cpuTempSensor = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
        if (cpuTempSensor) {
		double cpuTemp;
		fscanf (cpuTempSensor, "%lf", &cpuTemp);
		cpuTemp /= 1000;
		printf("CPU Temp Read: %6.1f\n", cpuTemp);
		tlm[4][B] = (int)((95.8 - cpuTemp)/1.48 + 0.5) % 100;
		fclose (cpuTempSensor);
	}
/*
FILE *temperatureFile;
double T;
temperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
if (temperatureFile == NULL)
  ; //print some message
fscanf (temperatureFile, "%lf", &T);
T /= 1000;
printf ("The temperature is %6.3f C.\n", T);
fclose (temperatureFile);
tlm[4][B] = (int)((95.8 - T)/1.48 + 0.5) % 100;
*/
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
