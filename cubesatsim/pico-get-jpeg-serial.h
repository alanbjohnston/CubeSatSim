#ifndef PICO_GET_JPEG_SERIAL
#define PICO_GET_JPEG_SERIAL

#include <LittleFS.h>
#include <FastCRC.h>

bool finished = false;

//char buffer[100001];
int index1 = 0;
char start_flag[] = "3d99de816e5ad7742b61a37c39141783";
char end_flag[] = "f681a5c52351befe0e3524eb1a40f14b7803317a";
int flag_count = 0;
int start_flag_detected = false;
int start_flag_complete = false;
int end_flag_detected = false;
int jpeg_start = 0;
FastCRC8 CRC8;

//#define DEBUG

//#define PICOW true
int led_pin = LED_BUILTIN;

void get_image_file();
#endif
