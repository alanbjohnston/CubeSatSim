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
#define BAT2 3
#define MINUS_X 4
#define MINUS_Y 5
#define PLUS_Z 6
#define MINUS_Z 7
#define TEMP 2 // OK and BME280 is positions 0 and 1
#define PRES 3
#define ALT 4
#define HUMI 5
#define GYRO_X 7 // MPU6050 is posisition 6
#define GYRO_Y 8
#define GYRO_Z 9
#define ACCEL_X 10
#define ACCEL_Y 11
#define ACCEL_Z 12
#define GPS1 14 // GPS is position 13
#define GPS2 15
#define GPS3 16
#define DTEMP 18  // TMP is position 17
#define XS1 20 // NEW user defined token will be position 19
#define XS2 21  
#define XS3 22
#define SENSOR_FIELDS 26

#define RSSI 0
#define IHU_TEMP 2
#define SPIN 1

#define OFF - 1
#define ON 1
#define CHECK 0
#define DISABLED 0
#define CARRIER 1
#define DTMF_APRS 2

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
void program_radio();

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
FILE *telem_file;	

#define S_RATE	(48000) // (44100)

#define AFSK 1
#define FSK 2
#define BPSK 3
#define SSTV 4
#define CW 5
#define REPEATER 11
#define TXCOMMAND 12

int transmitStatus = -1;

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
char hab_yes[10];
int squelch = 3; // default squelch
char rx[12], tx[12];
int tx_pl = 0;
int rx_pl = 0;

int bitRate, mode, bufLen, rsFrames, payloads, rsFrameLen, dataLen, headerLen, syncBits, syncWord, parityLen, samples, frameCnt, samplePeriod;
float sleepTime;
unsigned int sampleTime = 0;
int frames_sent = 0;
int cw_id = ON;
int vB4 = FALSE, vB5 = FALSE, vB3 = FALSE, ax5043 = FALSE, transmit = FALSE, onLed, onLedOn, onLedOff, txLed, txLedOn, txLedOff, payload = OFF;
// float voltageThreshold = 3.6, batteryVoltage = 4.5, batteryCurrent = 0, currentThreshold = 100;
float voltageThreshold = 3.5, batteryVoltage = 4.5, batteryCurrent = 0, currentThreshold = 100;
float latitude = 39.027702f, longitude = -77.078064f;
float lat_file, long_file;
double cpuTemp;
int frameTime;
long int newGpsTime;

float axis[3], angle[3], volts_max[3], amps_max[3], batt, speed, period, tempS, temp_max, temp_min, eclipse;
int i2c_bus0 = OFF, i2c_bus1 = OFF, i2c_bus3 = OFF, camera = OFF, sim_mode = FALSE, SafeMode = FALSE;
int rxAntennaDeployed = 0, txAntennaDeployed = 0, c2cStatus = 0;
int sim_config = FALSE; // sim mode not set by configuration
double eclipse_time;

float voltage[9], current[9], sensor[SENSOR_FIELDS], other[3];
char sensor_payload[500];

int test_i2c_bus(int bus);

//const char pythonCmd[] = "python3 -u /home/pi/CubeSatSim/python/voltcurrent.py ";
const char pythonCmd[] = "python3 -u /home/pi/CubeSatSim/ina219.py ";
char pythonStr[100], pythonConfigStr[100], busStr[10];
int map[8] = {0, 1, 2, 3, 4, 5, 6, 7};
char src_addr[5] = "";
char dest_addr[5] = "APCSS";
float voltage_min[9], current_min[9], voltage_max[9], current_max[9], sensor_max[SENSOR_FIELDS], sensor_min[SENSOR_FIELDS], other_max[3], other_min[3];

int get_payload_serial(int debug_camera);
int finished = FALSE;

//char buffer2[100001];
char buffer2[2000];
int index1 = 0;
//char start_flag[] = "3d99de816e5ad7742b61a37c39141783";
char start_flag[] = "_START_FLAG_";

//char end_flag[] = "f681a5c52351befe0e3524eb1a40f14b7803317a";
char end_flag[] = "_END_FLAG_";

int flag_count = 0;
int start_flag_detected = FALSE;
int start_flag_complete = FALSE;
int end_flag_detected = FALSE;
int jpeg_start = 0;
#define CAMERA_TIMEOUT 2000 // 1500 // 2000 // 10000 // 20000  // Payload timeout in milli seconds

void battery_saver(int setting);
int battery_saver_check();
int pi_zero_2_offset = 0;


int hab_mode = FALSE; 
int battery_saver_mode = FALSE;
long int loopTime;

int error_count = 0;
int groundCommandCount = 0;
