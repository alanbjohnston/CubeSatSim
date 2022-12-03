This code runs on the ESP32-CAM camera module

The board is ESP32 Arduino/AI Thinker ESP32-CAM

Select the USB Serial port

Compile and Upload

Open the Serial Monitor at 115200 bps

The red LED on the back of the camera will flash once when it takes a photo and flashes twice when it sends it over the serial port

Note: when you install the TJpg_Decoder library, you need to manually edit the libraries/TJpeg_Decoder/src/User_Config.h file and comment out this line:

#define TJPGD_LOAD_SD_LIBRARY

So that it is:

//#define TJPGD_LOAD_SD_LIBRARY

Otherwise, you will get an error about LittleFS when you try to compile.
