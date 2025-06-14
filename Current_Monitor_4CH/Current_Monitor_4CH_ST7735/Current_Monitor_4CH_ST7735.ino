/*
 Font draw speed and flicker test, draws all numbers 0-999 in each font
 (0-99 in font 8)
 Average time in milliseconds to draw a character is shown in red
 A total of 2890 characters are drawn in each font (190 in font 8)
 
 Needs fonts 2, 4, 6, 7 and 8

 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 Note that yield() or delay(0) must be called in long duration for/while
 loops to stop the ESP8266 watchdog triggering.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */


#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219_1(0x40);
Adafruit_INA219 ina219_2(0x41);

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

unsigned long drawTime = 0;

void setup(void) {

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219_1.begin()) {
    Serial.println("Failed to find INA219_1 chip");
    while (1) { delay(10); }
  }

  if (! ina219_2.begin()) {
    Serial.println("Failed to find INA219_2 chip");
    while (1) { delay(10); }
  }
  

  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  Serial.println("Measuring voltage and current with INA219 ...");


  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  //tft.drawString("1234567890123456", 0, 0, 2);
  //tft.drawString("CH1 (0x40)", 0, 0, 1);
  tft.drawString("mV", 28, 0, 1); 
  tft.drawString("mA", 64, 0, 1); 
  tft.drawString("mW", 108, 0, 1); 
  
  tft.drawString("1", 0, 20, 2);
  tft.drawString("2", 0, 40, 2);
  tft.drawString("3", 0, 60, 2);
  tft.drawString("4", 0, 80, 2);
  
}

void loop() {
  char buf[6];
  static uint32_t i;
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219_1.getShuntVoltage_mV();
  busvoltage = ina219_1.getBusVoltage_V();
  current_mA = ina219_1.getCurrent_mA();
  power_mW = ina219_1.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  sprintf(buf, "%4d ", int(busvoltage*1000));
  tft.drawString(buf, 12, 20, 2);
  sprintf(buf, "%4d ", int(current_mA));
  tft.drawString(buf, 48, 20, 2);
  
  sprintf(buf, "%5d", int(power_mW));
  tft.drawString(buf, 84, 20, 2);


  shuntvoltage = ina219_2.getShuntVoltage_mV();
  busvoltage = ina219_2.getBusVoltage_V();
  current_mA = ina219_2.getCurrent_mA();
  power_mW = ina219_2.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  sprintf(buf, "%4d ", int(busvoltage*1000));
  tft.drawString(buf, 12, 40, 2);
  sprintf(buf, "%4d ", int(current_mA));
  tft.drawString(buf, 48, 40, 2);
  
  sprintf(buf, "%5d", int(power_mW));
  tft.drawString(buf, 84, 40, 2);

  sprintf(buf, "%4d ", 9999);
  tft.drawString(buf, 12, 60, 2);
  sprintf(buf, "%4d ", 9999);
  tft.drawString(buf, 48, 60, 2);
  
  sprintf(buf, "%5d", 99999);
  tft.drawString(buf, 84, 60, 2);

  delay(500);
}
void loop1() {

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  drawTime = millis();

  for (int i = 0; i < 1000; i++) {
    tft.drawNumber(i, 0, 0, 1);
  }

  drawTime = millis() - drawTime;

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawFloat(drawTime / 2890.0, 3, 0, 80, 4);
 
  delay(4000);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  drawTime = millis();

  for (int i = 0; i < 1000; i++) {
    tft.drawNumber(i, 0, 0, 2);
  }

  drawTime = millis() - drawTime;

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawFloat(drawTime / 2890.0, 3, 0, 80, 4);

  delay(4000);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  drawTime = millis();

  for (int i = 0; i < 1000; i++) {
    tft.drawNumber(i, 0, 0, 4);
  }

  drawTime = millis() - drawTime;

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawFloat(drawTime / 2890.0, 3, 0, 80, 4);

  delay(4000);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  drawTime = millis();

  for (int i = 0; i < 1000; i++) {
    yield(); tft.drawNumber(i, 0, 0, 6);
  }

  drawTime = millis() - drawTime;

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawFloat(drawTime / 2890.0, 3, 0, 80, 4);
 
  delay(4000);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  drawTime = millis();

  for (int i = 0; i < 1000; i++) {
    yield(); tft.drawNumber(i, 0, 0, 7);
  }

  drawTime = millis() - drawTime;

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawFloat(drawTime / 2890.0, 3, 0, 80, 4);

  delay(4000);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  drawTime = millis();

  for (int i = 0; i < 100; i++) {
    yield(); tft.drawNumber(i, 0, 0, 8);
  }

  drawTime = millis() - drawTime;

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawFloat(drawTime / 190.0, 3, 0, 80, 4);

  delay(4000);
}
