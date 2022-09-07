//  ESP32-CAM JPEG over Serial
//
//  by Jim McLaughlin and Alan Johnston
//
//  Tested on ESP32-CAM-MB Camera Module hardware
//  with Arduino 1.8.19 using Board: ESP32 Arduino Selected: AI Thinker ESP32-CAM
//
//  example loop() program stores a JPEG and sends it over serial then waits 5 seconds.
//
//  Based on code example https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer
//
//  initialize_camera initializes the ESP32-CAM - only call once per boot!
//
//  save_camera_image takes a JPEG image and stores it with selected filename on
//       built-in SPIFFS storage on ESP32-CAM (no SD card is needed)
//
//  send_image_serial sends a selected filename from built-in storage over the serial
//       port at 115200 in binary format with a 30 byte start flag and 40 byte end flag
//       CRC-8 checksum is calculated over file and end flag and sent as the last byte.
//
//  listDir gives a directory file listing of the built-in file storage
//
#include <Arduino.h>
#include "esp_camera.h"
#include <FastCRC.h>
//#include "FS.h"
#include "SPIFFS.h"
#include <TJpg_Decoder.h>

// some info: https://visualgdb.com/tutorials/esp32/camera/

//#define DEBUG
#define FORMAT_SPIFFS_IF_FAILED true

//FONTS
const uint8_t b_fonts[43][11] = {
        {0x00, 0x18, 0x24, 0x62, 0x62, 0x62, 0x7E, 0x62, 0x62, 0x62, 0x00}, //00: A
        {0x00, 0x7C, 0x32, 0x32, 0x32, 0x3C, 0x32, 0x32, 0x32, 0x7C, 0x00}, //01: B
        {0x00, 0x3C, 0x62, 0x62, 0x60, 0x60, 0x60, 0x62, 0x62, 0x3C, 0x00}, //02: C
        {0x00, 0x7C, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x7C, 0x00}, //03: D
        {0x00, 0x7E, 0x60, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x7E, 0x00}, //04: E
        {0x00, 0x7E, 0x60, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x60, 0x00}, //05: F
        {0x00, 0x3C, 0x62, 0x62, 0x60, 0x60, 0x66, 0x62, 0x62, 0x3C, 0x00}, //06: G
        {0x00, 0x62, 0x62, 0x62, 0x62, 0x7E, 0x62, 0x62, 0x62, 0x62, 0x00}, //07: H
        {0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00}, //08: I
        {0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x4C, 0x4C, 0x4C, 0x38, 0x00}, //09: J
        {0x00, 0x62, 0x64, 0x68, 0x70, 0x68, 0x64, 0x62, 0x62, 0x62, 0x00}, //10: K
        {0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00}, //11: L
        {0x00, 0x42, 0x62, 0x76, 0x6A, 0x62, 0x62, 0x62, 0x62, 0x62, 0x00}, //12: M
        {0x00, 0x42, 0x62, 0x72, 0x6A, 0x66, 0x62, 0x62, 0x62, 0x62, 0x00}, //13: N
        {0x00, 0x3C, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x3C, 0x00}, //14: O
        {0x00, 0x7C, 0x62, 0x62, 0x62, 0x7C, 0x60, 0x60, 0x60, 0x60, 0x00}, //15: P
        {0x00, 0x3C, 0x62, 0x62, 0x62, 0x62, 0x62, 0x6A, 0x6A, 0x3C, 0x08}, //16: Q
        {0x00, 0x7C, 0x62, 0x62, 0x62, 0x7C, 0x68, 0x64, 0x62, 0x62, 0x00}, //17: R
        {0x00, 0x3C, 0x62, 0x60, 0x60, 0x3C, 0x06, 0x06, 0x46, 0x3C, 0x00}, //18: S
        {0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00}, //19: T
        {0x00, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x3C, 0x00}, //20: U
        {0x00, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x22, 0x14, 0x08, 0x00}, //21: V
        {0x00, 0x62, 0x62, 0x62, 0x62, 0x62, 0x6A, 0x76, 0x62, 0x42, 0x00}, //22: W
        {0x00, 0x42, 0x62, 0x74, 0x38, 0x1C, 0x2E, 0x46, 0x42, 0x42, 0x00}, //23: X
        {0x00, 0x42, 0x62, 0x74, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00}, //24: Y
        {0x00, 0x7E, 0x06, 0x0E, 0x0C, 0x18, 0x30, 0x70, 0x60, 0x7E, 0x00}, //25: Z
        {0x00, 0x3C, 0x62, 0x62, 0x66, 0x6A, 0x72, 0x62, 0x62, 0x3C, 0x00}, //26: 0
        {0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00}, //27: 1
        {0x00, 0x3C, 0x46, 0x06, 0x06, 0x1C, 0x20, 0x60, 0x60, 0x7E, 0x00}, //28: 2
        {0x00, 0x3C, 0x46, 0x06, 0x06, 0x1C, 0x06, 0x06, 0x46, 0x3C, 0x00}, //29: 3
        {0x00, 0x0C, 0x1C, 0x2C, 0x4C, 0x4C, 0x7E, 0x0C, 0x0C, 0x0C, 0x00}, //30: 4
        {0x00, 0x7E, 0x60, 0x60, 0x60, 0x7C, 0x06, 0x06, 0x46, 0x3C, 0x00}, //31: 5
        {0x00, 0x3C, 0x62, 0x60, 0x60, 0x7C, 0x62, 0x62, 0x62, 0x3C, 0x00}, //32: 6
        {0x00, 0x7E, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00}, //33: 7
        {0x00, 0x3C, 0x62, 0x62, 0x62, 0x3C, 0x62, 0x62, 0x62, 0x3C, 0x00}, //34: 8
        {0x00, 0x3C, 0x46, 0x46, 0x46, 0x3E, 0x06, 0x06, 0x46, 0x3C, 0x00}, //35: 9
        {0x00, 0x00, 0x02, 0x06, 0x0E, 0x1C, 0x38, 0x70, 0x60, 0x40, 0x00}, //36: /
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x00, 0x00, 0x00, 0x00}, //37: -
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00}, //38: .
        {0x00, 0x3C, 0x46, 0x06, 0x06, 0x0C, 0x10, 0x00, 0x30, 0x30, 0x00}, //39: ?
        {0x00, 0x18, 0x18, 0x18, 0x18, 0x10, 0x10, 0x00, 0x18, 0x18, 0x00}, //40: !
        {0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00}, //41: :
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  //42: space
};

// Nibble font table
const uint8_t l_fonts[23][5] = {
  { 0xE2, 0xA6, 0xA2, 0xA2, 0xE2 }, // 0: 01
  { 0xEE, 0x22, 0xE6, 0x82, 0xEE }, // 1: 23
  { 0xAE, 0xA8, 0xEE, 0x22, 0x2E }, // 2: 45
  { 0x8E, 0x82, 0xE2, 0xA2, 0xE2 }, // 3: 67
  { 0xEE, 0xAA, 0xEE, 0xA2, 0xE2 }, // 4: 89
  { 0x00, 0x22, 0x00, 0x22, 0x04 }, // 5: :;
  { 0x20, 0x4E, 0x80, 0x4E, 0x20 }, // 6: <=
  { 0x8E, 0x42, 0x26, 0x40, 0x84 }, // 7: >?
  { 0x64, 0x9A, 0xBE, 0x8A, 0x7A }, // 8: @A
  { 0xC6, 0xA8, 0xC8, 0xA8, 0xC6 }, // 9: BC
  { 0xCE, 0xA8, 0xAC, 0xA8, 0xCE }, // 10: DE
  { 0xE6, 0x88, 0xCE, 0x8A, 0x86 }, // 11: FG
  { 0xA4, 0xA4, 0xE4, 0xA4, 0xA4 }, // 12: HI
  { 0x69, 0x2A, 0x2C, 0x2A, 0x49 }, // 13: JK
  { 0x8A, 0x8E, 0x8E, 0x8A, 0xEA }, // 14: LM
  { 0x04, 0x9A, 0xDA, 0xBA, 0x94 }, // 15: NO
  { 0xC4, 0xAA, 0xCA, 0x8E, 0x86 }, // 16: PQ
  { 0xC6, 0xA8, 0xC4, 0xA2, 0xAC }, // 17: RS
  { 0xE0, 0x4A, 0x4A, 0x4A, 0x44 }, // 18: TU
  { 0x09, 0xA9, 0xA9, 0x6F, 0x26 }, // 19: vW (sort of..)
  { 0x0A, 0xAA, 0x46, 0xA2, 0x04 }, // 20: XY
  { 0xE6, 0x24, 0x44, 0x84, 0xE6 }, // 21: Z[
  { 0x00, 0x00, 0x00, 0x00, 0x00 }  // 22: SPACE
};

char input_buffer[240][240][3];

void app_main();
void send_image_serial(char *filename);
void save_camera_image(char* filename);
void initialize_camera();

static esp_err_t init_camera();
#define RX2_pin         IO16 // AMBE3000 RX to ESP32 TXD
#define TX2_pin         IO17 // AMBE3000 TX to ESP32 RXD
#define LED_PIN 33  // LED pin on ESP32-CAM-MB board is 33

const char start_flag[] = "3d99de816e5ad7742b61a37c39141783";  // MD5 hash of "ESP32-CAM" as start flag
const char end_flag[] = "f681a5c52351befe0e3524eb1a40f14b7803317a"; // SHA-1 hash of "ESP32-CAM" as end flag
FastCRC8 CRC8;
char hexValue[6];
char file_buf[50000];
int file_len;
bool file_flag = false;
camera_fb_t *pic;
File inFile;
File outFile;
int blocks = 0;
//int counter = 0;
bool write_complete = false;

// example code from https://github.com/espressif/arduino-esp32/blob/master/libraries/LittleFS/examples/LITTLEFS_test/LITTLEFS_test.ino
//
void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

void setup() {

  delay(5000);  

  Serial.begin(115200);

  initialize_camera();
        
  config_camera();      

  Serial.println("ESP32-CAM Camera initialized");

  pinMode(LED_PIN, OUTPUT); // Set the pin as output

  // On the ESP32 you can also define another hardware serial interface using pins of your choice like this
  // Serial2.begin(115200,SERIAL_8N1,14,15);

  //  Serial.println("Initializing CRC");

  //  crc.setPolynome(0x1021);

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  listDir(SPIFFS, "/", 0);

}

void loop() {
  
  char filename[] = "/cam.jpg";

  save_camera_image(filename);

  send_image_serial(filename);

  delay(500);
}

/**
   This example takes a picture every 5s and print its size on serial monitor.
*/

// =============================== SETUP ======================================

// 1. Board setup (Uncomment):
// #define BOARD_WROVER_KIT
// #define BOARD_ESP32CAM_AITHINKER

/**
   2. Kconfig setup

   If you have a Kconfig file, copy the content from
    https://github.com/espressif/esp32-camera/blob/master/Kconfig into it.
   In case you haven't, copy and paste this Kconfig file inside the src directory.
   This Kconfig file has definitions that allows more control over the camera and
   how it will be initialized.
*/

/**
   3. Enable PSRAM on sdkconfig:

   CONFIG_ESP32_SPIRAM_SUPPORT=y

   More info on
   https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html#config-esp32-spiram-support
*/

// ================================ CODE ======================================

//#include <esp_log.h>
//#include <esp_system.h>
//#include <nvs_flash.h>
//#include <sys/param.h>
//#include <string.h>

//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"

// ESP32Cam (AiThinker) PIN Map

#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1 //software reset will be performed
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27

#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22

//static const char *TAG = "example:take_picture";

static camera_config_t camera_config = {
  .pin_pwdn = CAM_PIN_PWDN,
  .pin_reset = CAM_PIN_RESET,
  .pin_xclk = CAM_PIN_XCLK,
  .pin_sscb_sda = CAM_PIN_SIOD,
  .pin_sscb_scl = CAM_PIN_SIOC,

  .pin_d7 = CAM_PIN_D7,
  .pin_d6 = CAM_PIN_D6,
  .pin_d5 = CAM_PIN_D5,
  .pin_d4 = CAM_PIN_D4,
  .pin_d3 = CAM_PIN_D3,
  .pin_d2 = CAM_PIN_D2,
  .pin_d1 = CAM_PIN_D1,
  .pin_d0 = CAM_PIN_D0,
  .pin_vsync = CAM_PIN_VSYNC,
  .pin_href = CAM_PIN_HREF,
  .pin_pclk = CAM_PIN_PCLK,

  //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
  .xclk_freq_hz = 20000000,
  .ledc_timer = LEDC_TIMER_0,
  .ledc_channel = LEDC_CHANNEL_0,

  .pixel_format = PIXFORMAT_JPEG, // Options: YUV422, GRAYSCALE, RGB565, JPEG
  .frame_size = FRAMESIZE_QVGA, // Options: QQVGA-UXGA, QVGA  Do not use sizes above QVGA when not JPEG

  .jpeg_quality = 6, // 12, //0-63 lower number means higher quality
  .fb_count = 1,       //if more than one, i2s runs in continuous mode. Use only with JPEG
  //    .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};

static esp_err_t init_camera()
{
  //initialize the camera
  esp_err_t err = esp_camera_init(&camera_config);
  if (err != ESP_OK)
  {
    //        ESP_LOGE(TAG, "Camera Init Failed");
    return err;
  }
   
        
  return ESP_OK;
}

void config_camera() {
 
  sensor_t * s = esp_camera_sensor_get();
        
//  s->set_brightness(s, 2);     // -2 to 2
//  s->set_contrast(s, 0);       // -2 to 2
//  s->set_saturation(s, 1);     // -2 to 2  
  s->set_hmirror(s, 1);        // 0 = disable , 1 = enable
//  s->set_vflip(s, 1);         
}

void initialize_camera() {
  if (ESP_OK != init_camera()) {
    Serial.println("Failed to initialize camera!");
    return;
  }
 }
 
void save_camera_image(char* filename)
{
  Serial.print("Saving camera image to file: ");
  Serial.println(filename);

  Serial.println("\nBlink...");
  digitalWrite(LED_PIN, LOW); // Turn on
  delay (100); // Wait 0.1 sec
  digitalWrite(LED_PIN, HIGH); // Turn off

  Serial.println("\nTaking picture...\n");

  pic = esp_camera_fb_get();

  const char path[] = "/cam.jpg";

  Serial.println("Writing image file"); //: %s\r\n", path);

  //   File file = SPIFFS.open(path, FILE_WRITE);
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }

  for (int k = 0; k < pic->len; k++) {
    file.write(pic->buf[k]);
  }
  file.close();

  Serial.println("Write complete");

  listDir(SPIFFS, "/", 0);
}

void send_image_serial(char *filename)
{

  Serial.println("Sending image over serial");
 
    Serial.println("\nBlink twice");
    digitalWrite(LED_PIN, LOW); // Turn on
    delay (100); // Wait 0.1 sec
    digitalWrite(LED_PIN, HIGH); // Turn off
    delay(100);  // Wait 0.1 sec
    digitalWrite(LED_PIN, LOW); // Turn on
    delay (100); // Wait 0.1 sec
    digitalWrite(LED_PIN, HIGH); // Turn off
  
// read from file

      const char path[] = "/cam.jpg";

      Serial.println("Reading image file"); //: %s\r\n", path);

//      File file = SPIFFS.open(path, FILE_READ);
      File file = SPIFFS.open(filename, FILE_READ);
      if (!file) {
        Serial.println("Failed to open file for reading");
        return;
      }
      int k = 0;
      while (file.available()) {
        //       file.write(pic->buf[size]);
        file_buf[k] = file.read();
        k++;
      }
      file.close();
      file_len = k;
      Serial.print(file_len);
      Serial.println(" bytes read from file.");

      file_flag = true;

//    }

 // Sending image file
    Serial.println("Start of binary data:");

    //      Serial.println("\nResetting CRC");
    //      crc.restart();

    int j = 0;
    // flag at start
    for (int i = 0; i < strlen(start_flag); i++) {
      sprintf(hexValue, "%02X", start_flag[i]);
      //        Serial.print(hexValue);
      Serial.write(start_flag[i]);
      //        Serial.print(start_flag[i], HEX);
    }

#ifdef DEBUG
#endif
    char str[] = "ESP32-CAM";
    if (file_flag == true) {
      //       Serial.println("******************* Sending stored file! ***************************\n");
      for (j = 0; j < file_len; j++) {
        ///      for (int i = 0; i < 9; i++) {
        uint8_t *data = (uint8_t *) &file_buf[j];
        //       crc.add(data[0]);
        //        crc.add(data[0]);
        ///        crc.add(str[i]);
#ifndef DEBUG
        if (file_buf[j] != 0x66)
          sprintf(hexValue, "%02X", file_buf[j]);
        else
          strcpy(hexValue, "66");
        Serial.write(file_buf[j]);
        //        Serial.print(hexValue);
        ///        Serial.write(str[j]);
#endif
        //        Serial.print(pic->buf[j], HEX);
#ifdef DEBUG
        //     char hexValue[5];
        sprintf(hexValue, "%02X", file_buf[j]);
        Serial.print(hexValue);
#endif
        //        Serial.print(" ");
      }
    } else {  // !file_flag - read from camera
      for (j = 0; j < pic->len; j++) {
        ///      for (int i = 0; i < 9; i++) {
        uint8_t *data = (uint8_t *) &pic->buf[j];
        //       crc.add(data[0]);
        //        crc.add(data[0]);
        ///        crc.add(str[i]);
        file_buf[j] = pic->buf[j];
#ifndef DEBUG
        sprintf(hexValue, "%02X", pic->buf[j]);
        Serial.write(pic->buf[j]);
        //        Serial.print(hexValue);
        ///        Serial.write(str[j]);
#endif
        //        Serial.print(pic->buf[j], HEX);
#ifdef DEBUG
        //     char hexValue[5];
        sprintf(hexValue, "%02X", pic->buf[j]);
        Serial.print(hexValue);
#endif
        //        Serial.print(" ");
      }
    }
    //    Serial.print("j = ");
    //    Serial.println(j);
    // flag at end
    for (int i = 0; i < strlen(end_flag); i++) {
      sprintf(hexValue, "%02X", end_flag[i]);
      //          Serial.print(hexValue);
      file_buf[j++] = end_flag[i];
      Serial.write(end_flag[i]);
      //         Serial.print(end_flag[i], HEX);
      //          if (file_flag) {
      //            file_buf[j++] = end_flag[i];
      //          } else {
      //            pic->buf[i++] = end_flag[j];
      //          }
    }

    uint8_t * data;
    int index1;

    data = (uint8_t *) &file_buf[0];
    index1 = j;

    //    int calculated_crc = crc8(data, index1, 0x07, 0x0000, 0x0000, false, false);

    int calculated_crc = CRC8.smbus(data, index1);
    //#ifndef DEBUG
    sprintf(hexValue, "%02X", calculated_crc);
    Serial.write(calculated_crc);
    //    Serial.print(hexValue);
    //   Serial.write(0xff & calculated_crc);
    //#endif
    //#ifdef DEBUG

    //     sprintf(hexValue, "%04X", calculated_crc);
    //     Serial.print(hexValue);
    //     Serial.println("\n ");
    //#endif

    Serial.println("\nEnd of binary data.");

    Serial.println("\nCRC cacluation data:");
    Serial.println(file_buf[0], HEX);
    Serial.println(file_buf[j - 1], HEX);
    Serial.println(j);

    Serial.print("\nCRC sent with image: ");
    //     char hexValue[6];
    sprintf(hexValue, "%02X", calculated_crc);
    Serial.print(hexValue);

    Serial.println("\n");

    /*  remove hex print
          for (int i = 0; i < pic->len; i++) {
      //        Serial.write(pic->buf[i]);
            Serial.print(pic->buf[i], HEX);
      //        Serial.print(" ");
          }
          Serial.println(" ");
    */
    // use pic->buf to access the image
    ESP_LOGI(TAG, "Picture taken! Its size was: %zu bytes", pic->len);
    // format of the captured picture will be stored in the format field of the
    // structure returned by esp_camera_fb_get()

    Serial.print("image size ");
    if (file_flag)
      Serial.println(file_len);
    else {
      Serial.println(pic->len);
      // must free the memory allocated by esp_camera_fb_get()
      esp_camera_fb_return(pic);
    }
  Serial.println("File sent!");
}

void print_hex(byte octet) {
      char hexValue[5];
      sprintf(hexValue, "%02X", octet);
      Serial.print(hexValue); 
}

char img_block[320][8][3];   // 320 pixels per row, 8 rows, 3 values (RGB) per.
  
bool get_block(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
#ifdef DEBUG  
  Serial.println("\nBlock callback");
  Serial.println(x);
  Serial.println(y);
  Serial.println(w);
  Serial.println(h);
#endif
  
//  return 1;

  uint16_t pixel_value;
  uint16_t *pixel;
  bool last_block = ((x == (320 - w)) & (y == (240 - h)));
//  char buffer[16 * 8 * 3];
  int counter = 0;
  //int x_block = (x / w) % w;
  //int y_block = (y / h) % h;
  
/*  
  if (((y % h) == 0) && ((x % w) == 0)) {
    Serial.print("\nStart of row! x = ");
    Serial.print(x);
    Serial.print(" y = ");
    Serial.println(y);
  }
*/
  pixel = bitmap;
  uint32_t total_pixels = w * h;

//  while (total_pixels--) {
    while (counter < total_pixels) {
    pixel_value = *pixel;
    
    int y_rel = counter / w;
    int x_rel = counter % w; //  - y_rel * w;
/*  
    Serial.print("Relative x + x = ");
    Serial.print(x_rel + x);
    Serial.print(" y + y = ");
    Serial.print(y_rel);
    Serial.print(" counter = ");
    Serial.println(counter);
*/
  /*
    if ((x == 0) && (y == 0)) {
      Serial.print(" ");
      Serial.print(pixel_value, HEX);
      Serial.print(" ");
    }
*/    
//    buffer[counter++] = pixel_value >> 8;
//    buffer[counter++] = pixel_value;

    byte red = (pixel_value & 0b1111100000000000) >> 8;
    byte green = (pixel_value & 0b0000011111100000) >> 3;
    byte blue = (pixel_value & 0b0000000000011111) << 3;
    
//    buffer[counter++] = red;
//    buffer[counter++] = green;
//    buffer[counter++] = blue;
    
    img_block[x_rel + x][y_rel][0] = red;
    img_block[x_rel + x][y_rel][1] = green;
    img_block[x_rel + x][y_rel][2] = blue;  
  
#ifdef DEBUG   
    Serial.print("\npixel_value: ");  
    Serial.print(pixel_value);  
    Serial.print("\nRGB: ");  
    print_hex(red);
    print_hex(green);
    print_hex(blue);
      
    Serial.println("\n img_block: ");
    for (int k = 0; k < 3; k ++)  
      print_hex(*(&img_block[x_rel + x][y_rel][0] + k));      
#endif    
 /*
    if (counter >= 155000) {
      Serial.println("Resetting counter****************************************\n");
      counter = 0;
    }
*/
    counter++;
    pixel++;
  }
  
//  Serial.println("\nWriting block to file");
//  Serial.print("Sizeof buffer: ");
//  Serial.println(sizeof(buffer));
  if (x == 304) {
    if (outFile) {
      Serial.println("********************************************* Writing block!");
      outFile.write((const uint8_t*)&img_block[0][0][0], sizeof(img_block));  
    } else
      Serial.println("Problem writing block");
    counter = 0;
  }
  if (last_block) {
    Serial.println("Complete!\n\n");
  }
/*
    for (int i = 0; i < counter; i++) {
//      Serial.print(buffer[i], HEX);
      char hexValue[5];
      sprintf(hexValue, "%02X", buffer[i]);
      Serial.print(hexValue);
    }
 */ 
    
//    Serial.print("\n\n Size: ");
//    Serial.println(counter);
    
//    write_complete = true;
//  }

//  delay(1000);

  blocks++;

  return 1;
}
 
int JpegDec_i;
int JpegDec_j;
int JpegDec_height = 240;
int JpegDec_width = 320;
byte  JpegDec_sortBuf[15360]; //320(px)*16(lines)*3(bytes) // Header buffer
int JpegDec_pxSkip;
uint8_t *JpegDec_pImg;
int JpegDec_x, JpegDec_y, JpegDec_bx, JpegDec_by;
int JpegDec_comps = 3;
  
bool merged_get_block(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
#ifdef DEBUG
  Serial.println("\nBlock callback");
  Serial.println(x);
  Serial.println(y);
  Serial.println(w);
  Serial.println(h);
#endif
  int JpegDec_MCUx = x;
  int JpegDec_MCUy = y;
  int JpegDec_MCUHeight = h;
  int JpegDec_MCUWidth = w;

//  JpegDec_pImg = bitmap;
  
//    JpegDec_pImg = JpegDec_pImage;
    for(JpegDec_by=0; JpegDec_by<JpegDec_MCUHeight; JpegDec_by++){
      for(JpegDec_bx=0; JpegDec_bx<JpegDec_MCUWidth; JpegDec_bx++){
//        JpegDec_x = JpegDec_MCUx * JpegDec_MCUWidth + JpegDec_bx;
//        JpegDec_y = JpegDec_MCUy * JpegDec_MCUHeight + JpegDec_by;
        JpegDec_x = JpegDec_MCUx + JpegDec_bx;
        JpegDec_y = JpegDec_MCUy + JpegDec_by;
        if(JpegDec_x<JpegDec_width && JpegDec_y<JpegDec_height){
//          if(JpegDec_comps == 1){ // Grayscale
//            //sprintf(str,"%u", pImg[0]);
//            outFile.write(JpegDec_pImg, 1);
//          }else
        { // RGB
            // When saving to the SD, write 16 lines on one time
            // First we write on the array 16 lines and then we save to SD
            JpegDec_pxSkip = ((JpegDec_y - (16 * JpegDec_j)) * 320) + JpegDec_x;
          
            int pixel_value = *bitmap;
            
            byte red = (pixel_value & 0b1111100000000000) >> 8;
            byte green = (pixel_value & 0b0000011111100000) >> 3;
            byte blue = (pixel_value & 0b0000000000011111) << 3;            
            
            JpegDec_sortBuf[(3 * JpegDec_pxSkip) + 0] = red;  // JpegDec_pImg[0];
            JpegDec_sortBuf[(3 * JpegDec_pxSkip) + 1] = green; // JpegDec_pImg[1];
            JpegDec_sortBuf[(3 * JpegDec_pxSkip) + 2] = blue; // JpegDec_pImg[2];
#ifdef DEBUG          
            Serial.print("sortBuf index = ");
            Serial.println((3 * JpegDec_pxSkip));
#endif
            JpegDec_i++;
            if(JpegDec_i == 5120){ //320(px)x16(lines)
#ifdef DEBUG
              Serial.println("Writing lines!");
#endif
              //              for(k = 0; k < 15360; k++){
//                imgFile.write(sortBuf[k]);
//              }
              outFile.write(JpegDec_sortBuf, sizeof(JpegDec_sortBuf));
              JpegDec_i = 0;
              JpegDec_j++; //15(sections)
            }
          }
        }
//        JpegDec_pImg += JpegDec_comps ;
        bitmap++;
      }
    }
#ifdef DEBUG  
  Serial.println("Block processed!");
#endif
  return 1;
}

void jpeg_decode(char* filename, char* fileout, bool debug){
  uint8_t *pImg;
//  uint16_t *pImg;
  int x,y, bx,by;
  byte sortBuf[15360]; //320(px)*16(lines)*3(bytes) // Header buffer
  int i,j,k;
  int pxSkip;
  
  // Open the file for writing
//  File imgFile = SD.open(fileout, FILE_WRITE);
  outFile = SPIFFS.open(fileout, "w+");
  
  if (outFile) {
    if (debug)
      Serial.println("Output opened");
  }
  else
    Serial.println("Failed to open output");

 /*  
  for(i = 0; i < 15360; i++){ // Cleaning Header Buffer array
    sortBuf[i] = 0xFF;
  }

  for(i = 0; i < 12; i++){
    byte fontNumber;
    char ch;
    ch = charId[i];
    for(y = 0; y < 11; y++){
      for(x = 0; x < 8; x++){
        pxSkip = 16 + (320 * (y + 3)) + (3 * 8 * i) + (3 * x); //Width: x3

        uint8_t mask;
        mask = pow(2, 7 - x);

        if(ch >= 65 && ch <= 90){ // A to Z
                fontNumber = ch - 65;
        }
        else if(ch >= 48 && ch <= 57){ //0 to 9
                fontNumber = ch - 22;
        }
        else if(ch == '/'){fontNumber = 36;}
        else if(ch == '-'){fontNumber = 37;}
        else if(ch == '.'){fontNumber = 38;}
        else if(ch == '?'){fontNumber = 39;}
        else if(ch == '!'){fontNumber = 40;}
        else if(ch == ':'){fontNumber = 41;}
        else if(ch == ' '){fontNumber = 42;}
        else              {fontNumber = 42;}

        if((b_fonts[fontNumber][y] & mask) != 0){
          for(j = 0; j < 9; j++){
                  sortBuf[(3 * pxSkip) + j] = 0x00;
          }
        }
      }
    }
  }

//  for(k = 0; k < 15360; k++){  // Adding header to the binary file
//    imgFile.write(sortBuf[k]);
//  }
  outFile.write(sortBuf, sizeof(sortBuf));
*/
        
//  writeFooter(&outFile);  //Writing first 10560 bytes (11*320*3)  // write footer after rotate
  
  // Decoding start
  
  if (debug)
    Serial.println("Starting jpeg decode");
  
  JpegDec_i = 0;
  JpegDec_j = 0;
  
  uint16_t w = 0, h = 0;
  // TJpgDec.getFsJpgSize(&w, &h, "/cam.jpg", LittleFS); // Note name preceded with "/"
  TJpgDec.getFsJpgSize(&w, &h, filename, SPIFFS); // Note name preceded with "/"
  
  if (debug) {
    Serial.print("Width = "); 
    Serial.print(w); 
    Serial.print(", height = "); 
    Serial.println(h);
  }
  
  if ((w == 0) && (h == 0)) {
    Serial.println("Failed to open jpeg input");
    return;
  }
//  counter = 0;
//  write_complete = false;
  
  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(false);    // was true
//  TJpgDec.setCallback(get_block);  
  TJpgDec.setCallback(merged_get_block);  
  //TJpgDec.drawFsJpg(0, 0, "/cam.jpg", LittleFS);
  TJpgDec.drawFsJpg(0, 0, filename, SPIFFS);
  
  if (debug)
    Serial.println("Draw complete");
  
//  while (!write_complete) { Serial.println("Waiting..."); delay(500);}

/*  
  JpegDec.decodeFile(filename);
  // Image Information
  Serial.print("Width     :");
  Serial.println(JpegDec.width);
  Serial.print("Height    :");
  Serial.println(JpegDec.height);
  Serial.print("Components:");
  Serial.println(JpegDec.comps);
  Serial.print("MCU / row :");
  Serial.println(JpegDec.MCUSPerRow);
  Serial.print("MCU / col :");
  Serial.println(JpegDec.MCUSPerCol);
  Serial.print("Scan type :");
  Serial.println(JpegDec.scanType);
  Serial.print("MCU width :");
  Serial.println(JpegDec.MCUWidth);
  Serial.print("MCU height:");
  Serial.println(JpegDec.MCUHeight);
  Serial.println("");
*/
  if (debug)
    Serial.println("Writing bin to FS");

//  imgFile.write(JpegDec.pImage, sizeof(JpegDec.pImage));
  
/*  
  i = 0;
  j = 0;
  while(JpegDec.read()){
    pImg = JpegDec.pImage ;
    for(by=0; by<JpegDec.MCUHeight; by++){
      for(bx=0; bx<JpegDec.MCUWidth; bx++){
        x = JpegDec.MCUx * JpegDec.MCUWidth + bx;
        y = JpegDec.MCUy * JpegDec.MCUHeight + by;
        if(x<JpegDec.width && y<JpegDec.height){
          if(JpegDec.comps == 1){ // Grayscale
            //sprintf(str,"%u", pImg[0]);
            imgFile.write(pImg, sizeof(pImg));
          }else{ // RGB
            // When saving to the SD, write 16 lines on one time
            // First we write on the array 16 lines and then we save to SD
            pxSkip = ((y - (16 * j)) * 320) + x;
            sortBuf[(3 * pxSkip) + 0] = pImg[0];
            sortBuf[(3 * pxSkip) + 1] = pImg[1];
            sortBuf[(3 * pxSkip) + 2] = pImg[2];
            i++;
            if(i == 5120){ //320(px)x16(lines)
//              for(k = 0; k < 15360; k++){
                imgFile.write(sortBuf, sizeof(sortBuf));
//              }
              i = 0;
              j++; //15(sections)
            }
          }
        }
        pImg += JpegDec.comps ;
      }
    }
  }
*/
  if (debug)
    Serial.println("Bin has been written to FS");
  outFile.close();
}

/*  
//void shot_pic(){
void get_pic() {
  int32_t start_time = millis();  
  
  
  time = millis() - start_time;
  Serial.println("get_pic done!");
  Serial.print(start_time); 
  Serial.println(" ms elapsed");
  
  return; 
}
  
  // Try to locate the camera
  if (cam.begin()) {
    Serial.println("Camera Found:");
  } else {
    Serial.println("No camera found?");
    return;
  }
  for (int i = 0; i <= 10; i++){
    cam.setImageSize(VC0706_320x240);
  }
  Serial.println("Snap in 3 secs...");
  delay(3000);
  if (! cam.takePicture())
    Serial.println("Failed to snap!");
  else
    Serial.println("Picture taken!");
  // Create an image with the name IMAGExx.JPG`
  strcpy(pic_filename, "IMAGE00.JPG");
  for (int i = 0; i < 100; i++) {
    pic_filename[5] = '0' + i/10;
    pic_filename[6] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(pic_filename)) {
      break;
    }
  }
  // Open the file for writing
  File imgFile = SD.open(pic_filename, FILE_WRITE);
  // Get the size of the image (frame) taken
  uint16_t jpglen = cam.frameLength();
  Serial.print("Storing ");
  Serial.print(jpglen, DEC);
  Serial.print(" byte image.");
  int32_t time = millis();
  pinMode(8, OUTPUT);
  // Read all the data up to # bytes!
  byte wCount = 0; // For counting # of writes
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min(32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = cam.readPicture(bytesToRead);
    imgFile.write(buffer, bytesToRead);
    if(++wCount >= 64) { // Every 2K, give a little feedback so it doesn't appear locked up
      Serial.print('.');
      wCount = 0;
    }
    //Serial.print("Read ");  Serial.print(bytesToRead, DEC); Serial.println(" bytes");
    jpglen -= bytesToRead;
  }
  imgFile.close();
  time = millis() - time;
  Serial.println("done!");
  Serial.print(time); Serial.println(" ms elapsed");
  
*/  
//}

/**     Write on a file with 11 lines the values of the GPS
 * @param dst Given an opened File stream then write data to dst.
 * @param latitude Floating point latitude value in degrees/min as received from the GPS (DDMM.MMMM)
 * @param lat N/S
 * @param longitude Floating point longitude value in degrees/min as received from the GPS (DDMM.MMMM)
 * @param lon E/W
 * @param altitude Altitude in meters above MSL
 */

/*
//void writeFooter(File* dst, nmea_float_t latitude, char lat, nmea_float_t longitude, char lon, nmea_float_t altitude){    //Write 16 lines with values
void writeFooter(File* dst){
  int x,y;
  byte sortBuf[10560]; //320(px)*11(lines)*3(bytes) // Header buffer
  int i,j,k;
  int pxSkip;
  char res[51] = "LAT: 1234.1234N     LONG: 1234.1234W     ALT:10000";
  for(i = 0; i < 10560; i++){ // Cleaning Header Buffer array
    sortBuf[i] = 0xFF;
  }
  for(i = 0; i < sizeof(res); i++){
    byte fontNumber;
    char ch;
    ch = res[i];
    for(y = 0; y < 5; y++){
      for(x = 0; x < 4; x++){
        //pxSkip = HORIZONTALOFFSET + VERSTICALOFFSET + (BITSPERWORD * i);
        //pxSkip = 16 + (320 * (y + 3)) + (4 * 2 * i) + (2 * x); Width: x2
        pxSkip = 16 + (320 * (y + 3)) + (4 * i) + x;
        // If ch is pair mask is: 11110000, if no 00001111
        uint8_t sl = (ch % 2)? 3 : 7 ;
        uint8_t mask = pow(2, sl - x);
        if(ch >= 48 && ch <=91){
          fontNumber = (ch-48)/2;
        }
        else {
          fontNumber = 22;
        }
        if((l_fonts[fontNumber][y] & mask) != 0){
          for(j = 0; j < 3; j++){
                  sortBuf[(3 * pxSkip) + j] = 0x00;
          }
        }
      }
    }
  }
//  for(k = 0; k < 10560; k++){  // Adding header to the binary file
//    dst->write(sortBuf[k]);
    dst->write(sortBuf, sizeof(sortBuf));
//  }
}
*/

void raw_decode(char* filename, char* fileout){  // used to decode .raw files in RGB565 format

// Open the input file for reading
  inFile = SPIFFS.open(filename, "r");
  
  if (inFile)
    Serial.println("Input opened");
  else {
    Serial.println("Failed to open input");
    return;
  }
// Open the output file for writing
  outFile = SPIFFS.open(fileout, "w+");
  
  if (outFile)
    Serial.println("Output opened");
  else {
    Serial.println("Failed to open output");
    return;
  }
  char buff[2];
  char buffer[3];
  
  int i = 0;
//  int redx = 128;
//  int greenx = 128;
//  int bluex = 128;
  
//  while (i++ < (320 * 240 * 3)) {
  while (i++ < (320 * 240 * 1.49)) {
    inFile.readBytes(buff, 2);
    
#ifdef DEBUG    
    print_hex(buff[0]);
    print_hex(buff[1]);
#endif
    
    int pixel_value = (buff[0] << 8) + buff[1];  // endian for raw
 
    byte red = (pixel_value & 0b1111100000000000) >> 8;
    byte green = (pixel_value & 0b0000011111100000) >> 3;
    byte blue = (pixel_value & 0b0000000000011111) << 3;

#ifdef TEST_PATTERN    
    int size = 5; // 46;
    int y = (int)( i / 320 );
    int x = (int)( i - y * 320 );
    int box = (int)(x/size) + (int)(y/size);  
 
    if (y < 10) { // 20) {
      red = 0;
      green = 255;
      blue = 0;
    }   
    else if ( box == ( (int)(box / 2) * 2)) {
//      Serial.println(x);
//      Serial.println(y);
//      Serial.println(box);
//      Serial.println(" ");
      red = 255; //(100 + x) % 256;
      green = 0; // ;
      blue = 0;    
    } else  {
//      Serial.println(x);
//      Serial.println(y);
//      Serial.println(box);
//      Serial.println(" ");
      red = 0;
      green = 0;
      blue = 255; //(100 + y) % 256;
    }  
#endif    
    
      buffer[0] = red;
      buffer[1] = green;
      buffer[2] = blue;    
/*     
    if (y < 20) { // 20) {
      buffer[0] = 0;
      buffer[1] = 255;
      buffer[2] = 0;
    } else {
    
      buffer[0] = redx;
      buffer[1] = greenx;
      buffer[2] = bluex;
    }
 */
    
    int bytes = outFile.write((const uint8_t*)buffer, 3);
//    Serial.println(bytes);
    if (bytes < 3) 
      Serial.println("Error writing output file");
    
  #ifdef DEBUG    
    print_hex(red);
    print_hex(green);
    print_hex(blue);
    
//    delay(100);
  #endif    
  }
  inFile.close();
  outFile.close();
}

/*  
//void writeFooter(File* dst, nmea_float_t latitude, char lat, nmea_float_t longitude, char lon, nmea_float_t altitude){    //Write 16 lines with values
void writeFooter(File* dst, char *telemetry){
  int x,y;
  byte sortBuf[10560]; //320(px)*11(lines)*3(bytes) // Header buffer
  int i,j,k;
  int pxSkip;

  char res[51]; //  = "LAT: 1234.1234N     LONG: 1234.1234W     ALT:10000";
  
  if (strlen(telemetry) > 50)
    telemetry[50] = '\0';
  strcpy(res, telemetry);

  for(i = 0; i < 10560; i++){ // Cleaning Header Buffer array
    sortBuf[i] = 0xFF;
  }

  for(i = 0; i < sizeof(res); i++){
    byte fontNumber;
    char ch;
    ch = res[i];
    for(y = 0; y < 5; y++){
      for(x = 0; x < 4; x++){
        //pxSkip = HORIZONTALOFFSET + VERSTICALOFFSET + (BITSPERWORD * i);
        //pxSkip = 16 + (320 * (y + 3)) + (4 * 2 * i) + (2 * x); Width: x2
        pxSkip = 16 + (320 * (y + 3)) + (4 * i) + x;

        // If ch is pair mask is: 11110000, if no 00001111
        uint8_t sl = (ch % 2)? 3 : 7 ;
        uint8_t mask = pow(2, sl - x);

        if(ch >= 48 && ch <=91){
          fontNumber = (ch-48)/2;
        }
        else {
          fontNumber = 22;
        }

        if((l_fonts[fontNumber][y] & mask) != 0){
          for(j = 0; j < 3; j++){
                  sortBuf[(3 * pxSkip) + j] = 0x00;
          }
        }
      }
    }
  }

  for(k = 0; k < 10560; k++){  // Adding header to the binary file
    dst->write(sortBuf[k]);
  }
}  

*/
  
void rotate_image(char *file_input, char *file_output, char *telemetry) {
  
  File input_file = SPIFFS.open(file_input, "r");           
  
  char pixel[3];
  int side = (320 - 240)/2;
  for (int y = 0; y < 240; y++) {
    for (int x = 0; x < 320; x++) {
      input_file.readBytes(pixel, sizeof(pixel));
      if (( x >= side) && (x < (320 - side))) {
        input_buffer[y][x - side][0] = pixel[0];
        input_buffer[y][x - side][1] = pixel[1];        
        input_buffer[y][x - side][2] = pixel[2];      
      }
    }
  }
  input_file.close();
  
  SPIFFS.remove(file_input);
  
  Serial.println("Input file read and deleted");
  Serial.println(side);
  
  input_file = SPIFFS.open(file_input, "w+"); 
  
  //writeFooter(&input_file, telemetry); 
  
  char side_pixel[] = { 0xff, 0xff, 0xff };
  for (int y = 0; y < 240; y++) {
    Serial.println(" ");
    for (int x = 0; x < 320; x++) {
      if (( x >= side) && (x < (320 - side))) {
        Serial.print("+");
//        Serial.print(x - side);
//        Serial.print(" ");
        pixel[0] = input_buffer[x - side][y][0];
        pixel[1] = input_buffer[x - side][y][1];
        pixel[2] = input_buffer[x - side][y][2];       
        if (input_file.write((const uint8_t*)pixel, sizeof(pixel)) < 3)
          Serial.println("Error writing to file");
      } else {
        Serial.print("-");
        if (input_file.write((const uint8_t*)side_pixel, sizeof(side_pixel)) < 3)
          Serial.println("Error writing to file");         
      } 
    }
  }
  
  input_file.close();
}
