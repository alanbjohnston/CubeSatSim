#include "esp32sstvpp.h"
#include "SPIFFS.h"
#include "esp_camera.h"
// jpg files to be stored in flash storage on Pico (FS 512kB setting)
#include "sstv1.h"
#include "sstv2.h"

static esp_err_t init_camera();
void config_camera();
void save_camera_image(char* filename);
static esp_err_t init_camera();
void load_files();

#define RX2_pin         IO16 // AMBE3000 RX to ESP32 TXD
#define TX2_pin         IO17 // AMBE3000 TX to ESP32 RXD
#define LED_PIN 33  // LED pin on ESP32-CAM-MB board is 33
#define FORMAT_SPIFFS_IF_FAILED true

camera_fb_t *pic;


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
  // put your setup code here, to run once:
    esp_log_level_set("*", ESP_LOG_VERBOSE); 
       pinMode(LED_PIN, OUTPUT); // Set the pin as output 
     digitalWrite(LED_PIN, HIGH); // Turn off            
  Serial.begin(115200);
  delay(5000);
  Serial.println("Running");
  picosstvpp_begin(3);

  if (init_camera() == ESP_OK)
 {
        
  config_camera();       
       
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
 }
	
	  
  SPIFFS.format();		
  
 deleteFile(SPIFFS, "/cam.jpg"); 
 deleteFile(SPIFFS, "/cam.bin");   
 listDir(SPIFFS, "/", 0); 
  
 load_files(); 
}

void loop() {
  // put your main code here, to run repeatedly:
 Serial.println("\nBlink three times");
    digitalWrite(LED_PIN, LOW); // Turn on
    delay (100); // Wait 0.1 sec
    digitalWrite(LED_PIN, HIGH); // Turn off
    delay(100);  // Wait 0.1 sec
    digitalWrite(LED_PIN, LOW); // Turn on
    delay (100); // Wait 0.1 sec
    digitalWrite(LED_PIN, HIGH); // Turn off       
     delay(100);  // Wait 0.1 sec
    digitalWrite(LED_PIN, LOW); // Turn on
    delay (100); // Wait 0.1 sec
    digitalWrite(LED_PIN, HIGH); // Turn off     
/*    
 if (init_camera() == ESP_OK)
 {
        
  config_camera();      
       
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
*/
// listDir(SPIFFS, "/", 0);
        
  char filename[] = "/cam.bin";
//  char filename2[] = "/cam.jpg";
  char sstv_image1[] = "/sstv_image_1_320_x_240.jpg";
  char sstv_image2[] = "/sstv_image_2_320_x_240.jpg";	
	
  deleteFile(SPIFFS, filename); 
//  deleteFile(SPIFFS, filename2); 
	
  jpeg_decode(sstv_image1, filename, true);
	
//  save_camera_image(filename);	
  
  listDir(SPIFFS, "/", 0);

  picosstvpp();

  delay(2000);
}

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

  .pixel_format = PIXFORMAT_RGB565, //888, //PIXFORMAT_JPEG, // Options: YUV422, GRAYSCALE, RGB565, JPEG
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
        
  s->set_brightness(s, 2);     // -2 to 2
  s->set_contrast(s, 0);       // -2 to 2
  s->set_saturation(s, 1);     // -2 to 2  
  s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
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

//  listDir(SPIFFS, "/", 0);
}


void print_hex(byte octet) {
      char hexValue[5];
      sprintf(hexValue, "%02X", octet);
      Serial.print(hexValue); 
}

void load_files() {
  SPIFFS.begin();
  File f;
	
 deleteFile(SPIFFS, "/sstv_image_1_320_x_240.jpg");	
 deleteFile(SPIFFS, "/sstv_image_2_320_x_240.jpg");	
	
  f = SPIFFS.open("/sstv_image_1_320_x_240.jpg", "r");	
//  if (f) {
  if (false) {	  
    Serial.println("Image sstv_image_1_320_x_240.jpg already in FS");
    f.close();
  } else {
    Serial.println("Loading image sstv_image_1_320_x_240.jpg into FS");
    f = SPIFFS.open("/sstv_image_1_320_x_240.jpg", FILE_WRITE);
    if (!f) Serial.println("Error opening");	  	  
    if (f.write(sstv_image_1_320_x_240, sizeof(sstv_image_1_320_x_240)) < sizeof(sstv_image_1_320_x_240)) {
       Serial.println("Loading image failed.");	     
       delay(2000);
    }
    f.close();
  }

  f = SPIFFS.open("/sstv_image_2_320_x_240.jpg", "r");
//  if (f) {
  if (false) {	  
    Serial.println("Image sstv_image_2_320_x_240.jpg already in FS");
    f.close();
  } else {
    Serial.println("Loading image sstv_image_2_320_x_240.jpg into FS");
    f = SPIFFS.open("/sstv_image_2_320_x_240.jpg", FILE_WRITE);
    if (!f) Serial.println("Error opening");		  
    if (f.write(sstv_image_2_320_x_240, sizeof(sstv_image_2_320_x_240)) < sizeof(sstv_image_2_320_x_240)) {
       Serial.println("Loading image failed");
       delay(2000);
    }
    f.close();
  }
	
  listDir(SPIFFS, "/", 0); 
}
