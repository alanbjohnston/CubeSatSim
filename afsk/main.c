/*
 *  Transmits CubeSat Telemetry at 434.9MHz in AFSK, FSK, or BPSK format
 *
 *  Copyright Alan B. Johnston
 *
 *  Portions Copyright (C) 2018 Jonathan Brandenburg
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
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
#include <wiringSerial.h>
#include <time.h>
#include <math.h>
#include "TelemEncoding.h"
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
#define BAT 2
#define BUS 3
#define MINUS_X 4
#define MINUS_Y 5
#define PLUS_Z 6
#define MINUS_Z 7
#define TEMP 2
#define PRES 3
#define ALT 4
#define HUMI 5
#define GYRO_X 7
#define GYRO_Y 8
#define GYRO_Z 9
#define ACCEL_X 10
#define ACCEL_Y 11
#define ACCEL_Z 12
#define XS1 14
#define XS2 15
#define XS3 16

#define RSSI 0
#define IHU_TEMP 2
#define SPIN 1

#define OFF - 1
#define ON 1

uint32_t tx_freq_hz = 434900000 + FREQUENCY_OFFSET;
uint8_t data[1024];
uint32_t tx_channel = 0;

ax5043_conf_t hax5043;
ax25_conf_t hax25;

int twosToInt(int val, int len);
float rnd_float(double min, double max);
void get_tlm();
void get_tlm_fox();
int encodeA(short int * b, int index, int val);
int encodeB(short int * b, int index, int val);
void config_x25();
void trans_x25();
int upper_digit(int number);
int lower_digit(int number);
static int init_rf();
int socket_open = 0;
int sock = 0;
int loop = -1, loop_count = 0;
int firstTime = ON; // 0;
long start;
int testCount = 0;
long time_start;
char cmdbuffer[1000];
FILE * file1;
short int buffer[2336400]; // max size for 10 frames count of BPSK
FILE *sopen(const char *program);

#define S_RATE	(48000) // (44100)

#define AFSK 1
#define FSK 2
#define BPSK 3
#define CW 4

int rpitxStatus = -1;

float amplitude; // = ; // 20000; // 32767/(10%amp+5%amp+100%amp)
float freq_Hz = 3000; // 1200
short int sin_samples;
short int sin_map[16];

int smaller;
int flip_ctr = 0;
int phase = 1;
int ctr = 0;
int rd = 0;
int nrd;
void write_to_buffer(int i, int symbol, int val);
void write_wave(int i, short int * buffer);
int uart_fd;

int reset_count;
float uptime_sec;
long int uptime;
char call[5];

int bitRate, mode, bufLen, rsFrames, payloads, rsFrameLen, dataLen, headerLen, syncBits, syncWord, parityLen, samples, frameCnt, samplePeriod;
float sleepTime;
unsigned int sampleTime = 0;
int frames_sent = 0;
int cw_id = ON;
int vB4 = FALSE, vB5 = FALSE, vB3 = FALSE, ax5043 = FALSE, transmit = FALSE, onLed, onLedOn, onLedOff, txLed, txLedOn, txLedOff, payload = OFF;
float batteryThreshold = 3.0, batteryVoltage;
float latitude = 39.027702f, longitude = -77.078064f;
float lat_file, long_file;

float axis[3], angle[3], volts_max[3], amps_max[3], batt, speed, period, tempS, temp_max, temp_min, eclipse;
int i2c_bus0 = OFF, i2c_bus1 = OFF, i2c_bus3 = OFF, camera = OFF, sim_mode = FALSE, rxAntennaDeployed = 0, txAntennaDeployed = 0;
double eclipse_time;

int test_i2c_bus(int bus);

const char pythonCmd[] = "python3 /home/pi/CubeSatSim/python/voltcurrent.py ";
char pythonStr[100], pythonConfigStr[100], busStr[10];
int map[8] = {0, 1, 2, 3, 4, 5, 6, 7};
char src_addr[5] = "";
char dest_addr[5] = "CQ";
float voltage_min[9], current_min[9], voltage_max[9], current_max[9], sensor_max[17], sensor_min[17], other_max[3], other_min[3];

int main(int argc, char * argv[]) {

  mode = FSK;
  frameCnt = 1;

  if (argc > 1) {
    //	  strcpy(src_addr, argv[1]);
    if ( * argv[1] == 'b') {
      mode = BPSK;
      printf("Mode BPSK\n");
    } else if ( * argv[1] == 'a') {
      mode = AFSK;
      printf("Mode AFSK\n");
    } else if ( * argv[1] == 'c') {
      mode = CW;
      printf("Mode CW\n");
    } else {
      printf("Mode FSK\n");
    }

    if (argc > 2) {
      //		  printf("String is %s %s\n", *argv[2], argv[2]);
      loop = atoi(argv[2]);
      loop_count = loop;
    }
    printf("Looping %d times \n", loop);

    if (argc > 3) {
      if ( * argv[3] == 'n') {
        cw_id = OFF;
        printf("No CW id\n");
      }
    }
  }

  // Open configuration file with callsign and reset count	
  FILE * config_file = fopen("/home/pi/CubeSatSim/sim.cfg", "r");
  if (config_file == NULL) {
    printf("Creating config file.");
    config_file = fopen("/home/pi/CubeSatSim/sim.cfg", "w");
    fprintf(config_file, "%s %d", " ", 100);
    fclose(config_file);
    config_file = fopen("/home/pi/CubeSatSim/sim.cfg", "r");
  }

//  char * cfg_buf[100];
  fscanf(config_file, "%s %d %f %f", call, & reset_count, & lat_file, & long_file);
  fclose(config_file);
  printf("Config file /home/pi/CubeSatSim/sim.cfg contains %s %d %f %f\n", call, reset_count, lat_file, long_file);
  reset_count = (reset_count + 1) % 0xffff;

  if ((fabs(lat_file) > 0) && (fabs(lat_file) < 90.0) && (fabs(long_file) > 0) && (fabs(long_file) < 180.0)) {
    printf("Valid latitude and longitude in config file\n");
    latitude = lat_file;
    longitude = long_file;
  }
  wiringPiSetup();

  // Check for SPI and AX-5043 Digital Transceiver Board	
  FILE * file = popen("sudo raspi-config nonint get_spi", "r");
  if (fgetc(file) == 48) {
    printf("SPI is enabled!\n");

    FILE * file2 = popen("ls /dev/spidev0.* 2>&1", "r");
    //          printf("Result getc: %c \n", getc(file2));

    if (fgetc(file2) != 'l') {
      printf("SPI devices present!\n");
      //	  }
      pclose(file2);
      setSpiChannel(SPI_CHANNEL);
      setSpiSpeed(SPI_SPEED);
      initializeSpi();
      //	  char src_addr[5] = "KU2Y";
      //          char dest_addr[5] = "CQ";
      ax25_init( & hax25, (uint8_t * ) dest_addr, '1', (uint8_t * ) call, '1', AX25_PREAMBLE_LEN, AX25_POSTAMBLE_LEN);
      if (init_rf()) {
        printf("AX5043 successfully initialized!\n");
        ax5043 = TRUE;
        cw_id = OFF;
        mode = AFSK;
        //		cycle = OFF;
        printf("Mode AFSK with AX5043\n");
        transmit = TRUE;
      } else
        printf("AX5043 not present!\n");
    }
  }
  //       else
  //       {
  //	  printf("SPI not enabled!\n");
  //       }
  pclose(file);
  txLed = 0; // defaults for vB3 board without TFB
  txLedOn = LOW;
  txLedOff = HIGH;
  if (!ax5043) {
    pinMode(2, INPUT);
    pullUpDnControl(2, PUD_UP);

    if (digitalRead(2) != HIGH) {
      printf("vB3 with TFB Present\n");
      vB3 = TRUE;
      txLed = 3;
      txLedOn = LOW;
      txLedOff = HIGH;
      onLed = 0;
      onLedOn = LOW;
      onLedOff = HIGH;
      transmit = TRUE;
    } else {
      pinMode(3, INPUT);
      pullUpDnControl(3, PUD_UP);

      if (digitalRead(3) != HIGH) {
        printf("vB4 Present\n");
        txLed = 2;
        txLedOn = HIGH;
        txLedOff = LOW;
        vB4 = TRUE;
        onLed = 0;
        onLedOn = HIGH;
        onLedOff = LOW;
        transmit = TRUE;
      } else {
        pinMode(26, INPUT);
        pullUpDnControl(26, PUD_UP);

        if (digitalRead(26) != HIGH) {
          printf("vB5 Present\n");
          txLed = 2;
          txLedOn = HIGH;
          txLedOff = LOW;
          vB5 = TRUE;
          onLed = 27;
          onLedOn = HIGH;
          onLedOff = LOW;
          transmit = TRUE;
        }
      }
    }
  }
  pinMode(txLed, OUTPUT);
  digitalWrite(txLed, txLedOff);
  #ifdef DEBUG_LOGGING
  printf("Tx LED Off\n");
  #endif
  pinMode(onLed, OUTPUT);
  digitalWrite(onLed, onLedOn);
  #ifdef DEBUG_LOGGING
  printf("Power LED On\n");
  #endif

  config_file = fopen("sim.cfg", "w");
  fprintf(config_file, "%s %d %8.4f %8.4f", call, reset_count, lat_file, long_file);
  //    fprintf(config_file, "%s %d", call, reset_count);
  fclose(config_file);
  config_file = fopen("sim.cfg", "r");

  if (vB4) {
    map[BAT] = BUS;
    map[BUS] = BAT;
    snprintf(busStr, 10, "%d %d", test_i2c_bus(1), test_i2c_bus(0));
  } else if (vB5) {
    map[MINUS_X] = PLUS_Z;
    map[PLUS_Z] = MINUS_X;

    if (access("/dev/i2c-11", W_OK | R_OK) >= 0) { // Test if I2C Bus 11 is present			
      printf("/dev/i2c-11 is present\n\n");
      snprintf(busStr, 10, "%d %d", test_i2c_bus(1), test_i2c_bus(11));
    } else {
      snprintf(busStr, 10, "%d %d", test_i2c_bus(1), test_i2c_bus(3));
    }
  } else {
    map[BUS] = MINUS_Z;
    map[BAT] = BUS;
    map[PLUS_Z] = BAT;
    map[MINUS_Z] = PLUS_Z;
    snprintf(busStr, 10, "%d %d", test_i2c_bus(1), test_i2c_bus(0));
    batteryThreshold = 8.0;
  }

  strcpy(pythonStr, pythonCmd);
  strcat(pythonStr, busStr);
  strcat(pythonConfigStr, pythonStr);
  strcat(pythonConfigStr, " c");  

  fprintf(stderr, "pythonConfigStr: %s\n", pythonConfigStr);
	
  file1 = sopen(pythonConfigStr);  // try new function
  fgets(cmdbuffer, 1000, file1);
  fprintf(stderr, "pythonStr result: %s\n", cmdbuffer);
/*	
  sleep(5);
  fputc('\n', file1);
  fgets(cmdbuffer, 1000, file1);
  printf("pythonStr result2: %s\n", cmdbuffer);	
	
  file1 = popen(pythonConfigStr, "w");

  fgets(cmdbuffer, 1000, file1);
  printf("pythonStr result: %s\n", cmdbuffer);
  fgets(cmdbuffer, 1000, file1);
  printf("pythonStr resulta: %s\n", cmdbuffer);
  fgets(cmdbuffer, 1000, file1);
  printf("pythonStr resultb: %s\n", cmdbuffer);	
//  pclose(file1);  

  sleep(5);
  fputc('\n', file1);
  fgets(cmdbuffer, 1000, file1);
  printf("pythonStr result2: %s\n", cmdbuffer);	
  fgets(cmdbuffer, 1000, file1);
  printf("pythonStr result2a: %s\n", cmdbuffer);	

  sleep(5);
  fputc('\n', file1);
  fgets(cmdbuffer, 1000, file1);
  printf("pythonStr result2: %s\n", cmdbuffer);		
*/
  // try connecting to Arduino payload using UART

  if (!ax5043 && !vB3) // don't test if AX5043 is present
  {
    payload = OFF;

    if ((uart_fd = serialOpen("/dev/ttyAMA0", 115200)) >= 0) {  // was 9600
      char c;
      int charss = (char) serialDataAvail(uart_fd);
      if (charss != 0)
        printf("Clearing buffer of %d chars \n", charss);
      while ((charss--> 0))
        c = (char) serialGetchar(uart_fd); // clear buffer

      unsigned int waitTime;
      int i;
      for (i = 0; i < 2; i++) {
        serialPutchar(uart_fd, 'R');
        printf("Querying payload with R to reset\n");
        waitTime = millis() + 500;
        while ((millis() < waitTime) && (payload != ON)) {
          if (serialDataAvail(uart_fd)) {
            printf("%c", c = (char) serialGetchar(uart_fd));
            fflush(stdout);
            if (c == 'O') {
              printf("%c", c = (char) serialGetchar(uart_fd));
              fflush(stdout);
              if (c == 'K')
                payload = ON;
            }
          }
          //        sleep(0.75);
        }
      }
      if (payload == ON)
        printf("\nPayload is present!\n");
      else
        printf("\nPayload not present!\n");
    } else {
      fprintf(stderr, "Unable to open UART: %s\n", strerror(errno));
    }
  }

  // test i2c buses	
  i2c_bus0 = (test_i2c_bus(0) != -1) ? ON : OFF;
  i2c_bus1 = (test_i2c_bus(1) != -1) ? ON : OFF;
  i2c_bus3 = (test_i2c_bus(3) != -1) ? ON : OFF;

  // check for camera	
//  char cmdbuffer1[1000];
  FILE * file4 = popen("vcgencmd get_camera", "r");
  fgets(cmdbuffer, 1000, file4);
  char camera_present[] = "supported=1 detected=1";
  // printf("strstr: %s \n", strstr( & cmdbuffer1, camera_present));
  camera = (strstr( (const char *)& cmdbuffer, camera_present) != NULL) ? ON : OFF;
  //  printf("Camera result:%s camera: %d \n", & cmdbuffer1, camera);
  pclose(file4);

  #ifdef DEBUG_LOGGING
  printf("INFO: I2C bus status 0: %d 1: %d 3: %d camera: %d\n", i2c_bus0, i2c_bus1, i2c_bus3, camera);
  #endif

  if ((i2c_bus1 == OFF) && (i2c_bus3 == OFF)) {

    sim_mode = TRUE;

    printf("Simulated telemetry mode!\n");

    srand((unsigned int)time(0));

    axis[0] = rnd_float(-0.2, 0.2);
    if (axis[0] == 0)
      axis[0] = rnd_float(-0.2, 0.2);
    axis[1] = rnd_float(-0.2, 0.2);
    axis[2] = (rnd_float(-0.2, 0.2) > 0) ? 1.0 : -1.0;

    angle[0] = (float) atan(axis[1] / axis[2]);
    angle[1] = (float) atan(axis[2] / axis[0]);
    angle[2] = (float) atan(axis[1] / axis[0]);

    volts_max[0] = rnd_float(4.5, 5.5) * (float) sin(angle[1]);
    volts_max[1] = rnd_float(4.5, 5.5) * (float) cos(angle[0]);
    volts_max[2] = rnd_float(4.5, 5.5) * (float) cos(angle[1] - angle[0]);

    float amps_avg = rnd_float(150, 300);

    amps_max[0] = (amps_avg + rnd_float(-25.0, 25.0)) * (float) sin(angle[1]);
    amps_max[1] = (amps_avg + rnd_float(-25.0, 25.0)) * (float) cos(angle[0]);
    amps_max[2] = (amps_avg + rnd_float(-25.0, 25.0)) * (float) cos(angle[1] - angle[0]);

    batt = rnd_float(3.8, 4.3);
    speed = rnd_float(1.0, 2.5);
    eclipse = (rnd_float(-1, +4) > 0) ? 1.0 : 0.0;
    period = rnd_float(150, 300);
    tempS = rnd_float(20, 55);
    temp_max = rnd_float(50, 70);
    temp_min = rnd_float(10, 20);

    #ifdef DEBUG_LOGGING
    for (int i = 0; i < 3; i++)
      printf("axis: %f angle: %f v: %f i: %f \n", axis[i], angle[i], volts_max[i], amps_max[i]);
    printf("batt: %f speed: %f eclipse_time: %f eclipse: %f period: %f temp: %f max: %f min: %f\n", batt, speed, eclipse_time, eclipse, period, tempS, temp_max, temp_min);
    #endif

    time_start = (long int) millis();

    eclipse_time = (long int)(millis() / 1000.0);
    if (eclipse == 0.0)
      eclipse_time -= period / 2; // if starting in eclipse, shorten interval	
  }

  //int ret;
  //uint8_t data[1024];

  tx_freq_hz -= tx_channel * 50000;

  if (mode == AFSK)
    sleep(10); // delay awaiting CW ID completion

  if (transmit == FALSE) {

    fprintf(stderr, "\nNo CubeSatSim Band Pass Filter detected.  No transmissions after the CW ID.\n");
    fprintf(stderr, " See http://cubesatsim.org/wiki for info about building a CubeSatSim\n\n");
  }

  for (int i = 0; i < 9; i++) {
    voltage_min[i] = 1000.0;
    current_min[i] = 1000.0;
    voltage_max[i] = -1000.0;
    current_max[i] = -1000.0;
  }
  for (int i = 0; i < 17; i++) {
    sensor_min[i] = 1000.0;
    sensor_max[i] = -1000.0;
 //   printf("Sensor min and max initialized!");
  }
  for (int i = 0; i < 3; i++) {
    other_min[i] = 1000.0;
    other_max[i] = -1000.0;
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
      amplitude = 32767 / 3;
      samples = S_RATE / bitRate;
      bufLen = (frameCnt * (syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))) * samples);

      samplePeriod =  (int) (((float)((syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen)))) / (float) bitRate) * 1000 - 500);
      sleepTime = 0.1f;

      printf("\n FSK Mode, %d bits per frame, %d bits per second, %d ms sample period\n",
        bufLen / (samples * frameCnt), bitRate, samplePeriod);
    } else if (mode == BPSK) {
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
      samples = S_RATE / bitRate;
      bufLen = (frameCnt * (syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))) * samples);

      //   samplePeriod = ((float)((syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))))/(float)bitRate) * 1000 - 1800;
      //    samplePeriod = 3000;
      //    sleepTime = 3.0;
      samplePeriod = 2200; // reduce dut to python and sensor querying delays
      sleepTime = 2.2f;

      printf("\n BPSK Mode, bufLen: %d,  %d bits per frame, %d bits per second, %d seconds per frame %d ms sample period\n",
        bufLen, bufLen / (samples * frameCnt), bitRate, bufLen / (samples * frameCnt * bitRate), samplePeriod);
	   
      sin_samples = S_RATE/freq_Hz;	 		
      printf("Sin map: ");	 		
      for (int j = 0; j < sin_samples; j++) {	 		
        sin_map[j] = (short int)(amplitude * sin((float)(2 * M_PI * j / sin_samples)));	 		
	printf(" %d", sin_map[j]);	 		
      }	 		
      printf("\n");
   }

  long int loopTime;
  loopTime = millis();	
	
  while (loop-- != 0) {
//    frames_sent++;

    printf("++++ Loop time: %d +++++\n", millis() - loopTime);
    loopTime = millis();
	  
    #ifdef DEBUG_LOGGING
    fprintf(stderr, "INFO: Battery voltage: %f V  Battery Threshold %f V\n", batteryVoltage, batteryThreshold);
    #endif
    if ((batteryVoltage > 1.0) && (batteryVoltage < batteryThreshold)) // no battery INA219 will give 0V, no battery plugged into INA219 will read < 1V
    {
      fprintf(stderr, "Battery voltage too low: %f V - shutting down!\n", batteryVoltage);
      digitalWrite(txLed, txLedOff);
      digitalWrite(onLed, onLedOff);
      
      sleep(1);
      digitalWrite(onLed, onLedOn);
      sleep(1);
      digitalWrite(onLed, onLedOff);
      sleep(1);
      digitalWrite(onLed, onLedOn);
      sleep(1);
      digitalWrite(onLed, onLedOff);

      popen("sudo shutdown -h now > /dev/null 2>&1", "r");
      sleep(10);
    }

    //  sleep(1);  // Delay 1 second
    ctr = 0;
    #ifdef DEBUG_LOGGING
    fprintf(stderr, "INFO: Getting TLM Data\n");
    #endif

    if ((mode == AFSK) || (mode == CW)) {
      get_tlm();
    } else // FSK or BPSK
    {
      get_tlm_fox();
    }

    #ifdef DEBUG_LOGGING
//    fprintf(stderr, "INFO: Getting ready to send\n");
    #endif
  }

  if (mode == BPSK) {
    digitalWrite(txLed, txLedOn);
    #ifdef DEBUG_LOGGING
    printf("Tx LED On\n");
    #endif
    printf("Sleeping to allow BPSK transmission to finish.\n");
    sleep((unsigned int)(loop_count * 5));
    printf("Done sleeping\n");
    digitalWrite(txLed, txLedOff);
    #ifdef DEBUG_LOGGING
    printf("Tx LED Off\n");
    #endif
  } else if (mode == FSK) {
    printf("Sleeping to allow FSK transmission to finish.\n");
    sleep((unsigned int)loop_count);
    printf("Done sleeping\n");
  }

  return 0;
}

//  Returns lower digit of a number which must be less than 99
//
int lower_digit(int number) {

  int digit = 0;
  if (number < 100)
    digit = number - ((int)(number / 10) * 10);
  else
    fprintf(stderr, "ERROR: Not a digit in lower_digit!\n");
  return digit;
}

// Returns upper digit of a number which must be less than 99
//
int upper_digit(int number) {

  int digit = 0;
  if (number < 100)

    digit = (int)(number / 10);
  else
    fprintf(stderr, "ERROR: Not a digit in upper_digit!\n");
  return digit;
}

static int init_rf() {
  int ret;
  fprintf(stderr, "Initializing AX5043\n");

  ret = ax5043_init( & hax5043, XTAL_FREQ_HZ, VCO_INTERNAL);
  if (ret != PQWS_SUCCESS) {
    fprintf(stderr,
      "ERROR: Failed to initialize AX5043 with error code %d\n", ret);
    //       exit(EXIT_FAILURE);
    return (0);
  }
  return (1);
}

void get_tlm(void) {

  FILE * txResult;

  for (int j = 0; j < frameCnt; j++) {
    digitalWrite(txLed, txLedOn);
    #ifdef DEBUG_LOGGING
    printf("Tx LED On\n");
    #endif
    int tlm[7][5];
    memset(tlm, 0, sizeof tlm);

    //  Reading I2C voltage and current sensors

    int count1;
    char * token;
//    char cmdbuffer[1000];

//    FILE * file = popen(pythonStr, "r");
    fputc('\n', file1);
    fgets(cmdbuffer, 1000, file1);
    printf("Pyton result: %s\n", cmdbuffer);
//    pclose(file);

    const char space[2] = " ";
    token = strtok(cmdbuffer, space);

    float voltage[9], current[9];

    memset(voltage, 0, sizeof(voltage));
    memset(current, 0, sizeof(current));

    for (count1 = 0; count1 < 8; count1++) {
      if (token != NULL) {
        voltage[count1] = (float) atof(token);
        #ifdef DEBUG_LOGGING
        //		 printf("voltage: %f ", voltage[count1]);
        #endif
        token = strtok(NULL, space);
        if (token != NULL) {
          current[count1] = (float) atof(token);
          if ((current[count1] < 0) && (current[count1] > -0.5))
            current[count1] *= (-1);
          #ifdef DEBUG_LOGGING
          //		    printf("current: %f\n", current[count1]);
          #endif
          token = strtok(NULL, space);
        }
      }
    }

    batteryVoltage = voltage[map[BAT]];

    double cpuTemp;

    FILE * cpuTempSensor = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (cpuTempSensor) {
      fscanf(cpuTempSensor, "%lf", & cpuTemp);
      cpuTemp /= 1000;

      #ifdef DEBUG_LOGGING
      printf("CPU Temp Read: %6.1f\n", cpuTemp);
      #endif

    }
    fclose(cpuTempSensor);

    if (sim_mode) {
      // simulated telemetry 

      double time = ((long int) millis() - time_start) / 1000.0;

      if ((time - eclipse_time) > period) {
        eclipse = (eclipse == 1) ? 0.0 : 1.0;
        eclipse_time = time;
        printf("\n\nSwitching eclipse mode! \n\n");
      }

      /*
        double Xi = eclipse * amps_max[0] * sin(2.0 * 3.14 * time / (46.0 * speed)) * fabs(sin(2.0 * 3.14 * time / (46.0 * speed))) + rnd_float(-2, 2);	  
        double Yi = eclipse * amps_max[1] * sin((2.0 * 3.14 * time / (46.0 * speed)) + (3.14/2.0)) * fabs(sin((2.0 * 3.14 * time / (46.0 * speed)) + (3.14/2.0))) + rnd_float(-2, 2);	  
        double Zi = eclipse * amps_max[2] * sin((2.0 * 3.14 * time / (46.0 * speed)) + 3.14 + angle[2])  * fabs(sin((2.0 * 3.14 * time / (46.0 * speed)) + 3.14 + angle[2])) + rnd_float(-2, 2);
      */
/*	    
      double Xi = eclipse * amps_max[0] * sin(2.0 * 3.14 * time / (46.0 * speed)) + rnd_float(-2, 2);
      double Yi = eclipse * amps_max[1] * sin((2.0 * 3.14 * time / (46.0 * speed)) + (3.14 / 2.0)) + rnd_float(-2, 2);
      double Zi = eclipse * amps_max[2] * sin((2.0 * 3.14 * time / (46.0 * speed)) + 3.14 + angle[2]) + rnd_float(-2, 2);

      double Xv = eclipse * volts_max[0] * sin(2.0 * 3.14 * time / (46.0 * speed)) + rnd_float(-0.2, 0.2);
      double Yv = eclipse * volts_max[1] * sin((2.0 * 3.14 * time / (46.0 * speed)) + (3.14 / 2.0)) + rnd_float(-0.2, 0.2);
      double Zv = 2.0 * eclipse * volts_max[2] * sin((2.0 * 3.14 * time / (46.0 * speed)) + 3.14 + angle[2]) + rnd_float(-0.2, 0.2);
*/
      float Xi = eclipse * amps_max[0] * (float) sin(2.0 * 3.14 * time / (46.0 * speed)) + rnd_float(-2, 2);
      float Yi = eclipse * amps_max[1] * (float) sin((2.0 * 3.14 * time / (46.0 * speed)) + (3.14 / 2.0)) + rnd_float(-2, 2);
      float Zi = eclipse * amps_max[2] * (float) sin((2.0 * 3.14 * time / (46.0 * speed)) + 3.14 + angle[2]) + rnd_float(-2, 2);

      float Xv = eclipse * volts_max[0] * (float) sin(2.0 * 3.14 * time / (46.0 * speed)) + rnd_float(-0.2, 0.2);
      float Yv = eclipse * volts_max[1] * (float) sin((2.0 * 3.14 * time / (46.0 * speed)) + (3.14 / 2.0)) + rnd_float(-0.2, 0.2);
      float Zv = 2.0 * eclipse * volts_max[2] * (float) sin((2.0 * 3.14 * time / (46.0 * speed)) + 3.14 + angle[2]) + rnd_float(-0.2, 0.2);

      // printf("Yi: %f Zi: %f %f %f Zv: %f \n", Yi, Zi, amps_max[2], angle[2], Zv);

      current[map[PLUS_X]] = (Xi >= 0) ? Xi : 0;
      current[map[MINUS_X]] = (Xi >= 0) ? 0 : ((-1.0f) * Xi);
      current[map[PLUS_Y]] = (Yi >= 0) ? Yi : 0;
      current[map[MINUS_Y]] = (Yi >= 0) ? 0 : ((-1.0f) * Yi);
      current[map[PLUS_Z]] = (Zi >= 0) ? Zi : 0;
      current[map[MINUS_Z]] = (Zi >= 0) ? 0 : ((-1.0f) * Zi);

      voltage[map[PLUS_X]] = (Xv >= 1) ? Xv : rnd_float(0.9, 1.1);
      voltage[map[MINUS_X]] = (Xv <= -1) ? ((-1.0f) * Xv) : rnd_float(0.9, 1.1);
      voltage[map[PLUS_Y]] = (Yv >= 1) ? Yv : rnd_float(0.9, 1.1);
      voltage[map[MINUS_Y]] = (Yv <= -1) ? ((-1.0f) * Yv) : rnd_float(0.9, 1.1);
      voltage[map[PLUS_Z]] = (Zv >= 1) ? Zv : rnd_float(0.9, 1.1);
      voltage[map[MINUS_Z]] = (Zv <= -1) ? ((-1.0f) * Zv) : rnd_float(0.9, 1.1);

      // printf("temp: %f Time: %f Eclipse: %d : %f %f | %f %f | %f %f\n",tempS, time, eclipse, voltage[map[PLUS_X]], voltage[map[MINUS_X]], voltage[map[PLUS_Y]], voltage[map[MINUS_Y]], current[map[PLUS_Z]], current[map[MINUS_Z]]);

      tempS += (eclipse > 0) ? ((temp_max - tempS) / 50.0) : ((temp_min - tempS) / 50.0f);
      cpuTemp = tempS + rnd_float(-1.0, 1.0);

      voltage[map[BUS]] = rnd_float(5.0, 5.005);
      current[map[BUS]] = rnd_float(158, 171);

      //  float charging = current[map[PLUS_X]] + current[map[MINUS_X]] + current[map[PLUS_Y]] + current[map[MINUS_Y]] + current[map[PLUS_Z]] + current[map[MINUS_Z]];
      float charging = eclipse * (fabs(amps_max[0] * 0.707) + fabs(amps_max[1] * 0.707) + rnd_float(-4.0, 4.0));

      current[map[BAT]] = ((current[map[BUS]] * voltage[map[BUS]]) / batt) - charging;

      //  printf("charging: %f bat curr: %f bus curr: %f bat volt: %f bus volt: %f \n",charging, current[map[BAT]], current[map[BUS]], batt, voltage[map[BUS]]);

      batt -= (batt > 3.5) ? current[map[BAT]] / 30000 : current[map[BAT]] / 3000;
      if (batt < 3.0) {
        batt = 3.0;
        printf("Safe Mode!\n");
      }
      if (batt > 4.5)
        batt = 4.5;

      voltage[map[BAT]] = batt + rnd_float(-0.01, 0.01);

      // end of simulated telemetry	
    }
    tlm[1][A] = (int)(voltage[map[BUS]] / 15.0 + 0.5) % 100; // Current of 5V supply to Pi
    tlm[1][B] = (int)(99.5 - current[map[PLUS_X]] / 10.0) % 100; // +X current [4]
    tlm[1][C] = (int)(99.5 - current[map[MINUS_X]] / 10.0) % 100; // X- current [10] 
    tlm[1][D] = (int)(99.5 - current[map[PLUS_Y]] / 10.0) % 100; // +Y current [7]

    tlm[2][A] = (int)(99.5 - current[map[MINUS_Y]] / 10.0) % 100; // -Y current [10] 
    tlm[2][B] = (int)(99.5 - current[map[PLUS_Z]] / 10.0) % 100; // +Z current [10] // was 70/2m transponder power, AO-7 didn't have a Z panel
    tlm[2][C] = (int)(99.5 - current[map[MINUS_Z]] / 10.0) % 100; // -Z current (was timestamp)
    tlm[2][D] = (int)(50.5 + current[map[BAT]] / 10.0) % 100; // NiMH Battery current

    tlm[3][A] = abs((int)((voltage[map[BAT]] * 10.0) - 65.5) % 100);
    tlm[3][B] = (int)(voltage[map[BUS]] * 10.0) % 100; // 5V supply to Pi

    tlm[4][B] = (int)((95.8 - cpuTemp) / 1.48 + 0.5) % 100;

    tlm[6][B] = 0;
    tlm[6][D] = 49 + rand() % 3;

    #ifdef DEBUG_LOGGING
    // Display tlm
    int k, j;
    for (k = 1; k < 7; k++) {
      for (j = 1; j < 5; j++) {
        printf(" %2d ", tlm[k][j]);
      }
      printf("\n");
    }
    #endif

    char str[1000];
    char tlm_str[1000];
    char header_str[] = "\x03\xf0hi hi ";
    char header_str3[] = "echo '";
    //char header_str2[] = ">CQ:>041440zhi hi ";
    //char header_str2[] = ">CQ:=4003.79N\\07534.33WShi hi ";
    char header_str2[] = ">CQ:";
    char header_str2b[30]; // for APRS coordinates
    char header_lat[10];
    char header_long[10];
    char header_str4[] = "hi hi ";
    char footer_str1[] = "\' > t.txt && echo \'";
    char footer_str[] = ">CQ:010101/hi hi ' >> t.txt && gen_packets -o telem.wav t.txt -r 48000 > /dev/null 2>&1 && cat telem.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1";

    if (ax5043) {
      strcpy(str, header_str);
    } else {
      strcpy(str, header_str3);
      if (mode != CW) {
        strcat(str, call);
        strcat(str, header_str2);
        //	sprintf(header_str2b, "=%7.2f%c%c%c%08.2f%cShi hi ",4003.79,'N',0x5c,0x5c,07534.33,'W');  // add APRS lat and long
        if (latitude > 0)
          sprintf(header_lat, "%7.2f%c", latitude * 100.0, 'N'); // lat
        else
          sprintf(header_lat, "%7.2f%c", latitude * (-100.0), 'S'); // lat
        if (longitude > 0)
          sprintf(header_long, "%08.2f%c", longitude * 100.0, 'E'); // long
        else
          sprintf(header_long, "%08.2f%c", longitude * (-100.0), 'W'); // long

        sprintf(header_str2b, "=%s%c%c%sShi hi ", header_lat, 0x5c, 0x5c, header_long); // add APRS lat and long	    
        printf("\n\nString is %s \n\n", header_str2b);
        strcat(str, header_str2b);
      } else {
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

    if (payload == ON) {
      char c;
      int charss = (char) serialDataAvail(uart_fd);
      if (charss != 0)
        printf("Clearing buffer of %d chars \n", charss);
      while ((charss--> 0))
        c = (char) serialGetchar(uart_fd); // clear buffer

      unsigned int waitTime;
      int i = 0;

      serialPutchar(uart_fd, '?');
      printf("Querying payload with ?\n");
      waitTime = millis() + 500;
      int end = FALSE;
      while ((millis() < waitTime) && !end) {
        int chars = (char) serialDataAvail(uart_fd);
        while ((chars--> 0) && !end) {
          c = (char) serialGetchar(uart_fd);
          //	  printf ("%c", c);
          //	  fflush(stdout);
          if (c != '\n') {
            sensor_payload[i++] = c;
          } else {
            end = TRUE;
          }
        }
      }
      //    sensor_payload[i++] = '\n';
      sensor_payload[i] = '\0';
      printf("Payload string: %s", sensor_payload);

      strcat(str, sensor_payload); // append to telemetry string for transmission
    }

    digitalWrite(txLed, txLedOn);
    #ifdef DEBUG_LOGGING
    printf("Tx LED On\n");
    #endif
    if (mode == CW)
      system(cw_str2);
    digitalWrite(txLed, txLedOn);
    #ifdef DEBUG_LOGGING
    printf("Tx LED On\n");
    #endif

    if (ax5043) {
      digitalWrite(txLed, txLedOn);
      #ifdef DEBUG_LOGGING
      printf("Tx LED On\n");
      #endif
      fprintf(stderr, "INFO: Transmitting X.25 packet using AX5043\n");
      memcpy(data, str, strnlen(str, 256));
      int ret = ax25_tx_frame( & hax25, & hax5043, data, strnlen(str, 256));
      if (ret) {
        fprintf(stderr,
          "ERROR: Failed to transmit AX.25 frame with error code %d\n",
          ret);
        exit(EXIT_FAILURE);
      }
      ax5043_wait_for_transmit();
      digitalWrite(txLed, txLedOff);
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
    } else {
      strcat(str, footer_str1);
      strcat(str, call);
      strcat(str, footer_str);
      fprintf(stderr, "String to execute: %s\n", str);
      if (transmit) {
        FILE * file2 = popen(str, "r");
        pclose(file2);
      } else {
        fprintf(stderr, "\nNo CubeSatSim Band Pass Filter detected.  No transmissions after the CW ID.\n");
        fprintf(stderr, " See http://cubesatsim.org/wiki for info about building a CubeSatSim\n\n");
      }
      digitalWrite(txLed, txLedOff);
      #ifdef DEBUG_LOGGING
      printf("Tx LED Off\n");
      #endif
      sleep(3);
      digitalWrite(txLed, txLedOn);
      #ifdef DEBUG_LOGGING
      printf("Tx LED On\n");
      #endif
    }

  }

  digitalWrite(txLed, txLedOff);
  #ifdef DEBUG_LOGGING
  printf("Tx LED Off\n");
  #endif

  return;
}

void get_tlm_fox() {

  //  Reading I2C voltage and current sensors
/*
  FILE * uptime_file = fopen("/proc/uptime", "r");
  fscanf(uptime_file, "%f", & uptime_sec);
  uptime = (int) uptime_sec;
  #ifdef DEBUG_LOGGING
  printf("Reset Count: %d Uptime since Reset: %ld \n", reset_count, uptime);
  #endif
  fclose(uptime_file);
*/
  int i;
  //	long int sync = SYNC_WORD;
  long int sync = syncWord;

  smaller = (int) (S_RATE / (2 * freq_Hz));

  //	short int b[DATA_LEN];
  short int b[dataLen];
  short int b_max[dataLen];
  short int b_min[dataLen];
	
  memset(b, 0, sizeof(b));
  memset(b_max, 0, sizeof(b_max));
  memset(b_min, 0, sizeof(b_min));
	
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

  int id, frm_type = 0x01, STEMBoardFailure = 1, NormalModeFailure = 0, groundCommandCount = 0;
  int PayloadFailure1 = 0, PayloadFailure2 = 0;
  int PSUVoltage = 0, PSUCurrent = 0, Resets = 0, Rssi = 2048;
  int batt_a_v = 0, batt_b_v = 0, batt_c_v = 0, battCurr = 0;
  int posXv = 0, negXv = 0, posYv = 0, negYv = 0, posZv = 0, negZv = 0;
  int posXi = 0, negXi = 0, posYi = 0, negYi = 0, posZi = 0, negZi = 0;
  int head_offset = 0;
  //  int xAngularVelocity = (-0.69)*(-10)*(-10) + 45.3 * (-10) + 2078, yAngularVelocity = (-0.69)*(-6)*(-6) + 45.3 * (-6) + 2078, zAngularVelocity = (-0.69)*(6)*(6) + 45.3 * (6) + 2078; // XAxisAngularVelocity
  //  int xAngularVelocity = 2078, yAngularVelocity = 2078, zAngularVelocity = 2078;  // XAxisAngularVelocity Y and Z set to 0
  // int xAngularVelocity = 2048, yAngularVelocity = 2048, zAngularVelocity = 2048; // XAxisAngularVelocity Y and Z set to 0
  // int RXTemperature = 0, temp = 0, spin = 0;;
  // float xAccel = 0.0, yAccel = 0.0, zAccel = 0.0;
  // float BME280pressure = 0.0, BME280altitude = 0.0, BME280humidity = 0.0, BME280temperature = 0.0;
  // float XSsensor1 = 0.0, XSsensor2 = 0.0, XSsensor3 = 0.0;
  // int sensor1 = 0, sensor2 = 2048, sensor3 = 2048;

  short int buffer_test[bufLen];
  int buffSize;
  buffSize = (int) sizeof(buffer_test);

  if (mode == FSK)
    id = 7;
  else
    id = 0; // 99 in h[6]

  //  for (int frames = 0; frames < FRAME_CNT; frames++) 
  for (int frames = 0; frames < frameCnt; frames++) {

    if (firstTime != ON) {
      // delay for sample period
      digitalWrite(txLed, txLedOn);
      #ifdef DEBUG_LOGGING
//      printf("Tx LED On\n");
      #endif
/*
      while ((millis() - sampleTime) < (unsigned int)samplePeriod)
        sleep((unsigned int)sleepTime);
*/
//      if (mode == FSK) 
	sleep(2.3);  // No sleep at all!
//      else	    
//        sleep(1.3);
	    
      digitalWrite(txLed, txLedOff);
      #ifdef DEBUG_LOGGING
//      printf("Tx LED Off\n");
      #endif

//      printf("Sample period: %d\n", millis() - (unsigned int)sampleTime);
      sampleTime = (unsigned int) millis();
    } else
      printf("first time - no sleep\n");
	  

	  
    float voltage[9], current[9], sensor[17], other[3];
    char sensor_payload[500];
    memset(voltage, 0, sizeof(voltage));
    memset(current, 0, sizeof(current));
    memset(sensor, 0, sizeof(sensor));
    memset(other, 0, sizeof(other));	
	  
    FILE * uptime_file = fopen("/proc/uptime", "r");
    fscanf(uptime_file, "%f", & uptime_sec);
    uptime = (int) uptime_sec;
    #ifdef DEBUG_LOGGING
    printf("INFO: Reset Count: %d Uptime since Reset: %ld \n", reset_count, uptime);
    #endif
    fclose(uptime_file);	
	  
    if (sim_mode) { // simulated telemetry 

      double time = ((long int)millis() - time_start) / 1000.0;

      if ((time - eclipse_time) > period) {
        eclipse = (eclipse == 1) ? 0 : 1;
        eclipse_time = time;
        printf("\n\nSwitching eclipse mode! \n\n");
      }

      double Xi = eclipse * amps_max[0] * (float) sin(2.0 * 3.14 * time / (46.0 * speed)) + rnd_float(-2, 2);
      double Yi = eclipse * amps_max[1] * (float) sin((2.0 * 3.14 * time / (46.0 * speed)) + (3.14 / 2.0)) + rnd_float(-2, 2);
      double Zi = eclipse * amps_max[2] * (float) sin((2.0 * 3.14 * time / (46.0 * speed)) + 3.14 + angle[2]) + rnd_float(-2, 2);

      double Xv = eclipse * volts_max[0] * (float) sin(2.0 * 3.14 * time / (46.0 * speed)) + rnd_float(-0.2, 0.2);
      double Yv = eclipse * volts_max[1] * (float) sin((2.0 * 3.14 * time / (46.0 * speed)) + (3.14 / 2.0)) + rnd_float(-0.2, 0.2);
      double Zv = 2.0 * eclipse * volts_max[2] * (float) sin((2.0 * 3.14 * time / (46.0 * speed)) + 3.14 + angle[2]) + rnd_float(-0.2, 0.2);

      // printf("Yi: %f Zi: %f %f %f Zv: %f \n", Yi, Zi, amps_max[2], angle[2], Zv);

      current[map[PLUS_X]] = (Xi >= 0) ? Xi : 0;
      current[map[MINUS_X]] = (Xi >= 0) ? 0 : ((-1.0f) * Xi);
      current[map[PLUS_Y]] = (Yi >= 0) ? Yi : 0;
      current[map[MINUS_Y]] = (Yi >= 0) ? 0 : ((-1.0f) * Yi);
      current[map[PLUS_Z]] = (Zi >= 0) ? Zi : 0;
      current[map[MINUS_Z]] = (Zi >= 0) ? 0 : ((-1.0f) * Zi);

      voltage[map[PLUS_X]] = (Xv >= 1) ? Xv : rnd_float(0.9, 1.1);
      voltage[map[MINUS_X]] = (Xv <= -1) ? ((-1.0f) * Xv) : rnd_float(0.9, 1.1);
      voltage[map[PLUS_Y]] = (Yv >= 1) ? Yv : rnd_float(0.9, 1.1);
      voltage[map[MINUS_Y]] = (Yv <= -1) ? ((-1.0f) * Yv) : rnd_float(0.9, 1.1);
      voltage[map[PLUS_Z]] = (Zv >= 1) ? Zv : rnd_float(0.9, 1.1);
      voltage[map[MINUS_Z]] = (Zv <= -1) ? ((-1.0f) * Zv) : rnd_float(0.9, 1.1);

      // printf("temp: %f Time: %f Eclipse: %d : %f %f | %f %f | %f %f\n",tempS, time, eclipse, voltage[map[PLUS_X]], voltage[map[MINUS_X]], voltage[map[PLUS_Y]], voltage[map[MINUS_Y]], current[map[PLUS_Z]], current[map[MINUS_Z]]);

      tempS += (eclipse > 0) ? ((temp_max - tempS) / 50.0f) : ((temp_min - tempS) / 50.0f);
      tempS += +rnd_float(-1.0, 1.0);
      //  IHUcpuTemp = (int)((tempS + rnd_float(-1.0, 1.0)) * 10 + 0.5);
      other[IHU_TEMP] = tempS;

      voltage[map[BUS]] = rnd_float(5.0, 5.005);
      current[map[BUS]] = rnd_float(158, 171);

      //  float charging = current[map[PLUS_X]] + current[map[MINUS_X]] + current[map[PLUS_Y]] + current[map[MINUS_Y]] + current[map[PLUS_Z]] + current[map[MINUS_Z]];
      float charging = eclipse * (fabs(amps_max[0] * 0.707) + fabs(amps_max[1] * 0.707) + rnd_float(-4.0, 4.0));

      current[map[BAT]] = ((current[map[BUS]] * voltage[map[BUS]]) / batt) - charging;

      //  printf("charging: %f bat curr: %f bus curr: %f bat volt: %f bus volt: %f \n",charging, current[map[BAT]], current[map[BUS]], batt, voltage[map[BUS]]);

      batt -= (batt > 3.5) ? current[map[BAT]] / 30000 : current[map[BAT]] / 3000;
      if (batt < 3.0) {
        batt = 3.0;
        NormalModeFailure = 1;
        printf("Safe Mode!\n");
      } else
        NormalModeFailure = 0;

      if (batt > 4.5)
        batt = 4.5;

      voltage[map[BAT]] = batt + rnd_float(-0.01, 0.01);

      // end of simulated telemetry
    }
    else {
      int count1;
      char * token;
//    char cmdbuffer[1000];
/**/
//    FILE * file = popen(pythonStr, "r");
      fputc('\n', file1);
      fgets(cmdbuffer, 1000, file1);
      fprintf(stderr, "Python read Result: %s\n", cmdbuffer);
//    pclose(file);
/**/
      const char space[2] = " ";
      token = strtok(cmdbuffer, space);

      for (count1 = 0; count1 < 8; count1++) {
        if (token != NULL) {
          voltage[count1] = (float) atof(token);
          #ifdef DEBUG_LOGGING
          //  printf("voltage: %f ", voltage[count1]);
          #endif
          token = strtok(NULL, space);
          if (token != NULL) {
            current[count1] = (float) atof(token);
            if ((current[count1] < 0) && (current[count1] > -0.5))
              current[count1] *= (-1.0f);
            #ifdef DEBUG_LOGGING
            //  printf("current: %f\n", current[count1]);
            #endif
            token = strtok(NULL, space);
          }
        }
      }
    //	 printf("\n"); 	  
	
//   sleep(0.5);
//    printf("Sleep over\n");	
	
      batteryVoltage = voltage[map[BAT]];
      if (batteryVoltage < 3.5) {
        NormalModeFailure = 1;
        printf("Safe Mode!\n");
      } else
        NormalModeFailure = 0;

      FILE * cpuTempSensor = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
      if (cpuTempSensor) {
        double cpuTemp;
        fscanf(cpuTempSensor, "%lf", & cpuTemp);
        cpuTemp /= 1000;

        #ifdef DEBUG_LOGGING
        printf("CPU Temp Read: %6.1f\n", cpuTemp);
        #endif

        other[IHU_TEMP] = (double)cpuTemp;

      //  IHUcpuTemp = (int)((cpuTemp * 10.0) + 0.5);
      }
      fclose(cpuTempSensor);

      if (payload == ON) {  // -55
        STEMBoardFailure = 0;

        char c;
        int charss = (char) serialDataAvail(uart_fd);
        if (charss != 0)
        printf("Clearing buffer of %d chars \n", charss);
        while ((charss--> 0))
          c = (char) serialGetchar(uart_fd); // clear buffer
/*	      
        charss = (char) serialDataAvail(uart_fd);
        if (charss != 0)
        printf("Clearing buffer of %d chars \n", charss);
        while ((charss--> 0))
          c = (char) serialGetchar(uart_fd); // clear buffer
*/	      

        unsigned int waitTime;
        int i = 0;
        serialPutchar(uart_fd, '?');
        printf("Querying payload with ?\n");
        waitTime = millis() + 500;
        int end = FALSE;
        //  int retry = FALSE;
        while ((millis() < waitTime) && !end) {
          int chars = (char) serialDataAvail(uart_fd);
          while ((chars--> 0) && !end) {
            c = (char) serialGetchar(uart_fd);
            //	printf ("%c", c);
            //	fflush(stdout);
            if (c != '\n') {
              sensor_payload[i++] = c;
            } else {
              end = TRUE;
            }
          }
        }
        sensor_payload[i++] = ' ';
        //  sensor_payload[i++] = '\n';
        sensor_payload[i] = '\0';
        printf("Payload string: %s", sensor_payload);

        if ((sensor_payload[0] == 'O') && (sensor_payload[1] == 'K')) // only process if valid payload response
        {
          int count1;
          char * token;
          //  char cmdbuffer[1000];

        //	FILE *file = popen("python3 /home/pi/CubeSatSim/python/voltcurrent.py 1 11", "r");	
        //    	fgets(cmdbuffer, 1000, file);
        //	printf("result: %s\n", cmdbuffer);
        //    	pclose(file);

          const char space[2] = " ";
          token = strtok(sensor_payload, space);
          for (count1 = 0; count1 < 17; count1++) {
            if (token != NULL) {
              sensor[count1] = (float) atof(token);
              #ifdef DEBUG_LOGGING
              //  printf("sensor: %f ", sensor[count1]);
              #endif
              token = strtok(NULL, space);
            }
          }
          printf("\n");
        }
      }
      if ((sensor_payload[0] == 'O') && (sensor_payload[1] == 'K')) {
        for (count1 = 0; count1 < 17; count1++) {
          if (sensor[count1] < sensor_min[count1])
            sensor_min[count1] = sensor[count1];
          if (sensor[count1] > sensor_max[count1])
            sensor_max[count1] = sensor[count1];
            //  printf("Smin %f Smax %f \n", sensor_min[count1], sensor_max[count1]);
        }   	    
      }
    }
//    if (mode == FSK) 
    {  // just moved
      for (int count1 = 0; count1 < 8; count1++) {
        if (voltage[count1] < voltage_min[count1])
          voltage_min[count1] = voltage[count1];
        if (current[count1] < current_min[count1])
          current_min[count1] = current[count1];
	      
        if (voltage[count1] > voltage_max[count1])
          voltage_max[count1] = voltage[count1];
        if (current[count1] > current_max[count1])
          current_max[count1] = current[count1];

         printf("Vmin %4.2f Vmax %4.2f Imin %4.2f Imax %4.2f \n", voltage_min[count1], voltage_max[count1], current_min[count1], current_max[count1]);
      }
       for (int count1 = 0; count1 < 3; count1++) {
        if (other[count1] < other_min[count1])
          other_min[count1] = other[count1];
        if (other[count1] > other_max[count1])
          other_max[count1] = other[count1];

        //  printf("Other min %f max %f \n", other_min[count1], other_max[count1]);
      }
      	  if (mode == FSK)
	  {
	      if (loop % 8 == 0) {
		printf("Sending MIN frame \n");
		frm_type = 0x03;
		for (int count1 = 0; count1 < 17; count1++) {
		  if (count1 < 3)
		    other[count1] = other_min[count1];
		  if (count1 < 8) {
		    voltage[count1] = voltage_min[count1];
		    current[count1] = current_min[count1];
		  }
		  if (sensor_min[count1] != 1000.0) // make sure values are valid
		    sensor[count1] = sensor_min[count1];
		}
	      }
	      if ((loop + 4) % 8 == 0) {
		printf("Sending MAX frame \n");
		frm_type = 0x02;
		for (int count1 = 0; count1 < 17; count1++) {
		  if (count1 < 3)
		    other[count1] = other_max[count1];
		  if (count1 < 8) {
		    voltage[count1] = voltage_max[count1];
		    current[count1] = current_max[count1];
		  }
		  if (sensor_max[count1] != -1000.0) // make sure values are valid
		    sensor[count1] = sensor_max[count1];
		}
	      }
	  }
	  else
	  	frm_type = 0x02;  // BPSK always send MAX MIN frame
    }  
	  
//   if (mode == FSK) {	// remove this 
//   }
    memset(rs_frame, 0, sizeof(rs_frame));
    memset(parities, 0, sizeof(parities));
/*
    FILE * uptime_file = fopen("/proc/uptime", "r");
    fscanf(uptime_file, "%f", & uptime_sec);
    uptime = (int) uptime_sec;
    fclose(uptime_file);
    printf("Reset Count: %d Uptime since Reset: %ld \n", reset_count, uptime);
*/	  
    //sleep(1);
    //printf("Sleep over\n");

    h[0] = (short int) ((h[0] & 0xf8) | (id & 0x07)); // 3 bits
    //    printf("h[0] %x\n", h[0]);
    h[0] = (short int) ((h[0] & 0x07) | ((reset_count & 0x1f) << 3));
    //    printf("h[0] %x\n", h[0]);
    h[1] = (short int) ((reset_count >> 5) & 0xff);
    //    printf("h[1] %x\n", h[1]);
    h[2] = (short int) ((h[2] & 0xf8) | ((reset_count >> 13) & 0x07));
    //    printf("h[2] %x\n", h[2]);
    h[2] = (short int) ((h[2] & 0x0e) | ((uptime & 0x1f) << 3));
    //    printf("h[2] %x\n", h[2]);
    h[3] = (short int) ((uptime >> 5) & 0xff);
    h[4] = (short int) ((uptime >> 13) & 0xff);
    h[5] = (short int) ((h[5] & 0xf0) | ((uptime >> 21) & 0x0f));
    h[5] = (short int) ((h[5] & 0x0f) | (frm_type << 4));

    if (mode == BPSK)
      h[6] = 99;

    posXi = (int)(current[map[PLUS_X]] + 0.5) + 2048;
    posYi = (int)(current[map[PLUS_Y]] + 0.5) + 2048;
    posZi = (int)(current[map[PLUS_Z]] + 0.5) + 2048;
    negXi = (int)(current[map[MINUS_X]] + 0.5) + 2048;
    negYi = (int)(current[map[MINUS_Y]] + 0.5) + 2048;
    negZi = (int)(current[map[MINUS_Z]] + 0.5) + 2048;

    posXv = (int)(voltage[map[PLUS_X]] * 100);
    posYv = (int)(voltage[map[PLUS_Y]] * 100);
    posZv = (int)(voltage[map[PLUS_Z]] * 100);
    negXv = (int)(voltage[map[MINUS_X]] * 100);
    negYv = (int)(voltage[map[MINUS_Y]] * 100);
    negZv = (int)(voltage[map[MINUS_Z]] * 100);

    batt_c_v = (int)(voltage[map[BAT]] * 100);
    battCurr = (int)(current[map[BAT]] + 0.5) + 2048;
    PSUVoltage = (int)(voltage[map[BUS]] * 100);
    PSUCurrent = (int)(current[map[BUS]] + 0.5) + 2048;

    if (payload == ON)
      STEMBoardFailure = 0;

    // read payload sensor if available

    encodeA(b, 0 + head_offset, batt_a_v);
    encodeB(b, 1 + head_offset, batt_b_v);
    encodeA(b, 3 + head_offset, batt_c_v);

    //  encodeB(b, 4 + head_offset, (int)(xAccel * 100 + 0.5) + 2048);	  // Xaccel
    //  encodeA(b, 6 + head_offset, (int)(yAccel * 100 + 0.5) + 2048);	  // Yaccel
    //  encodeB(b, 7 + head_offset, (int)(zAccel * 100 + 0.5) + 2048);	  // Zaccel

    encodeB(b, 4 + head_offset, (int)(sensor[ACCEL_X] * 100 + 0.5) + 2048); // Xaccel
    encodeA(b, 6 + head_offset, (int)(sensor[ACCEL_Y] * 100 + 0.5) + 2048); // Yaccel
    encodeB(b, 7 + head_offset, (int)(sensor[ACCEL_Z] * 100 + 0.5) + 2048); // Zaccel

    encodeA(b, 9 + head_offset, battCurr);

    //  encodeB(b, 10 + head_offset,(int)(BME280temperature * 10 + 0.5));	// Temp
    encodeB(b, 10 + head_offset, (int)(sensor[TEMP] * 10 + 0.5)); // Temp	  

    if (mode == FSK) {
      encodeA(b, 12 + head_offset, posXv);
      encodeB(b, 13 + head_offset, negXv);
      encodeA(b, 15 + head_offset, posYv);
      encodeB(b, 16 + head_offset, negYv);
      encodeA(b, 18 + head_offset, posZv);
      encodeB(b, 19 + head_offset, negZv);

      encodeA(b, 21 + head_offset, posXi);
      encodeB(b, 22 + head_offset, negXi);
      encodeA(b, 24 + head_offset, posYi);
      encodeB(b, 25 + head_offset, negYi);
      encodeA(b, 27 + head_offset, posZi);
      encodeB(b, 28 + head_offset, negZi);
    } else // BPSK
    {
      encodeA(b, 12 + head_offset, posXv);
      encodeB(b, 13 + head_offset, posYv);
      encodeA(b, 15 + head_offset, posZv);
      encodeB(b, 16 + head_offset, negXv);
      encodeA(b, 18 + head_offset, negYv);
      encodeB(b, 19 + head_offset, negZv);

      encodeA(b, 21 + head_offset, posXi);
      encodeB(b, 22 + head_offset, posYi);
      encodeA(b, 24 + head_offset, posZi);
      encodeB(b, 25 + head_offset, negXi);
      encodeA(b, 27 + head_offset, negYi);
      encodeB(b, 28 + head_offset, negZi);
	    
      encodeA(b_max, 12 + head_offset, (int)(voltage_max[map[PLUS_X]] * 100));
      encodeB(b_max, 13 + head_offset, (int)(voltage_max[map[PLUS_Y]] * 100));
      encodeA(b_max, 15 + head_offset, (int)(voltage_max[map[PLUS_Z]] * 100));
      encodeB(b_max, 16 + head_offset, (int)(voltage_max[map[MINUS_X]] * 100));
      encodeA(b_max, 18 + head_offset, (int)(voltage_max[map[MINUS_Y]] * 100));
      encodeB(b_max, 19 + head_offset, (int)(voltage_max[map[MINUS_Z]] * 100));

      encodeA(b_max, 21 + head_offset, (int)(current_max[map[PLUS_X]] + 0.5) + 2048);
      encodeB(b_max, 22 + head_offset, (int)(current_max[map[PLUS_Y]] + 0.5) + 2048);
      encodeA(b_max, 24 + head_offset, (int)(current_max[map[PLUS_Z]] + 0.5) + 2048);
      encodeB(b_max, 25 + head_offset, (int)(current_max[map[MINUS_X]] + 0.5) + 2048);
      encodeA(b_max, 27 + head_offset, (int)(current_max[map[MINUS_Y]] + 0.5) + 2048);
      encodeB(b_max, 28 + head_offset, (int)(current_max[map[MINUS_Z]] + 0.5) + 2048);	    

      encodeA(b_max, 9 + head_offset, (int)(current_max[map[BAT]] + 0.5) + 2048);
      encodeA(b_max, 3 + head_offset, (int)(voltage_max[map[BAT]] * 100));
      encodeA(b_max, 30 + head_offset, (int)(voltage_max[map[BUS]] * 100));
      encodeB(b_max, 46 + head_offset, (int)(current_max[map[BUS]] + 0.5) + 2048);
	    
      encodeB(b_max, 4 + head_offset, (int)(sensor_max[ACCEL_X] * 100 + 0.5) + 2048); // Xaccel
      encodeA(b_max, 6 + head_offset, (int)(sensor_max[ACCEL_Y] * 100 + 0.5) + 2048); // Yaccel
      encodeB(b_max, 7 + head_offset, (int)(sensor_max[ACCEL_Z] * 100 + 0.5) + 2048); // Zaccel	    

      encodeA(b_max, 33 + head_offset, (int)(sensor_max[PRES] + 0.5)); // Pressure
      encodeB(b_max, 34 + head_offset, (int)(sensor_max[ALT] * 10.0 + 0.5)); // Altitude
      encodeB(b_max, 40 + head_offset, (int)(sensor_max[GYRO_X] + 0.5) + 2048);
      encodeA(b_max, 42 + head_offset, (int)(sensor_max[GYRO_Y] + 0.5) + 2048);
      encodeB(b_max, 43 + head_offset, (int)(sensor_max[GYRO_Z] + 0.5) + 2048);
	    
      encodeA(b_min, 12 + head_offset, (int)(voltage_min[map[PLUS_X]] * 100));
      encodeB(b_min, 13 + head_offset, (int)(voltage_min[map[PLUS_Y]] * 100));
      encodeA(b_min, 15 + head_offset, (int)(voltage_min[map[PLUS_Z]] * 100));
      encodeB(b_min, 16 + head_offset, (int)(voltage_min[map[MINUS_X]] * 100));
      encodeA(b_min, 18 + head_offset, (int)(voltage_min[map[MINUS_Y]] * 100));
      encodeB(b_min, 19 + head_offset, (int)(voltage_min[map[MINUS_Z]] * 100));

      encodeA(b_min, 21 + head_offset, (int)(current_min[map[PLUS_X]] + 0.5) + 2048);
      encodeB(b_min, 22 + head_offset, (int)(current_min[map[PLUS_Y]] + 0.5) + 2048);
      encodeA(b_min, 24 + head_offset, (int)(current_min[map[PLUS_Z]] + 0.5) + 2048);
      encodeB(b_min, 25 + head_offset, (int)(current_min[map[MINUS_X]] + 0.5) + 2048);
      encodeA(b_min, 27 + head_offset, (int)(current_min[map[MINUS_Y]] + 0.5) + 2048);
      encodeB(b_min, 28 + head_offset, (int)(current_min[map[MINUS_Z]] + 0.5) + 2048);	
	    
      encodeA(b_min, 9 + head_offset, (int)(current_min[map[BAT]] + 0.5) + 2048);
      encodeA(b_min, 3 + head_offset, (int)(voltage_min[map[BAT]] * 100));
      encodeA(b_min, 30 + head_offset, (int)(voltage_min[map[BUS]] * 100));
      encodeB(b_min, 46 + head_offset, (int)(current_min[map[BUS]] + 0.5) + 2048);	    
	    
      encodeB(b_min, 4 + head_offset, (int)(sensor_min[ACCEL_X] * 100 + 0.5) + 2048); // Xaccel
      encodeA(b_min, 6 + head_offset, (int)(sensor_min[ACCEL_Y] * 100 + 0.5) + 2048); // Yaccel
      encodeB(b_min, 7 + head_offset, (int)(sensor_min[ACCEL_Z] * 100 + 0.5) + 2048); // Zaccel	

      encodeA(b_min, 33 + head_offset, (int)(sensor_min[PRES] + 0.5)); // Pressure
      encodeB(b_min, 34 + head_offset, (int)(sensor_min[ALT] * 10.0 + 0.5)); // Altitude
      encodeB(b_min, 40 + head_offset, (int)(sensor_min[GYRO_X] + 0.5) + 2048);
      encodeA(b_min, 42 + head_offset, (int)(sensor_min[GYRO_Y] + 0.5) + 2048);
      encodeB(b_min, 43 + head_offset, (int)(sensor_min[GYRO_Z] + 0.5) + 2048);
	    
    }    
    encodeA(b, 30 + head_offset, PSUVoltage);
    //  encodeB(b, 31 + head_offset,(spin * 10) + 2048);	  
    encodeB(b, 31 + head_offset, ((int)(other[SPIN] * 10)) + 2048);

    //  encodeA(b, 33 + head_offset,(int)(BME280pressure + 0.5));  // Pressure
    //  encodeB(b, 34 + head_offset,(int)(BME280altitude + 0.5));   // Altitude

    encodeA(b, 33 + head_offset, (int)(sensor[PRES] + 0.5)); // Pressure
    encodeB(b, 34 + head_offset, (int)(sensor[ALT] * 10.0 + 0.5)); // Altitude

    encodeA(b, 36 + head_offset, Resets);
    //  encodeB(b, 37 + head_offset,  Rssi);	
    encodeB(b, 37 + head_offset, (int)(other[RSSI] + 0.5) + 2048);

    //  encodeA(b, 39 + head_offset,  IHUcpuTemp);
    encodeA(b, 39 + head_offset, (int)(other[IHU_TEMP] * 10 + 0.5));

    //  encodeB(b, 40 + head_offset,  xAngularVelocity);
    //  encodeA(b, 42 + head_offset,  yAngularVelocity);
    //  encodeB(b, 43 + head_offset,  zAngularVelocity);

    encodeB(b, 40 + head_offset, (int)(sensor[GYRO_X] + 0.5) + 2048);
    encodeA(b, 42 + head_offset, (int)(sensor[GYRO_Y] + 0.5) + 2048);
    encodeB(b, 43 + head_offset, (int)(sensor[GYRO_Z] + 0.5) + 2048);

    //  encodeA(b, 45 + head_offset, (int)(BME280humidity + 0.5));  // in place of sensor1
    encodeA(b, 45 + head_offset, (int)(sensor[HUMI] + 0.5)); // in place of sensor1

    encodeB(b, 46 + head_offset, PSUCurrent);
    //  encodeA(b, 48 + head_offset, (int)(XSsensor2) + 2048);
    //  encodeB(b, 49 + head_offset, (int)(XSsensor3 * 100 + 0.5) + 2048);

    encodeA(b, 48 + head_offset, (int)(sensor[XS2]) + 2048);
    encodeB(b, 49 + head_offset, (int)(sensor[XS3] * 100 + 0.5) + 2048);

    // camera = ON;

    int status = STEMBoardFailure + NormalModeFailure * 2 + PayloadFailure1 * 4 + PayloadFailure2 * 8 +
      (i2c_bus0 == OFF) * 16 + (i2c_bus1 == OFF) * 32 + (i2c_bus3 == OFF) * 64 + (camera == OFF) * 128 + groundCommandCount * 256;

    encodeA(b, 51 + head_offset, status);
    //  encodeA(b, 51 + head_offset, STEMBoardFailure + NormalModeFailure * 2 + (i2c_bus0 == OFF) * 16 + (i2c_bus1 == OFF) * 32 + (i2c_bus3 == OFF) * 64  + (0) * 128 + 1 * 256 + 1 * 512 + 1 * 1024 + 1*2048); 
    encodeB(b, 52 + head_offset, rxAntennaDeployed + txAntennaDeployed * 2);

    if (txAntennaDeployed == 0) {
      txAntennaDeployed = 1;
      printf("TX Antenna Deployed!\n");
    }
    
    if (mode == BPSK) {  // wod field experiments
      encodeA(b, 64 + head_offset, 0x7f);  // was 7f -> fe, ff -> 1fe  was 63
//      encodeA(b, 63 + head_offset, 0x1e);  // 1e -> 224,65 ff -> fefe, ff -> e1, 
      encodeA(b, 63 + head_offset, 0xff);  // 0x80 is 1000 0000 at 65
      encodeB(b, 74 + head_offset, 0xfff);  // was ff	
    }
    short int data10[headerLen + rsFrames * (rsFrameLen + parityLen)];
    short int data8[headerLen + rsFrames * (rsFrameLen + parityLen)];

    int ctr1 = 0;
    int ctr3 = 0;
    for (i = 0; i < rsFrameLen; i++) {
      for (int j = 0; j < rsFrames; j++) {
        if (!((i == (rsFrameLen - 1)) && (j == 2))) // skip last one for BPSK
        {
          if (ctr1 < headerLen) {
            rs_frame[j][i] = h[ctr1];
            update_rs(parities[j], h[ctr1]);
            //      				printf("header %d rs_frame[%d][%d] = %x \n", ctr1, j, i, h[ctr1]);
            data8[ctr1++] = rs_frame[j][i];
            //				printf ("data8[%d] = %x \n", ctr1 - 1, rs_frame[j][i]);
          } else {
	     if (mode == FSK)
	     {
            	rs_frame[j][i] = b[ctr3 % dataLen];
            	update_rs(parities[j], b[ctr3 % dataLen]);
	     }  else // BPSK
		if ((int)(ctr3/dataLen) == 3)  
		{
            		rs_frame[j][i] = b_max[ctr3 % dataLen];
            		update_rs(parities[j], b_max[ctr3 % dataLen]);
		}
		else if ((int)(ctr3/dataLen) == 4)  
		{
            		rs_frame[j][i] = b_min[ctr3 % dataLen];
            		update_rs(parities[j], b_min[ctr3 % dataLen]);
		}		
		else
		{
            		rs_frame[j][i] = b[ctr3 % dataLen];
            		update_rs(parities[j], b[ctr3 % dataLen]);
		}
	     {
	    }
		  
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
    /*
    	  printf("Parities ");
    		for (int m = 0; m < parityLen; m++) {
    		 	printf("%d ", parities[0][m]);
    		}
    		printf("\n");
    */
    #endif
	   
    //sleep(1);
    //printf("Sleep over\n");

    int ctr2 = 0;
    memset(data10, 0, sizeof(data10));

    for (i = 0; i < dataLen * payloads + headerLen; i++) // 476 for BPSK
    {
      data10[ctr2] = (Encode_8b10b[rd][((int) data8[ctr2])] & 0x3ff);
      nrd = (Encode_8b10b[rd][((int) data8[ctr2])] >> 10) & 1;
      //		printf ("data10[%d] = encoded data8[%d] = %x \n",
      //		 	ctr2, ctr2, data10[ctr2]); 

      rd = nrd; // ^ nrd;
      ctr2++;
    }
    for (i = 0; i < parityLen; i++) {
      for (int j = 0; j < rsFrames; j++) {
        data10[ctr2++] = (Encode_8b10b[rd][((int) parities[j][i])] & 0x3ff);
        nrd = (Encode_8b10b[rd][((int) parities[j][i])] >> 10) & 1;
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
    //int offset = 0;

    #ifdef DEBUG_LOGGING
    //	printf("\nAt start of buffer loop, syncBits %d samples %d ctr %d\n", syncBits, samples, ctr);
    #endif

    for (i = 1; i <= syncBits * samples; i++) {
      write_wave(ctr, buffer);
      //		printf("%d ",ctr);
      if ((i % samples) == 0) {
        int bit = syncBits - i / samples + 1;
        val = sync;
        data = val & 1 << (bit - 1);
        //   	printf ("%d i: %d new frame %d sync bit %d = %d \n",
        //  		 ctr/SAMPLES, i, frames, bit, (data > 0) );
        if (mode == FSK) {
          phase = ((data != 0) * 2) - 1;
          //		printf("Sending a %d\n", phase);
        } else {
          if (data == 0) {
            phase *= -1;
            if ((ctr - smaller) > 0) {
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
    for (i = 1; i <= (10 * (headerLen + dataLen * payloads + rsFrames * parityLen) * samples); i++) // 572   
    {
      write_wave(ctr, buffer);
      if ((i % samples) == 0) {
        int symbol = (int)((i - 1) / (samples * 10));
        int bit = 10 - (i - symbol * samples * 10) / samples + 1;
        val = data10[symbol];
        data = val & 1 << (bit - 1);
        //		printf ("%d i: %d new frame %d data10[%d] = %x bit %d = %d \n",
        //	    		 ctr/SAMPLES, i, frames, symbol, val, bit, (data > 0) );
        if (mode == FSK) {
          phase = ((data != 0) * 2) - 1;
          //			printf("Sending a %d\n", phase);
        } else {
          if (data == 0) {
            phase *= -1;
            if ((ctr - smaller) > 0) {
              for (int j = 1; j <= smaller; j++)
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
  // int count;
  //  for (count = 0; count < dataLen; count++) {
  //      printf("%02X", b[count]);
  //  }
  //  printf("\n");

  // socket write

  if (!socket_open && transmit) {
    printf("Opening socket!\n");
 //   struct sockaddr_in address;
 //   int valread;
    struct sockaddr_in serv_addr;
    //    char *hello = "Hello from client"; 
    //    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("\n Socket creation error \n");
      error = 1;
    }

    memset( & serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if (inet_pton(AF_INET, "127.0.0.1", & serv_addr.sin_addr) <= 0) {
      printf("\nInvalid address/ Address not supported \n");
      error = 1;
    }

    if (connect(sock, (struct sockaddr * ) & serv_addr, sizeof(serv_addr)) < 0) {
      printf("\nConnection Failed \n");
      printf("Error: %s \n", strerror(errno));
      error = 1;
    }
    if (error == 1)
    ; //rpitxStatus = -1;
    else
      socket_open = 1;
  }

  if (!error && transmit) {
    //	digitalWrite (0, LOW);
 //   printf("Sending %d buffer bytes over socket after %d ms!\n", ctr, (long unsigned int)millis() - start);
    start = millis();
    int sock_ret = send(sock, buffer, (unsigned int)(ctr * 2 + 2), 0);
    printf("Millis6: %d Result of socket send: %d \n\n", (unsigned int)millis() - start, sock_ret);
    
    if (sock_ret < (ctr * 2 + 2)) {
  //    printf("Not resending\n");
      sleep(0.5);
      sock_ret = send(sock, &buffer[sock_ret], (unsigned int)(ctr * 2 + 2 - sock_ret), 0);
      printf("Millis7: %d Result of socket send: %d \n\n", millis() - start, sock_ret);
    }
    
    if (mode == BPSK)
    {	  
      start = millis();  // send frame a second time
      sock_ret = send(sock, buffer, (unsigned int)(ctr * 2 + 2), 0);
      printf("Millis8: %d Result of socket send: %d \n\n", (unsigned int)millis() - start, sock_ret);
    
      if (sock_ret < (ctr * 2 + 2)) {
  //    printf("Not resending\n");
        sleep(0.5);
        sock_ret = send(sock, &buffer[sock_ret], (unsigned int)(ctr * 2 + 2 - sock_ret), 0);
        printf("Millis9: %d Result of socket send: %d \n\n", millis() - start, sock_ret);
      }
    }

    if (sock_ret == -1) {
      printf("Error: %s \n", strerror(errno));
      socket_open = 0;
      //rpitxStatus = -1;
    }
  }
  if (!transmit) {
    fprintf(stderr, "\nNo CubeSatSim Band Pass Filter detected.  No transmissions after the CW ID.\n");
    fprintf(stderr, " See http://cubesatsim.org/wiki for info about building a CubeSatSim\n\n");
  }
  //    digitalWrite (0, HIGH);

//  if (mode == FSK)
    firstTime = 0;
//  else if (frames_sent > 0) //5)
//    firstTime = 0;

  return;
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

//	 static  int NOT_FRAME = /* 0fa */ 0xfa & 0x3ff;
//	 static  int FRAME = /* 0fa */ ~0xfa & 0x3ff;
	
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
				buffer[ctr++] = (short int)(0.1 * phase * (ctr - flip_ctr) / smaller);
			else
				buffer[ctr++] = (short int)(0.25 * amplitude * phase);
		}
		else
		{
			if ((ctr - flip_ctr) < smaller)
//  		 		buffer[ctr++] = (short int)(amplitude * 0.4 * phase * sin((float)(2*M_PI*i*freq_Hz/S_RATE)));	 	  		 		buffer[ctr++] = (short int)(amplitude * 0.4 * phase * sin((float)(2*M_PI*i*freq_Hz/S_RATE)));	
  		 		buffer[ctr++] = (short int)(phase * sin_map[ctr % sin_samples] / 2);
		else
//  		 		buffer[ctr++] = (short int)(amplitude * 0.4 * phase * sin((float)(2*M_PI*i*freq_Hz/S_RATE)));	 	 		 		buffer[ctr++] = (short int)(amplitude * phase * sin((float)(2*M_PI*i*freq_Hz/S_RATE)));	
  		 		buffer[ctr++] = (short int)(phase * sin_map[ctr % sin_samples]); 		 } 			
//		printf("%d %d \n", i, buffer[ctr - 1]);

}

int encodeA(short int  *b, int index, int val) {
//    printf("Encoding A\n");
    b[index] = val & 0xff;
    b[index + 1] = (short int) ((b[index + 1] & 0xf0) | ((val >> 8) & 0x0f));
    return 0;	
}

int encodeB(short int  *b, int index, int val) {
//    printf("Encoding B\n");
    b[index] =  (short int) ((b[index] & 0x0f)  |  ((val << 4) & 0xf0));
    b[index + 1] = (val >> 4 ) & 0xff;
    return 0;	
}

int twosToInt(int val,int len) {   // Convert twos compliment to integer
// from https://www.raspberrypi.org/forums/viewtopic.php?t=55815
	
      if(val & (1 << (len - 1)))
         val = val - (1 << len);

      return(val);
}

float rnd_float(double min,double max) {   // returns 2 decimal point random number
	
	int val = (rand() % ((int)(max*100) - (int)(min*100) + 1)) + (int)(min*100);
	float ret = ((float)(val)/100);
	
      return(ret);
}

int test_i2c_bus(int bus)
{
	int output = bus; // return bus number if OK, otherwise return -1
	char busDev[20] = "/dev/i2c-";
	char busS[5];
	snprintf(busS, 5, "%d", bus);  
	strcat (busDev, busS);	
	printf("I2C Bus Tested: %s \n", busDev);
	
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
    	 		printf("ERROR: %sd bus has a problem \n  Check I2C wiring and pullup resistors \n", busDev);
			output = -1;
    		}													
	} else
	{
    	 	printf("ERROR: %s bus has a problem \n  Check software to see if I2C enabled \n", busDev);
		output = -1; 
	}
	return(output);	// return bus number or -1 if there is a problem with the bus
}
