/*
 *  Transmits CubeSat Telemetry at 434.9MHz in AO-7 format
 *
 *  Copyright Alan B. Johnston
 *
 *  Portions Copyright (C) 2018 Jonathan Brandenburg
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or/
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
#include <wiringSerial.h>
#include <time.h>
#include <math.h>
#include "Adafruit_INA219.h" // From Adafruit INA219 library for Arduino
#include "make_wav.h"
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8080

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
#define ON 1

uint32_t tx_freq_hz = 434900000 + FREQUENCY_OFFSET;
uint8_t data[1024];
uint32_t tx_channel = 0;

ax5043_conf_t hax5043;
ax25_conf_t hax25;

int twosToInt(int val, int len);
int get_tlm(void);
int get_tlm_fox();
int encodeA(short int *b, int index, int val);
int encodeB(short int *b, int index, int val);
void config_x25();
void trans_x25();
int upper_digit(int number);
int lower_digit(int number);
static int init_rf();
int socket_open = 0;
int sock = 0;
int loop = -1, loop_count = 0;
int firstTime = ON;
long start;
int testCount = 0;

short int buffer[2336400];  // max size for 10 frames count of BPSK

#define S_RATE  (48000)     // (44100)

#define AFSK 1
#define FSK 2
#define BPSK 3
#define CW 4

int rpitxStatus = -1;

float amplitude; // = ; // 20000; // 32767/(10%amp+5%amp+100%amp)
float freq_Hz = 3000;  // 1200
	
int smaller;
int flip_ctr = 0;
int phase = 1;
int ctr = 0;
int rd = 0;
int nrd;
void write_to_buffer(int i, int symbol, int val);	
void write_wave(int i, short int *buffer);
  int uart_fd;

//#define BUF_LEN (FRAME_CNT * (SYNC_BITS + 10 * (8 + 6 * DATA_LEN + 96)) * SAMPLES)     
//#define BUF_LEN (FRAME_CNT * (SYNC_BITS + 10 * (HEADER_LEN + RS_FRAMES * (RS_FRAME_LEN + PARITY_LEN))) * SAMPLES)    
//short int buffer[BUF_LEN];
//short int data10[HEADER_LEN + RS_FRAMES * (RS_FRAME_LEN + PARITY_LEN)];
//short int data8[HEADER_LEN + RS_FRAMES * (RS_FRAME_LEN + PARITY_LEN)]; 
int reset_count;
float uptime_sec;
long int uptime;
char call[5];

int bitRate, mode, bufLen, rsFrames, payloads, rsFrameLen, dataLen, headerLen, syncBits, syncWord, parityLen, samples, frameCnt, samplePeriod; 
float sleepTime;
int sampleTime = 0, frames_sent = 0;
int cw_id = ON;
int vB4 = FALSE, vB5 = FALSE, ax5043 = FALSE, transmit = FALSE, onLed, onLedOn, onLedOff, txLed, txLedOn, txLedOff, payload = OFF;
float batteryThreshold = 0;

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
 * Reads the current data from the requested i2c current sensor configuration and
 * stores it into a SensorData struct. An invalid file descriptor (i.e. less than zero)
 * results in a SensorData struct being returned that has both its #current and #power members
 * set to NAN.
 *
 * @param sensor A structure containing sensor configuration including the file descriptor.
 * @return struct SensorData A struct that contains the current, voltage, and power readings
 * from the requested sensor.
 */
struct SensorData read_sensor_data(struct SensorConfig sensor) {
    struct SensorData data = {
        .current = 0,
        .voltage = 0,
        .power = 0    };

    if (sensor.fd < 0) {
        return data;
    }
    // doesn't read negative currents accurately, shows -0.1mA	
    wiringPiI2CWriteReg16(sensor.fd, INA219_REG_CALIBRATION, sensor.calValue);
    wiringPiI2CWriteReg16(sensor.fd, INA219_REG_CONFIG, sensor.config);	
    wiringPiI2CWriteReg16(sensor.fd, INA219_REG_CALIBRATION, sensor.calValue);
    sleep(0.01);
    int value  = wiringPiI2CReadReg16(sensor.fd, INA219_REG_CURRENT);
    if (value == -1)
    {
	sensor.fd = -1;
	return data;
    }
    data.current  = (float) twosToInt(value, 16) / (float) sensor.currentDivider;
	
    wiringPiI2CWrite(sensor.fd, INA219_REG_BUSVOLTAGE);
    delay(1); // Max 12-bit conversion time is 586us per sample
    value = (wiringPiI2CRead(sensor.fd) << 8 ) | wiringPiI2CRead (sensor.fd);	
    data.voltage  =  ((float)(value >> 3) * 4) / 1000;	
    // power has very low resolution, seems to step in 512mW values	
    data.power   = (float) wiringPiI2CReadReg16(sensor.fd, INA219_REG_POWER) * (float) sensor.powerMultiplier;
 	
    return data;
}

/**
 * @brief Configures an i2c current sensor.
 *
 * Calculates the configuration values of the i2c sensor so that
 * current, voltage, and power can be read using read_sensor_data.
 * Supports 16V 400mA and 16V 2.0A settings.
 *
 * @param sensor A file descriptor that can be used to read from the sensor.
 * @param milliAmps The mA configuration, either 400mA or 2A are supported.
 * @return struct SensorConfig A struct that contains the configuraton of the sensor.
 */
//struct SensorConfig config_sensor(int sensor, int milliAmps) {
struct SensorConfig config_sensor(char *bus, int address,  int milliAmps) {
    struct SensorConfig data;
	
    if (access(bus, W_OK | R_OK) < 0)  {   // Test if I2C Bus is missing 
	    printf("ERROR: %s bus not present\n  Check raspi-config Interfacing Options/I2C and /boot/config.txt  \n", bus);
	    data.fd = OFF;
	    return (data);
    }
    char result[128];	
    int pos = strlen(bus) / sizeof(bus[0]) - 1;	
 //    printf("Bus size %d \n", pos);	
 //    printf("Bus value %d \n", atoi(&bus[pos]));	
    char command[50] = "timeout 10 i2cdetect -y ";	
    strcat (command, &bus[pos]);	
    FILE *i2cdetect = popen(command, "r");	
 		
    while (fgets(result, 128, i2cdetect) != NULL) {	
 	;	
 //        printf("result: %s", result);	
 	    }	
 		
    int error = pclose(i2cdetect)/256;	
	
 //    printf("%s error: %d \n", &command, error);	
    if (error != 0)	
    {	
 	printf("ERROR: %s bus has a problem \n  Check I2C wiring and pullup resistors \n", bus);	
 	data.fd = OFF;	
 	return (data);	
    }
	
    data.fd = wiringPiI2CSetupInterface(bus, address);	
	
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
	
    #ifdef DEBUG_LOGGING
	printf("Sensor %s %x configuration: %d %d %d %d %d\n", bus, address, data.fd,
	       data.config, data.calValue, data.currentDivider, data.powerMultiplier); 
    #endif	
    return data;
}

struct SensorConfig sensor[8];   // 8 current sensors in Solar Power PCB vB4/5
struct SensorData reading[8];   // 8 current sensors in Solar Power PCB vB4/5
struct SensorConfig tempSensor; 

char src_addr[5] = "";
char dest_addr[5] = "CQ";

int main(int argc, char *argv[]) {

  mode = FSK;
  frameCnt = 1;
	
  if (argc > 1) {
//	  strcpy(src_addr, argv[1]);
	  if (*argv[1] == 'b')
	  {
		  mode = BPSK;
		  printf("Mode BPSK\n");
	  }
	  else if (*argv[1] == 'a')
	  {
		  mode = AFSK;
		  printf("Mode AFSK\n");
	  }
	  else if (*argv[1] == 'c')
	  {
		  mode = CW;
		  printf("Mode CW\n");
	  }
	  else
	  {
		  printf("Mode FSK\n");
	  }

	  if (argc > 2)  {
//		  printf("String is %s %s\n", *argv[2], argv[2]);
		  loop = atoi(argv[2]);
		  loop_count = loop;
	  }
	  printf("Looping %d times \n", loop);
	  
	  if (argc > 3) {
		if (*argv[3] == 'n')
		{
			cw_id = OFF;
			printf("No CW id\n");
		}
	  }
  }
	
// Open configuration file with callsign and reset count	
  FILE* config_file = fopen("/home/pi/CubeSatSim/sim.cfg","r"); 
  if (config_file == NULL) 
  { 
       printf("Creating config file."); 
       config_file = fopen("/home/pi/CubeSatSim/sim.cfg","w");
	fprintf(config_file, "%s %d", " ", 100);
	fclose(config_file);
	config_file = fopen("/home/pi/CubeSatSim/sim.cfg","r"); 
  } 
	
  char* cfg_buf[100]; 
  fscanf(config_file, "%s %d", call, &reset_count);
  fclose(config_file);
  printf("Config file /home/pi/CubeSatSim/sim.cfg contains %s %d\n", call, reset_count); 	
  reset_count = (reset_count + 1) % 0xffff;
	
  wiringPiSetup ();

// Check for SPI and AX-5043 Digital Transceiver Board	
  FILE *file = popen("sudo raspi-config nonint get_spi", "r");
       if (fgetc(file) == 48) 
	{
	  printf("SPI is enabled!\n");	
	       
          FILE *file2 = popen("ls /dev/spidev0.* 2>&1", "r");
//          printf("Result getc: %c \n", getc(file2));

          if (fgetc(file2) != 'l')
          {
            printf("SPI devices present!\n");
//	  }
 	  pclose(file2);
	  setSpiChannel(SPI_CHANNEL);
  	  setSpiSpeed(SPI_SPEED);
  	  initializeSpi();
//	  char src_addr[5] = "KU2Y";
//          char dest_addr[5] = "CQ";
	  ax25_init(&hax25, (uint8_t *) dest_addr, '1', (uint8_t *) call, '1', AX25_PREAMBLE_LEN, AX25_POSTAMBLE_LEN);
	  if (init_rf())
	  {
		printf("AX5043 successfully initialized!\n");
		ax5043 = TRUE;
		cw_id = OFF;
		mode = AFSK;
//		cycle = OFF;
		printf("Mode AFSK with AX5043\n");  
		transmit = TRUE;
	  }	  
	  else
		printf("AX5043 not present!\n");
 	}
      } 
//       else
//       {
//	  printf("SPI not enabled!\n");
//       }
  pclose(file);
  txLed = 0;	// defaults for vB3 board without TFB
  txLedOn = LOW;
  txLedOff = HIGH;
  if (!ax5043)
  {
    pinMode (2, INPUT);
    pullUpDnControl (2, PUD_UP);

    if (digitalRead(2) != HIGH)
    {
	  printf("vB3 with TFB Present\n");
  	  txLed = 3;
	  txLedOn = LOW;
 	  txLedOff = HIGH;
	  onLed = 0;
          onLedOn = LOW;
	  onLedOff = HIGH;
	  transmit = TRUE;
    } else
    {
  	pinMode (3, INPUT);
  	pullUpDnControl (3, PUD_UP);

  	if (digitalRead(3) != HIGH)
  	{
	  printf("vB4 Present\n");
  	  txLed = 2;
	  txLedOn = HIGH;
 	  txLedOff = LOW;
 	  vB4 = TRUE;
	  onLed = 0;
          onLedOn = HIGH;
	  onLedOff = LOW;
	  batteryThreshold = 3.0;
	  transmit = TRUE;
  	}
	else
	{
		pinMode (26, INPUT);
  		pullUpDnControl (26, PUD_UP);

  		if (digitalRead(26) != HIGH)
  		{
	  		printf("vB5 Present\n");
  	  		txLed = 2;
	  		txLedOn = HIGH;
 	  		txLedOff = LOW;
 	  		vB5 = TRUE;
	  		onLed = 27;
          		onLedOn = HIGH;
	  		onLedOff = LOW;
	  		batteryThreshold = 3.0;
	  		transmit = TRUE;
		}
	}
    }
  }	
  pinMode (txLed, OUTPUT);
  digitalWrite (txLed, txLedOff);
  #ifdef DEBUG_LOGGING
	printf("Tx LED Off\n");
  #endif
  pinMode (onLed, OUTPUT);
  digitalWrite (onLed, onLedOn);
  #ifdef DEBUG_LOGGING
	printf("Power LED On\n");
  #endif
	
//    if ((cycle == ON) && !ax5043)  // don't cycle modes if using AX5043
//      mode = (reset_count) % 3;  // alternate between the three modes	
	
    config_file = fopen("sim.cfg","w");
    fprintf(config_file, "%s %d", call, reset_count);
    fclose(config_file);
    config_file = fopen("sim.cfg","r"); 
		
if (vB4)
{	
  sensor[PLUS_X]  = config_sensor("/dev/i2c-1", 0x40, 400); 
  sensor[PLUS_Y]  = config_sensor("/dev/i2c-1", 0x41, 400);
  sensor[BUS]  	  = config_sensor("/dev/i2c-1", 0x44, 400);
  sensor[BAT]     = config_sensor("/dev/i2c-1", 0x45, 400);
  sensor[PLUS_Z]  = config_sensor("/dev/i2c-0", 0x40, 400);
  sensor[MINUS_X] = config_sensor("/dev/i2c-0", 0x41, 400);
  sensor[MINUS_Y] = config_sensor("/dev/i2c-0", 0x44, 400);
  sensor[MINUS_Z] = config_sensor("/dev/i2c-0", 0x45, 400); 
}	
else if (vB5)
{	
  sensor[PLUS_X]  = config_sensor("/dev/i2c-1", 0x40, 400); 
  sensor[PLUS_Y]  = config_sensor("/dev/i2c-1", 0x41, 400);
  sensor[BUS]  	  = config_sensor("/dev/i2c-1", 0x45, 400);
  sensor[BAT]     = config_sensor("/dev/i2c-1", 0x44, 400);
  if (access("/dev/i2c-11", W_OK | R_OK) >= 0)  {   // Test if I2C Bus 11 is present			
	printf("/dev/i2c-11 is present\n\n");		
  	sensor[PLUS_Z]  = config_sensor("/dev/i2c-11", 0x40, 400);
  	sensor[MINUS_X] = config_sensor("/dev/i2c-11", 0x41, 400);
  	sensor[MINUS_Y] = config_sensor("/dev/i2c-11", 0x44, 400);
  	sensor[MINUS_Z] = config_sensor("/dev/i2c-11", 0x45, 400); 
  } else {
  	sensor[PLUS_Z]  = config_sensor("/dev/i2c-3", 0x40, 400);
  	sensor[MINUS_X] = config_sensor("/dev/i2c-3", 0x41, 400);
  	sensor[MINUS_Y] = config_sensor("/dev/i2c-3", 0x44, 400);
  	sensor[MINUS_Z] = config_sensor("/dev/i2c-3", 0x45, 400); 
  }
} 
else
{	
  sensor[PLUS_X]  = config_sensor("/dev/i2c-1", 0x40, 400); 
  sensor[PLUS_Y]  = config_sensor("/dev/i2c-1", 0x41, 400);
  sensor[PLUS_Z]  = config_sensor("/dev/i2c-1", 0x44, 400);
  sensor[BAT]     = config_sensor("/dev/i2c-1", 0x45, 400);
  sensor[BUS]     = config_sensor("/dev/i2c-1", 0x4a, 2000);
  sensor[MINUS_X] = config_sensor("/dev/i2c-0", 0x40, 400);
  sensor[MINUS_Y] = config_sensor("/dev/i2c-0", 0x41, 400);
  sensor[MINUS_Z] = config_sensor("/dev/i2c-0", 0x44, 400);
  tempSensor 	  = config_sensor("/dev/i2c-3", 0x48, 0);  
 }

// try connecting to Arduino payload using UART

 if (!ax5043)  // don't test if AX5043 is present
 {
  payload = OFF;

  if ((uart_fd = serialOpen ("/dev/ttyAMA0", 9600)) >= 0)
  {
     char c;
     unsigned int waitTime;
     int i;
    for(i = 0; i < 2; i++)
    {
     serialPutchar (uart_fd, '?');
     printf("Querying payload with ?\n");
     waitTime = millis() + 500;
     while ((millis() < waitTime) && (payload != ON)) 
     { 
        if (serialDataAvail (uart_fd))
        {
          printf ("%c", c = serialGetchar (uart_fd));
          fflush (stdout);
	  if (c == 'O')
	  {
             printf ("%c", c = serialGetchar (uart_fd));
             fflush (stdout);
             if (c == 'K')
         	 payload = ON;
          }
        }
//        sleep(0.75);
     }
    }
    if (payload == ON)
	  printf ("\nPayload is present!\n") ;
    else
	  printf ("\nPayload not present!\n") ;
  }
   else 
  {
    fprintf (stderr, "Unable to open UART: %s\n", strerror (errno)) ;
  }
 }

  int ret;
  //uint8_t data[1024];

  tx_freq_hz -= tx_channel * 50000;
	
  if (mode == AFSK)
	  sleep(10); // delay awaiting CW ID completion
/*	
   if (transmit == FALSE)
   {
	   
	fprintf(stderr,"\nNo CubeSatSim Band Pass Filter detected.  No transmissions after the CW ID.\n");
	fprintf(stderr, " See http://cubesatsim.org/wiki for info about building a CubeSatSim\n\n");
   }
	
// Send ID in CW (Morse Code)
cw_id = OFF;
if (cw_id == ON)	// Don't send CW if using AX5043 or in mode cycling or set by 3rd argument 
{
  char cw_str[200];
  char cw_header[] = "echo 'de ";
  char cw_footer[] = "' > id.txt && gen_packets -M 20 id.txt -o morse.wav -r 48000 > /dev/null 2>&1 && cat morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.897e3";

  strcpy(cw_str, cw_header);
//printf("Before 1st strcpy\n");
  strcat(cw_str, call);
//printf("Before 1st strcpy\n");
  strcat(cw_str, cw_footer);
//printf("Before 1st strcpy\n");
  digitalWrite (txLed, txLedOn);
  #ifdef DEBUG_LOGGING
	printf("Tx LED On\n");
  #endif
//printf("Before cmd\n");
//printf("CW String: %s\n", cw_str);
//	FILE* f;
	system(cw_str);
//	printf("File %d \n", f);
//  printf("close: %d \n", pclose(f));  // execute command and wait for termination before continuing
printf("After command\n");
//  sleep(7);
//printf("Before Write\n");
  digitalWrite (txLed, txLedOn);
  #ifdef DEBUG_LOGGING
	printf("Tx LED On\n");
  #endif
//printf("After Write\n");
}
//printf("Done CW!\n");
*/	
while (loop-- != 0)
  {
   frames_sent++;
   float batteryVoltage = read_sensor_data(sensor[BAT]).voltage;
   #ifdef DEBUG_LOGGING
      fprintf(stderr,"INFO: Battery voltage: %f V  Battery Threshold %f V\n", batteryVoltage, batteryThreshold);
   #endif	 
   if ((batteryVoltage > 1.0) && (batteryVoltage < batteryThreshold)) // no battery INA219 will give 0V, no battery plugged into INA219 will read < 1V
   {
	fprintf(stderr,"Battery voltage too low: %f V - shutting down!\n", batteryVoltage);
  	digitalWrite (txLed, txLedOff);
        digitalWrite (onLed, onLedOff);
        sleep(1);
        digitalWrite (onLed, onLedOn);
        sleep(1);
        digitalWrite (onLed, onLedOff);
        sleep(1);
        digitalWrite (onLed, onLedOn);
        sleep(1);
        digitalWrite (onLed, onLedOff);
  
	popen("sudo shutdown -h now > /dev/null 2>&1", "r"); 
	sleep(10);
  }
			  
  if (mode == FSK) {	
    bitRate = 200;
    rsFrames = 1;
    payloads = 1;
    rsFrameLen = 64;
    headerLen = 6;
    dataLen = 58;
    syncBits = 10;
    syncWord = 0b0011111010;
    parityLen = 32;
    amplitude = 32767/3;
    samples = S_RATE/bitRate;
    bufLen = (frameCnt * (syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))) * samples);

   samplePeriod = ((float)((syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))))/(float)bitRate) * 1000 - 500;
   sleepTime = 0.1;

    printf("\n FSK Mode, %d bits per frame, %d bits per second, %d ms sample period\n", 
	   bufLen/(samples * frameCnt), bitRate, samplePeriod);
  }
  else if (mode == BPSK) {
    bitRate = 1200;
    rsFrames = 3;
    payloads = 6;
    rsFrameLen = 159;
    headerLen = 8;
    dataLen = 78;
    syncBits = 31;
    syncWord = 0b1000111110011010010000101011101;
    parityLen = 32;
    amplitude = 32767;
    samples = S_RATE/bitRate;
    bufLen = (frameCnt * (syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))) * samples);

//   samplePeriod = ((float)((syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))))/(float)bitRate) * 1000 - 1800;
    samplePeriod = 3000;
    sleepTime = 3.0;

    printf("\n BPSK Mode, bufLen: %d,  %d bits per frame, %d bits per second, %d seconds per frame %d ms sample period\n", 
	   bufLen, bufLen/(samples * frameCnt), bitRate, bufLen/(samples * frameCnt * bitRate), samplePeriod);
  }
	 
  //  sleep(1);  // Delay 1 second
    ctr = 0;
    #ifdef DEBUG_LOGGING
      fprintf(stderr,"INFO: Getting TLM Data\n");
    #endif
	  	  
    if ((mode == AFSK) || (mode == CW))
    {
       get_tlm();
    }
    else // FSK or BPSK
    {
      get_tlm_fox();
    }

    #ifdef DEBUG_LOGGING
      fprintf(stderr,"INFO: Getting ready to send\n");
    #endif	     
 }

  if (mode == BPSK)
  {
	digitalWrite (txLed, txLedOn);
  #ifdef DEBUG_LOGGING
	printf("Tx LED On\n");
  #endif
	printf("Sleeping to allow BPSK transmission to finish.\n");
	sleep(loop_count * 5);
	printf("Done sleeping\n");
	digitalWrite (txLed, txLedOff);  
  #ifdef DEBUG_LOGGING
	printf("Tx LED Off\n");
  #endif
  }
  else if (mode == FSK)
  {
	printf("Sleeping to allow FSK transmission to finish.\n");
	sleep(loop_count);
	printf("Done sleeping\n");
  }
/*	
//  int transmit = popen("timeout 1 sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.897e3","r");
  int txResult = popen("sudo killall -9 rpitx > /dev/null 2>&1", "r");
  pclose(txResult);
  txResult = popen("sudo killall -9 sendiq > /dev/null 2>&1", "r"); 
  pclose(txResult);
  txResult = popen("sudo fuser -k 8080/tcp > /dev/null 2>&1", "r"); 
  pclose(txResult);
	
  if(cw_id == ON) // only turn off Power LED if CW ID is enabled (i.e. not demo.sh mode cycling)
      digitalWrite (onLed, onLedOff);
  #ifdef DEBUG_LOGGING
	printf("Tx LED Off\n");
  #endif
*/	
  return 0;
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

static int init_rf() {
    int ret;
    fprintf(stderr,"Initializing AX5043\n");

    ret = ax5043_init(&hax5043, XTAL_FREQ_HZ, VCO_INTERNAL);
    if (ret != PQWS_SUCCESS) {
        fprintf(stderr,
                "ERROR: Failed to initialize AX5043 with error code %d\n", ret);
 //       exit(EXIT_FAILURE);
        return(0);
    }
    return(1);
}

int get_tlm(void) {

      FILE* txResult;

for (int j = 0; j < frameCnt; j++)	
{	
   digitalWrite (txLed, txLedOn);
  #ifdef DEBUG_LOGGING
	printf("Tx LED On\n");
  #endif
  int tlm[7][5];
  memset(tlm, 0, sizeof tlm);
	
//  Reading I2C voltage and current sensors
  int count;
  for (count = 0; count < 8; count++)
  {
    reading[count] = read_sensor_data(sensor[count]);	
    #ifdef DEBUG_LOGGING
//      printf("Read sensor[%d] % 4.2fV % 6.1fmA % 6.1fmW \n", 
//	        count, reading[count].voltage, reading[count].current, reading[count].power); 
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

  if (ax5043)
  {
   if (tempSensor.fd != OFF) {
    int tempValue = wiringPiI2CReadReg16(tempSensor.fd, 0); 
    uint8_t upper = (uint8_t) (tempValue >> 8);
    uint8_t lower = (uint8_t) (tempValue & 0xff);
    float temp = (float)lower + ((float)upper / 0x100);
	  
    #ifdef DEBUG_LOGGING
      printf("Temp Sensor Read: %6.1f\n", temp);
    #endif
	  
    tlm[4][A] = (int)((95.8 - temp)/1.48 + 0.5) % 100;
   }
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

  }
  fclose (cpuTempSensor);
	
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
	
  char str[1000];
  char tlm_str[1000];
  char header_str[] = "\x03\xf0hi hi ";	
  char header_str3[] = "echo '";
  char header_str2[] = ">CQ:hi hi ";
  char header_str4[] = "hi hi ";	
  char footer_str1[] = "\' > t.txt && echo \'"; 
  char footer_str[] = ">CQ:hi hi ' >> t.txt && gen_packets -o telem.wav t.txt -r 48000 > /dev/null 2>&1 && cat telem.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1";
	
  if (ax5043)
  {
	strcpy(str, header_str);
  }
  else
  {
    strcpy(str, header_str3);
    if (mode != CW) 
    {
    	strcat(str, call);
    	strcat(str, header_str2);
    } else
    {
	strcat(str, header_str4);
    }
  }
 
    int channel;
    for (channel = 1; channel < 7; channel++) {
      sprintf(tlm_str, "%d%d%d %d%d%d %d%d%d %d%d%d ", 
        channel, upper_digit(tlm[channel][1]), lower_digit(tlm[channel][1]),
        channel, upper_digit(tlm[channel][2]), lower_digit(tlm[channel][2]), 
        channel, upper_digit(tlm[channel][3]), lower_digit(tlm[channel][3]), 
        channel, upper_digit(tlm[channel][4]), lower_digit(tlm[channel][4]));
//        printf("%s",tlm_str);
        strcat(str, tlm_str);
    }
// CW
	
  char cw_str2[500];
  char cw_header2[] = "echo '";
  char cw_footer2[] = "' > id.txt && gen_packets -M 20 id.txt -o morse.wav -r 48000 > /dev/null 2>&1 && cat morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.897e3";

  strcpy(cw_str2, cw_header2);
//printf("Before 1st strcpy\n");
  strcat(cw_str2, str);
//printf("Before 1st strcpy\n");
  strcat(cw_str2, cw_footer2);
//printf("Before 1st strcpy\n");
	
// read payload sensor if available
	
char sensor_payload[500];
	
if (payload == ON)
{
     char c;
     unsigned int waitTime;
     int i = 0;

     serialPutchar (uart_fd, '?');
     printf("Querying payload with ?\n");
     waitTime = millis() + 500;
     int end = FALSE;
     while ((millis() < waitTime) && !end) 
     { 
	int chars = serialDataAvail (uart_fd);
        while ((chars-- > 0) && !end)
        {
          c = serialGetchar (uart_fd);
//	  printf ("%c", c);
//	  fflush(stdout);
	  if (c != '\n')
	  {
	  	sensor_payload[i++] = c;
	  }
	  else
	  {
		  end = TRUE;
	  }
        }
    }
    sensor_payload[i] = '\0';
    printf("Payload string: %s\n", sensor_payload);
    strcat(str, sensor_payload);  // append to telemetry string for transmission
}

	
  digitalWrite (txLed, txLedOn);
  #ifdef DEBUG_LOGGING
	printf("Tx LED On\n");
  #endif
//printf("Before cmd\n");
//printf("CW telem String: %s\n", cw_str2);
//	FILE* f;
    if (mode == CW)
	system(cw_str2);
//	printf("File %d \n", f);
//  printf("close: %d \n", pclose(f));  // execute command and wait for termination before continuing
//printf("After command\n");
//  sleep(7);
//printf("Before Write\n");
  digitalWrite (txLed, txLedOn);
  #ifdef DEBUG_LOGGING
	printf("Tx LED On\n");
  #endif
//printf("After Write\n");
//}
//printf("Done CW!\n");
	
  if (ax5043)
  {
    	digitalWrite (txLed, txLedOn); 
  #ifdef DEBUG_LOGGING
	printf("Tx LED On\n");
  #endif
	fprintf(stderr,"INFO: Transmitting X.25 packet using AX5043\n");
        memcpy(data, str, strnlen(str, 256));
        int ret = ax25_tx_frame(&hax25, &hax5043, data, strnlen(str, 256));
        if (ret) {
            fprintf(stderr,
                    "ERROR: Failed to transmit AX.25 frame with error code %d\n",
                    ret);
            exit(EXIT_FAILURE);
        }
        ax5043_wait_for_transmit();
    	digitalWrite (txLed, txLedOff);
  #ifdef DEBUG_LOGGING
	printf("Tx LED Off\n");
  #endif

        if (ret) {
            fprintf(stderr,
                    "ERROR: Failed to transmit entire AX.25 frame with error code %d\n",
                    ret);
            exit(EXIT_FAILURE);
	}
        sleep(2);
  }
  else
  {
     	  strcat(str, footer_str1);
     	  strcat(str, call);
	  strcat(str,footer_str);
	  fprintf(stderr, "String to execute: %s\n", str);
	  if (transmit)
	  {
	  	FILE* file2 = popen(str, "r"); 
      	  	pclose(file2);
	  }
	  else
   	  {
		fprintf(stderr,"\nNo CubeSatSim Band Pass Filter detected.  No transmissions after the CW ID.\n");
		fprintf(stderr, " See http://cubesatsim.org/wiki for info about building a CubeSatSim\n\n");
   	  }
    	  digitalWrite (txLed, txLedOff);
  #ifdef DEBUG_LOGGING
	printf("Tx LED Off\n");
  #endif
	  sleep(3);
   	  digitalWrite (txLed, txLedOn);	  
  #ifdef DEBUG_LOGGING
	printf("Tx LED On\n");
  #endif
  }
	
  //digitalWrite (txLed, txLedOff);
 
   }
	
//printf("End of get_tlm and rpitx =========================================================\n");

   digitalWrite (txLed, txLedOff);
  #ifdef DEBUG_LOGGING
	printf("Tx LED Off\n");
  #endif

return;
}

int get_tlm_fox() {

//   memset(b, 0, 64);
	
//  Reading I2C voltage and current sensors
	
   FILE* uptime_file = fopen("/proc/uptime", "r");
    fscanf(uptime_file, "%f", &uptime_sec);
    uptime = (int) uptime_sec;
  #ifdef DEBUG_LOGGING
    printf("Reset Count: %d Uptime since Reset: %ld \n", reset_count, uptime);
  #endif
    fclose(uptime_file);
	
	int i;
//	long int sync = SYNC_WORD;
	long int sync = syncWord;

	smaller = S_RATE/(2 * freq_Hz);

//	short int b[DATA_LEN];
	short int b[dataLen];
	memset(b, 0, sizeof(b));

//	short int h[HEADER_LEN];
	short int h[headerLen];
	memset(h, 0, sizeof(h));
		
        memset(buffer, 0xa5, sizeof(buffer));
	
//	short int b10[DATA_LEN], h10[HEADER_LEN];
//	short int rs_frame[RS_FRAMES][223];
//	unsigned char parities[RS_FRAMES][PARITY_LEN],inputByte;
//	short int b10[dataLen], h10[headerLen];
	short int rs_frame[rsFrames][223];
	unsigned char parities[rsFrames][parityLen], inputByte;

  int id, frm_type = 0x01, TxTemp = 0, IHUcpuTemp = 0, STEMBoardFailure = 16;
  int PSUVoltage = 0, PSUCurrent = 0; 
  int batt_a_v = 0, batt_b_v = 0, batt_c_v = 0, battCurr = 0;
  int posXv = 0, negXv = 0, posYv = 0, negYv = 0, posZv = 0, negZv = 0;
  int posXi = 0, negXi = 0, posYi = 0, negYi = 0, posZi = 0, negZi = 0;
  int head_offset = 0; 	
// AngularVelocity = 43.3 dps + 1960 
  int xAngularVelocity = 43.3 * (-10) + 1960, yAngularVelocity = 43.3 * (-6) + 1960, zAngularVelocity = 43.3 * (6) + 1960; // XAxisAngularVelocity
	
  short int buffer_test[bufLen];
  int buffSize;
  buffSize = sizeof(buffer_test);
	
  if (mode == FSK)
    id = 7;
  else
    id = 0;  // 99 in h[6]

//  for (int frames = 0; frames < FRAME_CNT; frames++) 
  for (int frames = 0; frames < frameCnt; frames++) 
  {

if (firstTime != ON)
/* 
{//  digitalWrite (3, HIGH);	
  if (mode == BPSK)
	sleep(3); 
 // sleep(3.5); 
//  digitalWrite (3, LOW);	
}
	  
  if (mode == FSK)
*/
 {
// delay for sample period
  digitalWrite (txLed, txLedOn);
  #ifdef DEBUG_LOGGING
	printf("Tx LED On\n");
  #endif

    while ((millis() - sampleTime) < samplePeriod)
        sleep(sleepTime);
  
  digitalWrite (txLed, txLedOff);
  #ifdef DEBUG_LOGGING
	printf("Tx LED Off\n");
  #endif

    printf("Sample period: %d\n",millis() - sampleTime);
    sampleTime = millis();
  }  else
	printf("first time - no sleep\n");
	
    int count;
    for (count = 0; count < 8; count++)
    {
      reading[count] = read_sensor_data(sensor[count]);	
    #ifdef DEBUG_LOGGING
//      printf("Read sensor[%d] % 4.2fV % 6.1fmA % 6.1fmW \n", 
//	        count, reading[count].voltage, reading[count].current, reading[count].power); 
    #endif
    }
/*
    if (tempSensor.fd != OFF) {
      int tempValue = wiringPiI2CReadReg16(tempSensor.fd, 0); 
      uint8_t upper = (uint8_t) (tempValue >> 8);
      uint8_t lower = (uint8_t) (tempValue & 0xff);
      float temp = (float)lower + ((float)upper / 0x100);
	  
    #ifdef DEBUG_LOGGING
      printf("Temp Sensor Read: %6.1f\n", temp);
    #endif

      TxTemp = (int)((temp * 10.0) + 0.5);
      encodeB(b, 34 + head_offset,  TxTemp);
  }
*/  
  FILE *cpuTempSensor = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
  if (cpuTempSensor) {
		double cpuTemp;
		fscanf (cpuTempSensor, "%lf", &cpuTemp);
		cpuTemp /= 1000;
	  
    #ifdef DEBUG_LOGGING
      printf("CPU Temp Read: %6.1f\n", cpuTemp);
    #endif
	  
    IHUcpuTemp = (int)((cpuTemp * 10.0) + 0.5);
   }	  
   fclose(cpuTempSensor);
	  
    memset(rs_frame,0,sizeof(rs_frame));
    memset(parities,0,sizeof(parities));
	  
    FILE *uptime_file = fopen("/proc/uptime", "r");
    fscanf(uptime_file, "%f", &uptime_sec);
    uptime = (int) uptime_sec;
    fclose(uptime_file);
    printf("Reset Count: %d Uptime since Reset: %ld \n", reset_count, uptime);
	  
    h[0] = (h[0] & 0xf8) | (id & 0x07);  // 3 bits
//    printf("h[0] %x\n", h[0]);
    h[0] = (h[0] & 0x07)| ((reset_count & 0x1f) << 3);
//    printf("h[0] %x\n", h[0]);
    h[1] = (reset_count >> 5) & 0xff;
//    printf("h[1] %x\n", h[1]);
    h[2] = (h[2] & 0xf8) | ((reset_count >> 13) & 0x07);
//    printf("h[2] %x\n", h[2]);
    h[2] = (h[2] & 0x0e) | ((uptime & 0x1f) << 3);
//    printf("h[2] %x\n", h[2]);
    h[3] = (uptime >> 5) & 0xff;
    h[4] = (uptime >> 13) & 0xff;
    h[5] = (h[5] & 0xf0) | ((uptime >> 21) & 0x0f);
    h[5] = (h[5] & 0x0f) | (frm_type << 4);  
	  
    if (mode == BPSK)
      h[6] = 99;
	  
//  posXv = reading[PLUS_X].current;
  posXi = (int)reading[PLUS_X].current + 2048;
  posYi = (int)reading[PLUS_Y].current + 2048;
  posZi = (int)reading[PLUS_Z].current + 2048;
  negXi = (int)reading[MINUS_X].current + 2048;
  negYi = (int)reading[MINUS_Y].current + 2048;
  negZi = (int)reading[MINUS_Z].current + 2048;

  posXv = (int)(reading[PLUS_X].voltage * 100);
  posYv = (int)(reading[PLUS_Y].voltage* 100);
  posZv = (int)(reading[PLUS_Z].voltage * 100);
  negXv = (int)(reading[MINUS_X].voltage * 100);
  negYv = (int)(reading[MINUS_Y].voltage * 100);
  negZv = (int)(reading[MINUS_Z].voltage * 100);
	  
  batt_c_v = (int)(reading[BAT].voltage * 100);
  battCurr = (int)reading[BAT].current + 2048;
  PSUVoltage = (int)(reading[BUS].voltage * 100);
  PSUCurrent = (int)reading[BUS].current + 2048;
	  
  if (payload == ON)
	  STEMBoardFailure = 0;

  encodeA(b, 0 + head_offset, batt_a_v);
  encodeB(b, 1 + head_offset, batt_b_v);
  encodeA(b, 3 + head_offset, batt_c_v);
  encodeA(b, 9 + head_offset, battCurr);
	  
  if (mode == FSK)
  {	  
  encodeA(b, 12 + head_offset,posXv);  	
  encodeB(b, 13 + head_offset,negXv);	
  encodeA(b, 15 + head_offset,posYv);	
  encodeB(b, 16 + head_offset,negYv);	
  encodeA(b, 18 + head_offset,posZv);	
  encodeB(b, 19 + head_offset,negZv);
	  
  encodeA(b, 21 + head_offset,posXi);  	
  encodeB(b, 22 + head_offset,negXi);	
  encodeA(b, 24 + head_offset,posYi);	
  encodeB(b, 25 + head_offset,negYi);	
  encodeA(b, 27 + head_offset,posZi);	
  encodeB(b, 28 + head_offset,negZi);
  }
  else // BPSK
  {
  encodeA(b, 12 + head_offset,posXv);  	
  encodeB(b, 13 + head_offset,posYv);	
  encodeA(b, 15 + head_offset,posZv);	
  encodeB(b, 16 + head_offset,negXv);	
  encodeA(b, 18 + head_offset,negYv);	
  encodeB(b, 19 + head_offset,negZv);
	  
  encodeA(b, 21 + head_offset,posXi);  	
  encodeB(b, 22 + head_offset,posYi);	
  encodeA(b, 24 + head_offset,posZi);	
  encodeB(b, 25 + head_offset,negXi);	
  encodeA(b, 27 + head_offset,negYi);	
  encodeB(b, 28 + head_offset,negZi);
  }	  
	  
  encodeA(b, 30 + head_offset,PSUVoltage);
  encodeB(b, 46 + head_offset,PSUCurrent);
	  
  encodeA(b, 39 + head_offset,  IHUcpuTemp);
	  
  encodeB(b, 40 + head_offset,  xAngularVelocity);
  encodeA(b, 42 + head_offset,  yAngularVelocity);
  encodeB(b, 43 + head_offset,  zAngularVelocity);
	  
  encodeB(b, 51 + head_offset, STEMBoardFailure);
   
  	short int data10[headerLen + rsFrames * (rsFrameLen + parityLen)];
  	short int data8[headerLen + rsFrames * (rsFrameLen + parityLen)]; 
	  
	int ctr1 = 0;
	int ctr3 = 0;
//	for (i = 0; i < RS_FRAME_LEN; i++) 
	for (i = 0; i < rsFrameLen; i++) 
	{
//        for (int j  = 0; j < RS_FRAMES ; j++)
        for (int j  = 0; j < rsFrames ; j++)
		{
//			if (!((i == (RS_FRAME_LEN - 1)) && (j == 2))) // skip last one for BPSK
			if (!((i == (rsFrameLen - 1)) && (j == 2))) // skip last one for BPSK
			{
//				if (ctr1 < HEADER_LEN)
				if (ctr1 < headerLen)
				{
             				rs_frame[j][i] = h[ctr1];
		     			update_rs(parities[j], h[ctr1]);
      //      				printf("header %d rs_frame[%d][%d] = %x \n", ctr1, j, i, h[ctr1]);
					data8[ctr1++] = rs_frame[j][i];
	//				printf ("data8[%d] = %x \n", ctr1 - 1, rs_frame[j][i]);
				}
				else
				{
//             				rs_frame[j][i] = b[ctr3 % DATA_LEN];
             				rs_frame[j][i] = b[ctr3 % dataLen];
//		     			update_rs(parities[j], b[ctr3 % DATA_LEN]);
		     			update_rs(parities[j], b[ctr3 % dataLen]);
          //  				printf("%d rs_frame[%d][%d] = %x %d \n", 
          //  					ctr1, j, i, b[ctr3 % DATA_LEN], ctr3 % DATA_LEN);
					data8[ctr1++] = rs_frame[j][i];
		//			printf ("data8[%d] = %x \n", ctr1 - 1, rs_frame[j][i]);
					ctr3++;
				}								
			}
		}	
	} 
		
    #ifdef DEBUG_LOGGING	    	    
//	printf("\nAt end of data8 write, %d ctr1 values written\n\n", ctr1);

	  printf("Parities ");
//		for (int m = 0; m < PARITY_LEN; m++) {
		for (int m = 0; m < parityLen; m++) {
		 	printf("%d ", parities[0][m]);
		}
		printf("\n");
     #endif
	  
  	int ctr2 = 0;    
 	memset(data10,0,sizeof(data10));  
	
 
//    for (i = 0; i < DATA_LEN * PAYLOADS + HEADER_LEN; i++) // 476 for BPSK
    for (i = 0; i < dataLen * payloads + headerLen; i++) // 476 for BPSK
	{
				data10[ctr2] = (Encode_8b10b[rd][((int)data8[ctr2])] & 0x3ff);
				nrd = (Encode_8b10b[rd][((int)data8[ctr2])] >> 10) & 1;
		//		printf ("data10[%d] = encoded data8[%d] = %x \n",
		//		 	ctr2, ctr2, data10[ctr2]); 

				rd = nrd; // ^ nrd;
				ctr2++;
	}
//    for (i = 0; i < PARITY_LEN; i++) 
    for (i = 0; i < parityLen; i++) 
	{
//		for (int j  = 0; j < RS_FRAMES; j++)
		for (int j  = 0; j < rsFrames; j++)
		{
			data10[ctr2++] = (Encode_8b10b[rd][((int)parities[j][i])] & 0x3ff);
			nrd = (Encode_8b10b[rd][((int)parities[j][i])] >> 10) & 1;
		//	printf ("data10[%d] = encoded parities[%d][%d] = %x \n",
		//		 ctr2 - 1, j, i, data10[ctr2 - 1]); 

			rd = nrd; 
		}	
	}
    #ifdef DEBUG_LOGGING	    	    
// 	printf("\nAt end of data10 write, %d ctr2 values written\n\n", ctr2);
    #endif
	  
    int data;
    int val;
    int offset = 0;
	  
    #ifdef DEBUG_LOGGING	    	    
//	printf("\nAt start of buffer loop, syncBits %d samples %d ctr %d\n", syncBits, samples, ctr);
    #endif
	  
// 	for (i = 1; i <= SYNC_BITS * SAMPLES; i++)
 	for (i = 1; i <= syncBits * samples; i++)
	{
		write_wave(ctr, buffer);	
//		printf("%d ",ctr);
//		if ( (i % SAMPLES) == 0) {
		if ( (i % samples) == 0) {
//  			int bit = SYNC_BITS - i/SAMPLES + 1;
  			int bit = syncBits - i/samples + 1;
  			val = sync;
			data = val & 1 << (bit - 1);	
		 //   	printf ("%d i: %d new frame %d sync bit %d = %d \n",
		 //  		 ctr/SAMPLES, i, frames, bit, (data > 0) );
			if (mode == FSK)
			{
				phase = ((data != 0) * 2) - 1; 
		//		printf("Sending a %d\n", phase);
			}
			else
			{
				if (data == 0)  {
					phase *= -1;
					if ( (ctr - smaller) > 0)
					{
						for (int j = 1; j <= smaller; j++)
				     		buffer[ctr - j] = buffer[ctr - j] * 0.4;
					}
					flip_ctr = ctr;
				}
			}
		}
	}
    #ifdef DEBUG_LOGGING	    	    
//	printf("\n\nValue of ctr after header: %d Buffer Len: %d\n\n", ctr, buffSize);
    #endif
	  for (i = 1; 
//	  i <= (10 * (HEADER_LEN + DATA_LEN * PAYLOADS + RS_FRAMES * PARITY_LEN) * SAMPLES); i++) // 572   
	  i <= (10 * (headerLen + dataLen * payloads + rsFrames * parityLen) * samples); i++) // 572   
	{
		write_wave(ctr, buffer);
//		if ( (i % SAMPLES) == 0) {
		if ( (i % samples) == 0) {
//			int symbol = (int)((i - 1)/ (SAMPLES * 10));
//			int bit = 10 - (i - symbol * SAMPLES * 10) / SAMPLES + 1;	
			int symbol = (int)((i - 1)/ (samples * 10));
			int bit = 10 - (i - symbol * samples * 10) / samples + 1;	
			val = data10[symbol];
			data = val & 1 << (bit - 1);	
	//		printf ("%d i: %d new frame %d data10[%d] = %x bit %d = %d \n",
	//	    		 ctr/SAMPLES, i, frames, symbol, val, bit, (data > 0) );
		    if (mode == FSK)
			{
				phase = ((data != 0) * 2) - 1; 
	//			printf("Sending a %d\n", phase);
			}
			else
			{	 
				if (data == 0)  {
					phase *= -1;
					if ( (ctr - smaller) > 0)
					{
						for (int j = 1; j <= smaller; j ++)
				    	 	buffer[ctr - j] = buffer[ctr - j] * 0.4;
					}
					flip_ctr = ctr;
				}
			}	
		}
	 }   
	}
    #ifdef DEBUG_LOGGING	    	    
//	printf("\nValue of ctr after looping: %d Buffer Len: %d\n", ctr, buffSize);
//	printf("\ctr/samples = %d ctr/(samples*10) = %d\n\n", ctr/samples, ctr/(samples*10));
    #endif	

  int error = 0;
  int count;
//  for (count = 0; count < DATA_LEN; count++) {
//  for (count = 0; count < dataLen; count++) {
//      printf("%02X", b[count]);
//  }
//  printf("\n");
		
// rpitx
/*	
      char cmdbuffer[1000];
      FILE* txResult;
      if ((rpitxStatus != mode)) //  || ((loop % 1000) == 0))
	      
      {  // change rpitx mode
	  rpitxStatus = mode;    
	  printf("Changing rpitx mode!\n");
//     	  txResult = popen("ps -ef | grep rpitx | grep -v grep | awk '{print $2}' | sudo xargs kill -9 > /dev/null 2>&1", "r"); 
      	  txResult = popen("sudo killall -9 rpitx > /dev/null 2>&1", "r"); 
	  pclose(txResult);   
//	  printf("1\n");
//          sleep(1);
//     	  txResult = popen("ps -ef | grep sendiq | grep -v grep | awk '{print $2}' | sudo xargs kill -9 > /dev/null 2>&1", "r"); 
      	  txResult = popen("sudo killall -9 sendiq > /dev/null 2>&1", "r");
	  pclose(txResult);
//	  printf("2\n");
//  digitalWrite (txLed, txLedOn);
	      sleep(1);
	  txResult = popen("sudo fuser -k 8080/tcp > /dev/null 2>&1", "r");
	  pclose(txResult);
	  socket_open = 0;
	      
//	  printf("3\n");
          sleep(1);
//  digitalWrite (txLed, txLedOff);

	  if (transmit)
	  {
	    if (mode == FSK)  {  
      	 // 	txResult = popen("sudo nc -l 8080 | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.896e3&", "r"); 
      	  	txResult = popen("sudo nc -l 8080 | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | while true; do sudo timeout -k 1 60m /home/pi/rpitx/rpitx -i- -m RF -f 434.897e3; done &", "r"); 
  		pclose(txResult);
	//	  	printf("4\n");
           } else if (mode == BPSK) {
//      	  	txResult = popen("sudo nc -l 8080 | csdr convert_i16_f | csdr fir_interpolate_cc 2 | csdr dsb_fc | csdr bandpass_fir_fft_cc 0.002 0.06 0.01 | csdr fastagc_ff | sudo /home/pi/CubeSatSim/rpitx/sendiq -i /dev/stdin -s 96000 -f 434.8925e6 -t float 2>&1&", "r"); 
      	  	txResult = popen("sudo nc -l 8080 | csdr convert_i16_f | csdr fir_interpolate_cc 2 | csdr dsb_fc | csdr bandpass_fir_fft_cc 0.002 0.06 0.01 | csdr fastagc_ff | while true; do sudo timeout -k 1 60m /home/pi/rpitx/sendiq -i /dev/stdin -s 96000 -f 434.8945e6 -t float 2>&1; done &", "r"); 
           	pclose(txResult);           }
//      fgets(cmdbuffer, 1000, txResult);

	  }
	  else
          {
	     fprintf(stderr,"\nNo CubeSatSim Band Pass Filter detected.  No transmissions after the CW ID.\n");
	     fprintf(stderr, " See http://cubesatsim.org/wiki for info about building a CubeSatSim\n\n");
          }
      sleep(2);
//      printf("Results of transmit command: %s\n", cmdbuffer);
      }
*/
	
// socket write

  if (!socket_open && transmit)
    {
    printf("Opening socket!\n");
    struct sockaddr_in address; 
    int valread; 
    struct sockaddr_in serv_addr; 
//    char *hello = "Hello from client"; 
//    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        error = 1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        error = 1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
	printf("Error: %s \n", strerror(errno));
        error = 1; 
    }
    if (error == 1)
	; //rpitxStatus = -1;
    else
	 socket_open = 1;
    }
	
    if (!error && transmit)
    {
//	digitalWrite (0, LOW);
	printf("Sending %d buffer bytes over socket after %d ms!\n", ctr, millis()-start);
	start = millis();
//	int sock_ret = send(sock, buffer, buffSize, 0);
	int sock_ret = send(sock, buffer, ctr * 2 + 2, 0);
	printf("Millis5: %d Result of socket send: %d \n", millis() - start, sock_ret);

 	if (sock_ret < (ctr * 2 + 2))
	{
       		printf("Resending\n");
//	 	sock_ret = send(sock, buffer[sock_ret], ctr * 2 + 2 - sock_ret, 0);
       		printf("Millis10: %d Result of socket send: %d \n", millis() - start, sock_ret);
	}

	if (sock_ret == -1)  {
		printf("Error: %s \n", strerror(errno));
		socket_open = 0;
		//rpitxStatus = -1;
	}
    }
    if (!transmit)
    {
	fprintf(stderr,"\nNo CubeSatSim Band Pass Filter detected.  No transmissions after the CW ID.\n");
	fprintf(stderr, " See http://cubesatsim.org/wiki for info about building a CubeSatSim\n\n");
    }
//    digitalWrite (0, HIGH);
	
    if (mode == FSK)
	    firstTime = 0;
    else if (frames_sent > 0) //5)
	    firstTime = 0;

return 0;	
}
 
 /*
 * TelemEncoding.h
 *
 *  Created on: Feb 3, 2014
 *      Author: fox
 */
 
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define false 0
#define true 1

//static int twosToInt(int val,int len);
//static int encodeB(short int  *b, int index, int val);
//static int encodeA(short int  *b, int index, int val);

	 static  int NOT_FRAME = /* 0fa */ 0xfa & 0x3ff;
	 static  int FRAME = /* 0fa */ ~0xfa & 0x3ff;
	
/*
 * TelemEncoding.c
 *
   Fox-1 telemetry encoder
   January 2014 Phil Karn KA9Q

   This file has two external functions:
      void update_rs(unsigned char parity[32],unsigned char data);
      int encode_8b10b(int *state,int data).

   update_rs() is the Reed-Solomon encoder. Its first argument is the 32-byte
   encoder shift register, the second is the 8-bit data byte being encoded. It updates
   the shift register in place and returns void. At the end of each frame, it contains
   the parities ready for transmission, starting with parity[0].
   Be sure to zero this array before each new frame!

   encode_8b10b() is the 8b10b encoder. Its first argument is a pointer to a single integer
   with the 1-bit encoder state (the current run disparity, or RD). Initialize it to 0
   JUST ONCE at startup (not between frames).
   The second argument is the data byte being encoded. It updates the state and returns
   an integer containing the 10-bit encoded word, right justified.
   Transmit this word from left to right.

   The data argument is an int so it can hold the special value -1 to indicate end of frame;
   it generates the 8b10b control word K.28.5, which is used as an inter-frame flag.

   Some assert() calls are made to verify legality of arguments. These can be turned off in
   production code.


   sample frame transmission code:

   unsigned char data[64]; // Data block to be sent
   unsigned char parity[32]; // RS parities
   void transmit_word(int);  // User provided transmit function: 10 bits of data in bits 9....0
   int state,i;

   state = 0; // Only once at startup, not between frames
   memset(parity,0,sizeof(parity); // Do this before every frame
   // Transmit the data, updating the RS encoder
   for(i=0;i<64;i++){
     update_rs(parity,data[i]);
     transmit_word(encode_8b10b(&state,data[i]);
   }
   // get the RS parities
   for(i=0;i<32;i++)
     transmit_word(encode_8b10b(&state,parity[i]);

   transmit_word(encode_8b10b(&state,-1); // Transmit end-of-frame flag
*/


#include <string.h>
//#include "Fox.h"
//#include "TelemEncoding.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define NN (0xff) // Frame size in symbols
#define A0 (NN)   // special value for log(0)


// GF Antilog lookup table table
static unsigned char CCSDS_alpha_to[NN+1] = {
0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x87,0x89,0x95,0xad,0xdd,0x3d,0x7a,0xf4,
0x6f,0xde,0x3b,0x76,0xec,0x5f,0xbe,0xfb,0x71,0xe2,0x43,0x86,0x8b,0x91,0xa5,0xcd,
0x1d,0x3a,0x74,0xe8,0x57,0xae,0xdb,0x31,0x62,0xc4,0x0f,0x1e,0x3c,0x78,0xf0,0x67,
0xce,0x1b,0x36,0x6c,0xd8,0x37,0x6e,0xdc,0x3f,0x7e,0xfc,0x7f,0xfe,0x7b,0xf6,0x6b,
0xd6,0x2b,0x56,0xac,0xdf,0x39,0x72,0xe4,0x4f,0x9e,0xbb,0xf1,0x65,0xca,0x13,0x26,
0x4c,0x98,0xb7,0xe9,0x55,0xaa,0xd3,0x21,0x42,0x84,0x8f,0x99,0xb5,0xed,0x5d,0xba,
0xf3,0x61,0xc2,0x03,0x06,0x0c,0x18,0x30,0x60,0xc0,0x07,0x0e,0x1c,0x38,0x70,0xe0,
0x47,0x8e,0x9b,0xb1,0xe5,0x4d,0x9a,0xb3,0xe1,0x45,0x8a,0x93,0xa1,0xc5,0x0d,0x1a,
0x34,0x68,0xd0,0x27,0x4e,0x9c,0xbf,0xf9,0x75,0xea,0x53,0xa6,0xcb,0x11,0x22,0x44,
0x88,0x97,0xa9,0xd5,0x2d,0x5a,0xb4,0xef,0x59,0xb2,0xe3,0x41,0x82,0x83,0x81,0x85,
0x8d,0x9d,0xbd,0xfd,0x7d,0xfa,0x73,0xe6,0x4b,0x96,0xab,0xd1,0x25,0x4a,0x94,0xaf,
0xd9,0x35,0x6a,0xd4,0x2f,0x5e,0xbc,0xff,0x79,0xf2,0x63,0xc6,0x0b,0x16,0x2c,0x58,
0xb0,0xe7,0x49,0x92,0xa3,0xc1,0x05,0x0a,0x14,0x28,0x50,0xa0,0xc7,0x09,0x12,0x24,
0x48,0x90,0xa7,0xc9,0x15,0x2a,0x54,0xa8,0xd7,0x29,0x52,0xa4,0xcf,0x19,0x32,0x64,
0xc8,0x17,0x2e,0x5c,0xb8,0xf7,0x69,0xd2,0x23,0x46,0x8c,0x9f,0xb9,0xf5,0x6d,0xda,
0x33,0x66,0xcc,0x1f,0x3e,0x7c,0xf8,0x77,0xee,0x5b,0xb6,0xeb,0x51,0xa2,0xc3,0x00,
};

// GF log lookup table. Special value represents log(0)
static unsigned char CCSDS_index_of[NN+1] = {
 A0,  0,  1, 99,  2,198,100,106,  3,205,199,188,101,126,107, 42,
  4,141,206, 78,200,212,189,225,102,221,127, 49,108, 32, 43,243,
  5, 87,142,232,207,172, 79,131,201,217,213, 65,190,148,226,180,
103, 39,222,240,128,177, 50, 53,109, 69, 33, 18, 44, 13,244, 56,
  6,155, 88, 26,143,121,233,112,208,194,173,168, 80,117,132, 72,
202,252,218,138,214, 84, 66, 36,191,152,149,249,227, 94,181, 21,
104, 97, 40,186,223, 76,241, 47,129,230,178, 63, 51,238, 54, 16,
110, 24, 70,166, 34,136, 19,247, 45,184, 14, 61,245,164, 57, 59,
  7,158,156,157, 89,159, 27,  8,144,  9,122, 28,234,160,113, 90,
209, 29,195,123,174, 10,169,145, 81, 91,118,114,133,161, 73,235,
203,124,253,196,219, 30,139,210,215,146, 85,170, 67, 11, 37,175,
192,115,153,119,150, 92,250, 82,228,236, 95, 74,182,162, 22,134,
105,197, 98,254, 41,125,187,204,224,211, 77,140,242, 31, 48,220,
130,171,231, 86,179,147, 64,216, 52,176,239, 38, 55, 12, 17, 68,
111,120, 25,154, 71,116,167,193, 35, 83,137,251, 20, 93,248,151,
 46, 75,185, 96, 15,237, 62,229,246,135,165, 23, 58,163, 60,183,
};

// Only half the coefficients are given here because the
// generator polynomial is palindromic; G0 = G32, G1 = G31, etc.
// Only G16 is unique
static unsigned char CCSDS_poly[] = {
  0,249,  59, 66,  4,  43,126,251, 97,  30,   3,213, 50, 66,170,   5,
  24,
};

static inline int modnn(int x){
  while (x >= NN) {
    x -= NN;
    x = (x >> 8) + (x & NN);
  }
  return x;
}

// Update Reed-Solomon encoder
// parity -> 32-byte reed-solomon encoder state; clear this to zero before each frame
void update_rs(
   unsigned char parity[32], // 32-byte encoder state; zero before each frame
   unsigned char c)          // Current data byte to update
{
  unsigned char feedback;
  int j,t;

  assert(parity != NULL);
  feedback = CCSDS_index_of[c ^ parity[0]];
  if(feedback != A0){ // only if feedback is non-zero
    // Take advantage of palindromic polynomial to halve the multiplies
    // Do G1...G15, which is the same as G17...G31
    for(j=1;j<NP/2;j++){
      t = CCSDS_alpha_to[modnn(feedback + CCSDS_poly[j])];
      parity[j] ^= t;
      parity[NP-j] ^= t;
    }
    // Do G16, which is used in only parity[16]
    t = CCSDS_alpha_to[modnn(feedback + CCSDS_poly[j])];
    parity[j] ^= t;
  }
  // shift left
  memmove(&parity[0],&parity[1],NP-1);
  // G0 is 1 in alpha form, 0 in index form; don't need to multiply by it
  parity[NP-1] = CCSDS_alpha_to[feedback];
  //taskYIELD();
}

#define SYNC  (0x0fa) // K.28.5, RD=-1 
 
void write_little_endian(unsigned int word, int num_bytes, FILE *wav_file)
{
	unsigned buf;
	while(num_bytes>0)
	{   buf = word & 0xff;
		fwrite(&buf, 1,1, wav_file);
		num_bytes--;
	word >>= 8;
	}
}


void write_wave(int i, short int *buffer)
{
		if (mode == FSK)
		{
			if ((ctr - flip_ctr) < smaller)
				buffer[ctr++] = 0.1 * phase * (ctr - flip_ctr) / smaller;
			else
				buffer[ctr++] = 0.25 * amplitude * phase;
		}
		else
		{
			if ((ctr - flip_ctr) < smaller)
  		 		buffer[ctr++] = (int)(amplitude * 0.4 * phase * sin((float)(2*M_PI*i*freq_Hz/S_RATE))); 					
 			else
 		 		buffer[ctr++] = (int)(amplitude * phase * sin((float)(2*M_PI*i*freq_Hz/S_RATE)));
 		 } 			
//		printf("%d %d \n", i, buffer[ctr - 1]);

}

int encodeA(short int  *b, int index, int val) {
//    printf("Encoding A\n");
    b[index] = val & 0xff;
    b[index + 1] = (b[index + 1] & 0xf0) | ((val >> 8) & 0x0f);
    return 0;	
}

int encodeB(short int  *b, int index, int val) {
//    printf("Encoding B\n");
    b[index] = (b[index] & 0x0f)  |  ((val << 4) & 0xf0);
    b[index + 1] = (val >> 4 ) & 0xff;
    return 0;	
}

int twosToInt(int val,int len) {   // Convert twos compliment to integer
// from https://www.raspberrypi.org/forums/viewtopic.php?t=55815
	
      if(val & (1 << (len - 1)))
         val = val - (1 << len);

      return(val);
}
