//#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
//#include <unistd.h>
#include <string.h>
//#include "afsk/status.h"
//#include "afsk/ax5043.h"
//#include "afsk/ax25.h"
//#include "ax5043/spi/ax5043spi.h"
//#include <wiringPiI2C.h>
//#include <wiringPi.h>
//#include <wiringSerial.h>
#include <time.h>
#include <math.h>
//#include "TelemEncoding.h"
//#include <sys/socket.h>
//#include <stdlib.h>
//#include <netinet/in.h>
#include <string.h>
//#include <arpa/inet.h>
//#include <errno.h>
#include "DumbTXSWS.h"
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <MPU6050_tockn.h>
#include <EEPROM.h>
#include <Arduino-APRS-Library.h>
#include <stdio.h>
#include "pico/stdlib.h"   // stdlib 
#include "hardware/irq.h"  // interrupts
#include "hardware/pwm.h"  // pwm 
#include "hardware/sync.h" // wait for interrupt 


// Pico GPIO pin assignments
#define LPF_PIN 8  // LPF is installed
#define PI_3V3_PIN 9  // 3.3V supply used to detect Pi Zero
#define MAIN_PB_PIN 10 // Main board PB pushbutton pin
#define TXC_PIN 11 // Transceiver Board is present
#define SWTX_PIN 14 // SR_FRS_05W Transmit Pico software serial port 
#define SQUELCH 15 // SR_FRS_05W Squelch out
#define MAIN_INA219 16 // Power the INA219s on the Main board
#define PTT_PIN 17 // SR_FRS_05W PTT Push to Talk - transmit
#define STEM_LED_GREEN 18 // STEM board LED1 Green
#define STEM_LED_BLUE 19 // STEM board LED2 Blue
#define MAIN_LED_GREEN 20 // Main board LED1 
#define MAIN_LED_BLUE 21 // Main board LED1 
#define PD_PIN 22 // SR_FRS_05W PD pin - enable
#define AUDIO_OUT_PIN 26 // SR_FRS_05W audio out pin
#define AUDIO_IN_PIN 27 // SR_FRS_05W audio in pin
#define TEMPERATURE_PIN 28  // Diode temperature analog pin

#define PORT 8080
#define FREQUENCY_OFFSET 0
#define TRUE 1
#define FALSE 0

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

//#define WAV_DATA_LENGTH (50000 * 8)

uint32_t tx_freq_hz = 434900000 + FREQUENCY_OFFSET;
uint8_t data[1024];
uint32_t tx_channel = 0;

//ax5043_conf_t hax5043;
//ax25_conf_t hax25;

int twosToInt(int val, int len);
float toAprsFormat(float input);
float rnd_float(double min, double max);
void get_tlm();
void get_tlm_fox();
int encodeA(short int * b, int index, int val);
int encodeB(short int * b, int index, int val);
void config_x25();
void trans_x25();
int upper_digit(int number);
int lower_digit(int number);
void update_rs(unsigned char parity[32], unsigned char c);
void write_little_endian(unsigned int word, int num_bytes, FILE *wav_file);
static int init_rf();
void test_radio();
void config_radio();
void send_packet();
void read_ina219();
void read_sensors();
void get_tlm_ao7();
void print_string(char *string);
void start_payload(); 
void led_set(int ledPin, bool state);
void blink(int length);
void blink_setup();
short eeprom_word_read(int addr);
void eeprom_word_write(int addr, int val);
void read_payload();
void start_ina219();
void pwm_interrupt_handler();
void start_pwm();
void transmit_on(); 
void transmit_off(); 
void config_telem();
void config_simulated_telem(); 
void generate_simualted_telem();
void process_pushbutton();
void blinkTimes(int blinks);
void blink_pin(int pin, int duration);
void config_gpio();

extern int Encode_8b10b[][256];

//int socket_open = 0;
//int sock = 0;
int loop_count = 0;
int firstTime = ON; // 0;
long start;
int testCount = 0;
long time_start;
//char cmdbuffer[1000];
//FILE * file1;//
#define BUFFER_SIZE  970 // (970 * 2) //  * 2)
short int buffer[BUFFER_SIZE]; // 50000]; //BUFFER_SIZE]; // ctr is an int // 100000]; // 50000]; // 25000]; // 10240]; // was 2336400]; // max size for 10 frames count of BPSK
//short int buffer[(WAV_DATA_LENGTH/8)];
//FILE *sopen(const char *program);

#define S_RATE	(200) // (8000) //(48000) // (44100)

#define AFSK 1
#define FSK 2
#define BPSK 3
#define SSTV 4
#define CW 5

int rpitxStatus = -1;

float amplitude; // = ; // 20000; // 32767/(10%amp+5%amp+100%amp)
float freq_Hz = 3000; // 1200
short int sin_samples;
short int sin_map[16];
int STEMBoardFailure = 1;

int smaller;
int flip_ctr = 0;
int phase = 1;
int ctr = 0;
int rd = 0;
int nrd;
void write_to_buffer(int i, int symbol, int val);
void write_wave(int i, short int * buffer);
void sleep(float time);
int uart_fd;

int reset_count = 0;
float uptime_sec = 0;
long int uptime = 0;
char call[5];
char sim_yes[10];

int mode = FSK;
int new_mode = FSK;
int bitRate, bufLen, rsFrames, payloads, rsFrameLen, dataLen, headerLen, syncBits, syncWord, parityLen, samples, frameCnt, samplePeriod;
float sleepTime;
unsigned int sampleTime = 0;
int frames_sent = 0;
int cw_id = ON;
int vB4 = FALSE, vB5 = FALSE, vB3 = FALSE, ax5043 = FALSE, transmit = FALSE, onLed, onLedOn, onLedOff, txLed, txLedOn, txLedOff, payload = OFF;
float voltageThreshold = 3.5, batteryVoltage = 4.5, batteryCurrent = 0, currentThreshold = 100;
float latitude = 39.027702f, longitude = -77.078064f;
float lat_file, long_file;
double cpuTemp;
int frameTime;

float axis[3], angle[3], volts_max[3], amps_max[3], batt, speed, period, tempS, temp_max, temp_min, eclipse;
int i2c_bus0 = OFF, i2c_bus1 = OFF, i2c_bus3 = OFF, camera = OFF, sim_mode = FALSE, SafeMode = FALSE, rxAntennaDeployed = 0, txAntennaDeployed = 0;
double eclipse_time;

float voltage[9], current[9], sensor[17], other[3];
char sensor_payload[500];

int test_i2c_bus(int bus);

//const char pythonCmd[] = "python3 -u /home/pi/CubeSatSim/python/voltcurrent.py ";
//const char pythonCmd[] = "python3 -u /home/pi/CubeSatSim/ina219.py ";
//char pythonStr[100], pythonConfigStr[100], busStr[10];
int mapping[8] = {0, 1, 2, 3, 4, 5, 6, 7};
char src_addr[5] = "";
char dest_addr[6] = "APCSS";
float voltage_min[9], current_min[9], voltage_max[9], current_max[9], sensor_max[17], sensor_min[17], other_max[3], other_min[3];

// Payload OK 

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
MPU6050 mpu6050(Wire);

long timer = 0;
int bmePresent;
int RXLED = 17;  // The RX LED has a defined Arduino pin
int greenLED = 19;
int blueLED = 18;
int Sensor1 = 0;
float Sensor2 = 0;
void eeprom_word_write(int addr, int val);
short eeprom_word_read(int addr);
int first_time = true;
int first_read = true;
float T2 = 26.3; // Temperature data point 1
float R2 = 167; // Reading data point 1
float T1 = 2; // Temperature data point 2
float R1 = 179; // Reading data point 2
int sensorValue;
float Temp;
float rest;

unsigned int wav_position = 0;
int pwm_counter = 0;
int pwm_counter_max = 420;
int pwm_amplitude = 50; //50 //100;
int pwm_value;
int pwm_rnd_bit = 1;
int delay_ms;

int ready = FALSE;

#define PRESSED 0
#define HELD 0
#define RELEASED 1
int pb_state = RELEASED;
int mode_count = 0;
unsigned long pb_press_start;


/*
 * TelemEncoding.h
 *
 *  Created on: Feb 3, 2014
 *      Author: fox
 */

#ifndef TELEMENCODING_H_
#define TELEMENCODING_H_

/*
void update_rs(
   unsigned char parity[32], // 32-byte encoder state; zero before each frame
   unsigned char c          // Current data byte to update
);
*/

#define CHARACTER_BITS 10
#define CHARACTERS_PER_LONGWORD 3
#define CHARACTER_MASK ((1<<CHARACTER_BITS)-1)
#define SYNC_CHARACTER -1


#define PARITY_BYTES_PER_CODEWORD 32U     // Number of parity symbols in frame
#define NP 32U //For Phil's code
#define DATA_BYTES_PER_CODE_WORD 223

#endif /* TELEMENCODING_H_ */

