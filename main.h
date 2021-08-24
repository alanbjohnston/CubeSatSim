#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "afsk/status.h"
#include "afsk/ax5043.h"
#include "afsk/ax25.h"
#include "ax5043/spi/ax5043spi.h"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <time.h>
#include <math.h>
//#include "TelemEncoding.h"
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
#define XS4 17

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
extern int Encode_8b10b[][256];

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
int uart_fd;

int reset_count = 0;
float uptime_sec = 0;
long int uptime;
char call[5];
char sim_yes[10];

int bitRate, mode, bufLen, rsFrames, payloads, rsFrameLen, dataLen, headerLen, syncBits, syncWord, parityLen, samples, frameCnt, samplePeriod;
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

float voltage[9], current[9], sensor[19], other[3];
char sensor_payload[500];

int test_i2c_bus(int bus);

//const char pythonCmd[] = "python3 -u /home/pi/CubeSatSim/python/voltcurrent.py ";
const char pythonCmd[] = "python3 -u /home/pi/CubeSatSim/ina219.py ";
char pythonStr[100], pythonConfigStr[100], busStr[10];
int map[8] = {0, 1, 2, 3, 4, 5, 6, 7};
char src_addr[5] = "";
char dest_addr[5] = "APCSS";
float voltage_min[9], current_min[9], voltage_max[9], current_max[9], sensor_max[19], sensor_min[19], other_max[3], other_min[3];

