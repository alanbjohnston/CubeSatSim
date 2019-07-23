/*
 *  Transmits CubeSat Telemetry at 434.9MHz in AO-7 format
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
#include "../Adafruit_INA219/Adafruit_INA219.h" // From Adafruit INA219 library for Arduino

#define A 1
#define B 2
#define C 3
#define D 4

#define PLUS_X 0
#define PLUS_Y 1
#define PLUS_Z 2
#define BAT 3
#define MINUS_X 4
#define MINUS_Y 5
#define MINUS_Z 6
#define BUS 7
#define OFF -1

uint32_t tx_freq_hz = 434900000 + FREQUENCY_OFFSET;
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

uint16_t x_config = (0x2000 | 0x1800 | 0x0180 | 0x0018 | 0x0007 );

struct SensorConfig {
    int fd;
    uint16_t  config;
    int calValue;    
    int powerMultiplier;
    int currentDivider;
};

struct SensorData {
    double current;
    double voltage;
    double power;
};


/**
 * @brief Read the data from one of the i2c current sensors.
 *
 * Reads the current data from the requested i2c current sensor and
 * stores it into a SensorData struct. An invalid file descriptor (i.e. less than zero)
 * results in a SensorData struct being returned that has both its #current and #power members
 * set to NAN.
 *
 *
 * @param sensor A file descriptor that can be used to read from the sensor.
 * @return struct SensorData A struct that contains the power and current reading from the requested sensor.
 */
struct SensorData read_sensor_data(struct SensorConfig sensor) {
    struct SensorData data = {
        .current = NAN,
        .voltage = NAN,
        .power = NAN    };

    if (sensor.fd < 0) {
        return data;
    }
	
    wiringPiI2CWriteReg16(sensor.fd, INA219_REG_CALIBRATION, sensor.calValue);
    wiringPiI2CWriteReg16(sensor.fd, INA219_REG_CONFIG, sensor.config);	
    wiringPiI2CWriteReg16(sensor.fd, INA219_REG_CALIBRATION, sensor.calValue);
//    data.current  = (float)((int16_t)wiringPiI2CReadReg16(sensor.fd, INA219_REG_CURRENT)) / (float)sensor.currentDivider;
    int16_t value = (uint16_t)((wiringPiI2CRead(sensor.fd) << 8 ) | wiringPiI2CRead (sensor.fd));
    data.current  = (float)value / (float)sensor.currentDivider;;
    uint16_t value = (uint16_t)wireReadRegister(sensor.fd, INA219_REG_BUSVOLTAGE);
    data.voltage  =  ((double)(value >> 3) * 4) / 1000;
    data.power   = (float)((uint16_t)wiringPiI2CReadReg16(sensor.fd, INA219_REG_POWER)) * (float)sensor.powerMultiplier;	

    return data;
}
struct SensorConfig config_sensor(int sensor, int milliAmps) {
    struct SensorConfig data;
	
    data.fd = sensor;	
    data.config = INA219_CONFIG_BVOLTAGERANGE_32V |
                  INA219_CONFIG_GAIN_1_40MV | 
                  INA219_CONFIG_BADCRES_12BIT |
                  INA219_CONFIG_SADCRES_12BIT_1S_532US |
                  INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
		 
    if (milliAmps == 400) {	// INA219 16V 400mA configuration
      data.calValue = 8192;    
      data.powerMultiplier = 1; 
      data.currentDivider = 20;  // 40; in Adafruit config
    }
    else  {                     // INA219 16V 2A configuration
      data.calValue = 40960;    
      data.powerMultiplier = 2;  
      data.currentDivider = 10;  // 20; in Adafruit config
    }	
    return data;
}

struct SensorConfig sensor[8];   // 7 current sensors in Solar Power PCB plus one in MoPower UPS V2
struct SensorData reading[8];   // 7 current sensors in Solar Power PCB plus one in MoPower UPS V2 

char src_addr[5] = "";
char dest_addr[5] = "CQ";

int main(int argc, char *argv[]) {
	
  if (argc > 1) {
	  strcpy(src_addr, argv[1]);  
  }

  wiringPiSetup ();
  pinMode (0, OUTPUT);
  
  setSpiChannel(SPI_CHANNEL);
  setSpiSpeed(SPI_SPEED);
  initializeSpi();

  int tlm[7][5];
  memset(tlm, 0, sizeof tlm);

  timestamp = time(NULL);

  int file_i2c = access("/dev/i2c-3", W_OK | R_OK);
 
  if (file_i2c < 0)
  {
    fprintf(stderr,"ERROR: /dev/ic2-3 bus not present\n");
    tempSensor = OFF;
  } else
  {
    tempSensor = wiringPiI2CSetupInterface("/dev/i2c-3", 0x48);
  }

  #ifdef DEBUG_LOGGING
      fprintf(stderr,"tempSensor: %d \n",tempSensor);	
  #endif
		
  int test;
  if (((test = open("/dev/i2c-1", O_RDWR))) > 0)  // Test if I2C Bus 1 is present
  {
	close(test);
	sensor[PLUS_X] = config_sensor(wiringPiI2CSetupInterface("/dev/i2c-1", 0x40), 400); 
	sensor[PLUS_Y] = config_sensor(wiringPiI2CSetupInterface("/dev/i2c-1", 0x41), 400);
	sensor[PLUS_Z] = config_sensor(wiringPiI2CSetupInterface("/dev/i2c-1", 0x44), 400);
	sensor[BAT]    = config_sensor(wiringPiI2CSetupInterface("/dev/i2c-1", 0x45), 400);
	sensor[BUS]    = config_sensor(wiringPiI2CSetupInterface("/dev/i2c-1", 0x4a), 2400);
  } else
  {
	printf("ERROR: /dev/i2c-1 not present \n");
	sensor[PLUS_X] = config_sensor(OFF, 0);
	sensor[PLUS_Y] = config_sensor(OFF, 0);
	sensor[PLUS_Z] = config_sensor(OFF, 0);
	sensor[BAT]    = config_sensor(OFF, 0);
	sensor[BUS]    = config_sensor(OFF, 0);
  }
  if (((test = open("/dev/i2c-0", O_RDWR))) > 0)  // Test if I2C Bus 0 is present
  {
	close(test);
	sensor[MINUS_X] = config_sensor(wiringPiI2CSetupInterface("/dev/i2c-0", 0x40), 400);
	sensor[MINUS_Y] = config_sensor(wiringPiI2CSetupInterface("/dev/i2c-0", 0x41), 400);
	sensor[MINUS_Z] = config_sensor(wiringPiI2CSetupInterface("/dev/i2c-0", 0x44), 400);

  } else
  {
	printf("ERROR: /dev/i2c-0 not present \n");
	sensor[MINUS_X] = config_sensor(OFF, 0);
	sensor[MINUS_Y] = config_sensor(OFF, 0);
	sensor[MINUS_Z] = config_sensor(OFF, 0);
  }
  #ifdef DEBUG_LOGGING
	printf("Sensor[0] config %d %d %d %d %d\n", 
	       sensor[0].fd, sensor[0].config, sensor[0].calValue, sensor[0].currentDivider, sensor[0].powerMultiplier); 
 	printf("Sensor[BUS] config %d %d %d %d %d\n", 
	       sensor[BUS].fd, sensor[BUS].config, sensor[BUS].calValue, sensor[BUS].currentDivider, sensor[BUS].powerMultiplier); 
  #endif

  int ret;
  uint8_t data[1024];

  tx_freq_hz -= tx_channel * 50000;

  init_rf();

  ax25_init(&hax25, (uint8_t *) dest_addr, '1', (uint8_t *) src_addr, '1',
            AX25_PREAMBLE_LEN,
            AX25_POSTAMBLE_LEN);  
      
  /* Infinite loop */
  for (;;) {
    sleep(1);  // Delay 1 second
    
    #ifdef DEBUG_LOGGING
      fprintf(stderr,"INFO: Getting TLM Data\n");
    #endif
	  
    get_tlm(tlm);

    #ifdef DEBUG_LOGGING
      fprintf(stderr,"INFO: Preparing X.25 packet\n");
    #endif
	  
    char str[1000];
    char tlm_str[1000];

    char header_str[] = "\x03\xf0hi hi ";
    strcpy(str, header_str);
 //   printf("%s-1>CQ-1:hi hi ", CALLSIGN);     
    printf("%s-1>%s-1:hi hi ", (uint8_t *)src_addr, (uint8_t *)dest_addr);     

	  
    int channel;
    for (channel = 1; channel < 7; channel++) {
      sprintf(tlm_str, "%d%d%d %d%d%d %d%d%d %d%d%d ", 
        channel, upper_digit(tlm[channel][1]), lower_digit(tlm[channel][1]),
        channel, upper_digit(tlm[channel][2]), lower_digit(tlm[channel][2]), 
        channel, upper_digit(tlm[channel][3]), lower_digit(tlm[channel][3]), 
        channel, upper_digit(tlm[channel][4]), lower_digit(tlm[channel][4]));
        printf("%s",tlm_str);
        strcat(str, tlm_str);
    }
    printf("\n");
	  
    digitalWrite (0, LOW); 
  
    #ifdef DEBUG_LOGGING
      fprintf(stderr,"INFO: Transmitting X.25 packet\n");
    #endif
    memcpy(data, str, strnlen(str, 256));
    ret = ax25_tx_frame(&hax25, &hax5043, data, strnlen(str, 256));
    if (ret) {
      fprintf(stderr,
              "ERROR: Failed to transmit AX.25 frame with error code %d\n",
              ret);
      exit(EXIT_FAILURE);
    }

    ax5043_wait_for_transmit();
    digitalWrite (0, HIGH);
  
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
  #ifdef DEBUG_LOGGING  
    fprintf(stderr,"Initializing AX5043\n");
  #endif
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
  int count;
  for (count = 0; count < 8; count++)
  {
    reading[count] = read_sensor_data(sensor[count]);	
    #ifdef DEBUG_LOGGING
      printf("Read sensor[%d] % 4.2fV % 6.1fmA % 6.1fmW \n", 
	        count, reading[count].voltage, reading[count].current, reading[count].power); 
    #endif
  }
	    
  tlm[1][A] = (int)(reading[BUS].voltage /15.0 + 0.5) % 100;  // Current of 5V supply to Pi
  tlm[1][B] = (int) (99.5 - reading[PLUS_X].current/10.0) % 100;  // +X current [4]
  tlm[1][C] = (int) (99.5 - reading[MINUS_X].current/10.0) % 100;  			// X- current [10] 
  tlm[1][D] = (int) (99.5 - reading[PLUS_Y].current/10.0) % 100;  // +Y current [7]

  tlm[2][A] = (int) (99.5 - reading[MINUS_Y].current/10.0) % 100;  			// -Y current [10] 
  tlm[2][B] = (int) (99.5 - reading[PLUS_Z].current/10.0) % 100;  // +Z current [10] // was 70/2m transponder power, AO-7 didn't have a Z panel
  tlm[2][C] = (int) (99.5 - reading[MINUS_Z].current/10.0) % 100;  			// -Z current (was timestamp)
  tlm[2][D] = (int)(50.5 + reading[BAT].current/10.0) % 100;   // NiMH Battery current
	
  tlm[3][A] = abs((int)((reading[BAT].voltage * 10.0) - 65.5) % 100);
  tlm[3][B] = (int)(reading[BUS].voltage * 10.0) % 100;      // 5V supply to Pi
		   	
  if (tempSensor != OFF) {
    int tempValue = wiringPiI2CReadReg16(tempSensor, 0); 
    uint8_t upper = (uint8_t) (tempValue >> 8);
    uint8_t lower = (uint8_t) (tempValue & 0xff);
    float temp = (float)lower + ((float)upper / 0x100);
	  
    #ifdef DEBUG_LOGGING
      printf("Temp Sensor Read: %6.1f\n", temp);
    #endif
	  
    tlm[4][A] = (int)((95.8 - temp)/1.48 + 0.5) % 100;
  }
  
  FILE *cpuTempSensor = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
  if (cpuTempSensor) {
		double cpuTemp;
		fscanf (cpuTempSensor, "%lf", &cpuTemp);
		cpuTemp /= 1000;
	  
    #ifdef DEBUG_LOGGING
      printf("CPU Temp Read: %6.1f\n", cpuTemp);
    #endif
	  
    tlm[4][B] = (int)((95.8 - cpuTemp)/1.48 + 0.5) % 100;
		fclose (cpuTempSensor);
  }
  
  tlm[6][B] = 0 ;
  tlm[6][D] = 49 + rand() % 3; 

  #ifdef DEBUG_LOGGING
// Display tlm
    int k, j;
    for (k = 1; k < 7; k++) {
      for (j = 1; j < 5; j++) {
        printf(" %2d ",	tlm[k][j]);
      }
      printf("\n");
    }	
  #endif

  return 0;
}
