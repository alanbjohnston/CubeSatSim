/*
 *  Pico Payload code for the v1.3 and later STEM Payload Board for the CubeSatSim
 *
 *  Copyright Alan B. Johnston
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  (at your option) any later version.
 *F
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// This code is an Arduino sketch for the Raspberry Pi Pico
// based on the Raspberry Pi Code

//#define PICO_0V1 // define for Pico v0.1 hardware

#include "payload_pico.h"
#include "DumbTXSWS.h"
#include <Wire.h>
//#include <Adafruit_INA219.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//#include <MPU6050_tockn.h>
#include <EEPROM.h>
//#include <Arduino-APRS-Library.h>
#include <stdio.h>
#include "pico/stdlib.h"   // stdlib 
//#include "hardware/irq.h"  // interrupts
//#include "hardware/pwm.h"  // pwm 
//#include "hardware/sync.h" // wait for interrupt 
//#include "RPi_Pico_ISR_Timer.h"
//#include "RPi_Pico_TimerInterrupt.h"
#include <WiFi.h>
#include "hardware/gpio.h"
#include "hardware/adc.h"
//#include "SSTV-Arduino-Scottie1-Library.h"
#include "LittleFS.h"
//#include <Adafruit_SI5351_Library.h>
//#include "picosstvpp.h"
#include "pico/bootrom.h"
#include "hardware/watchdog.h"
#include <MQTT.h>
#include <TinyGPS++.h>

// jpg files to be stored in flash storage on Pico (FS 512kB setting)
//#include "sstv1.h"
//#include "sstv2.h"

//Adafruit_INA219 ina219_1_0x40;
//Adafruit_INA219 ina219_1_0x41(0x41);
//Adafruit_INA219 ina219_1_0x44(0x44);
//Adafruit_INA219 ina219_1_0x45(0x45);
//Adafruit_INA219 ina219_2_0x40(0x40);
//Adafruit_INA219 ina219_2_0x41(0x41);
//Adafruit_INA219 ina219_2_0x44(0x44);
//Adafruit_INA219 ina219_2_0x45(0x45);

//Adafruit_SI5351 clockgen = Adafruit_SI5351();
TinyGPSPlus gps;

unsigned long micros3;
bool show_gps = false;
volatile int skip = 0;

//WiFiServer server(port);
//WiFiClient client;
WiFiClient net;
MQTTClient client;

#define PICO_W    // define if Pico W board.  Otherwise, compilation fail for Pico or runtime fail if compile as Pico W

//#define APRS_VHF

byte green_led_counter = 0;
//char call[] = "AMSAT";   // put your callsign here

//extern bool get_camera_image(bool debug);
//extern bool start_camera();

float rand_float(float lower, float upper) {

  return (float)(random(lower*100, upper*100)/100.0);	
}

void setup() {

  set_sys_clock_khz(133000, true);  
	
  Serial.begin(115200);
	
  delay(10000);	
	
  LittleFS.begin();
//  LittleFS.format();	// only format if files of size 0 keep showing up
//#ifdef APRS_VHF	  
//   mode = AFSK;	// force to APRS	
//#else	
//  read_mode();
//#endif
	
//  new_mode = mode;
	
//  pinMode(LED_BUILTIN, OUTPUT);	
//  blinkTimes(1);	

///  sleep(5.0);	
	
// otherwise, run CubeSatSim Pico code
  
  Serial.println("CubeSatSim Pico Payload v0.2 starting...\n");
	
/**/	
  if (check_for_wifi()) {
     wifi = true;
     led_builtin_pin = LED_BUILTIN; // use default GPIO for Pico W	  
     pinMode(LED_BUILTIN, OUTPUT);		  
//     configure_wifi();	  
  }  else  {
     led_builtin_pin = 25; // manually set GPIO 25 for Pico board	  
//     pinMode(25, OUTPUT);
     pinMode(led_builtin_pin, OUTPUT);		  
  }
/**/		
	
  config_gpio();

  Serial.print("Pi Zero present, so running Payload OK code.");
  sr_frs_present = true;	 
  program_radio();  
  start_payload();
	
  pinMode(15, INPUT_PULLUP);
  pinMode(22, OUTPUT);
  digitalWrite(22, 1);
  pinMode(17, OUTPUT);
  digitalWrite(17, 1);

  prompt = false;
  prompting = false;
  
}

void loop() {

  payload_OK_only();
  sleep(1.0);
  Serial.print("Squelch: ");	
  Serial.println(digitalRead(15));
//  Serial.println(" ");	
  prompt_for_input();	
}

void config_gpio() {
	
  // set all Pico GPIO connected pins to input	
  for (int i = 6; i < 22; i++) { 
      pinMode(i, INPUT);	  
  }
  pinMode(26, INPUT);	
  pinMode(27, INPUT);	
  pinMode(28, INPUT);		
}	

void program_radio() {
 if (sr_frs_present) {	
  Serial.println("Programming SR_FRS!");	

  pinMode(PD_PIN, OUTPUT);
  pinMode(PTT_PIN, OUTPUT);
  digitalWrite(PD_PIN, HIGH);  // enable SR_FRS
  digitalWrite(PTT_PIN, HIGH);  // stop transmit	
	
  DumbTXSWS mySerial(SWTX_PIN); // TX pin
  mySerial.begin(9600);
    
  for (int i = 0; i < 5; i++) {
     sleep(0.5); // delay(500);
#ifdef APRS_VHF	  
     mySerial.println("AT+DMOSETGROUP=0,144.3900,144.3900,0,3,0,0\r");    // can change to 144.39 for standard APRS	  
//    mySerial.println("AT+DMOSETGROUP=0,145.0000,145.0000,0,3,0,0\r");    // can change to 145 for testing ASPRS	  
#else
     mySerial.println("AT+DMOSETGROUP=0,435.0000,434.9000,0,3,0,0\r");   // squelch set to 3
#endif	  
   sleep(0.5);	  
   mySerial.println("AT+DMOSETMIC=8,0\r");  // was 8
	
  }
 }
#ifdef APRS_VHF	  	
 Serial.println("Programming FM tx 144.39, rx on 144.39 MHz");
#else
 Serial.println("Programming FM tx 434.9, rx on 435.0 MHz");
#endif	
//  digitalWrite(PTT_PIN, LOW);  // transmit carrier for 0.5 sec
//  sleep(0.5);
//  digitalWrite(PTT_PIN, HIGH);	
  digitalWrite(PD_PIN, LOW);  // disable SR_FRS	
  pinMode(PD_PIN, INPUT);
  pinMode(PTT_PIN, INPUT);	
}

bool read_config_file() {
  char buff[255];
  // Open configuration file with callsign and reset count
  Serial.println("Reading config file");	
  File config_file = LittleFS.open("/payload.cfg", "r");	
//  FILE * config_file = fopen("/payload.cfg", "r");
  if (!config_file) {
    return(false);  
  }

//  char * cfg_buf[100];
  config_file.read((uint8_t *)buff, 255);
//  sscanf(buff, "%d", &cnt);	
  sscanf(buff, "%f %f %f", xOffset, yOffset, zOffset);
  config_file.close();
//  if (debug_mode)	
    Serial.printf("Config file /payload.cfg contains %f %f %f\n", xOffset, yOffset, zOffset);
	
  config_file.close();	
	
//  write_config_file();	

  return(true);	
}

void write_config_file() {
  Serial.println("Writing /payload.cfg file");	
  char buff[255];	
  File config_file = LittleFS.open("/payload.cfg", "w+");		  
	  
  sprintf(buff, "%f %f %f", xOffset, yOffset, zOffset);
  Serial.println("Writing string ");	
//  if (debug_mode)	
    print_string(buff);	
  config_file.write(buff, strlen(buff));	  
	  
  config_file.close();
//  Serial.println("Write complete");	
	
}

void read_sensors()
{
	
}	

void print_string(char *string)
{
  int count = 0;
  while ((count < 250) && (string[count] != 0))
  {
    Serial.print(string[count++]);	  
  }
  Serial.println(" ");	
}

void start_payload() {

#ifdef APRS_VHF	  
// Serial2.setRX(9);	
// Serial2.setRX(9);		
//   Serial2.setRX(1);
   delay(100);
// Serial2.setTX(8);
// Serial2.setTX(8);	
//   Serial2.setRX(0);
	
/*  	
   delay(100);
   Serial1.setRX(1);
   delay(100);
   Serial1.setTX(0);
   delay(10);
*/	
   Serial1.begin(115200); // serial to Pi
	
   Serial.println("Starting Serial1 for payload");
	
   Serial2.begin(115200);  // serial from GPS	
	
   Serial.println("Starting Serial2 for GPS");	
#else	
   Serial1.setRX(1);
   delay(100);
   Serial1.setTX(0);
   delay(100);		
  Serial1.begin(115200);  // Pi UART faster speed
#endif	

  Serial.println("Starting payload!");
  
  blink_setup();
/*
  blink(500);
  sleep(0.25); // delay(250);
  blink(500);
  sleep(0.25); // delay(250);
  led_set(greenLED, HIGH);
  sleep(0.25); // delay(250);
  led_set(greenLED, LOW);
  led_set(blueLED, HIGH);
  sleep(0.25); // delay(250);
  led_set(blueLED, LOW);
*/ 
	
  if (bme.begin()) {
    bmePresent = 1;
  } else {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    bmePresent = 0;
  }
	
  Wire.begin();
  Wire.beginTransmission(0x68);
  if (Wire.endTransmission() != 0)  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    mpuPresent = 0;
  }
  else {
    mpuPresent = 1;
    mpu6050.begin();	  


//  if ((read_config_file()) && (payload_command != PAYLOAD_RESET))
  if (false)	  
  {
    Serial.println("Reading gyro offsets from config file\n");
    mpu6050.setGyroOffsets(xOffset, yOffset, zOffset);
	  	  
  }
	  
  else
  {
    payload_command = false;
	  
    Serial.println("Calculating gyro offsets and storing in EEPROM\n");

    mpu6050.calcGyroOffsets(true);
/*
    eeprom_word_write(0, 0xA07);
    eeprom_word_write(1, (int)(mpu6050.getGyroXoffset() * 100.0) + 0.5);
    eeprom_word_write(2, (int)(mpu6050.getGyroYoffset() * 100.0) + 0.5);
    eeprom_word_write(3, (int)(mpu6050.getGyroZoffset() * 100.0) + 0.5);
*/
//    flag = 0xA07;
    xOffset = mpu6050.getGyroXoffset();
    yOffset = mpu6050.getGyroYoffset();
    zOffset = mpu6050.getGyroZoffset();
	  
    write_config_file();	  
  }
  }		

  if (!(payload = bmePresent || mpuPresent))
    Serial.println("No payload sensors detected");
	
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
	
}

void payload_OK_only()
{
  payload_str[0] = '\0';  // clear the payload string
	
  if ((Serial.available() > 0)|| first_time == true)   // commented back in
  {
    blink(50);
    char result = Serial.read();
    char header[] = "OK BME280 ";
    char str[100];
	  
    strcpy(payload_str, header);
//    print_string(str);		  
    if (bmePresent) 
//    	sprintf(str, "%4.2f %6.2f %6.2f %5.2f ", 
   	sprintf(str, "%.1f %.2f %.1f %.2f ", 
	  bme.readTemperature(), bme.readPressure() / 100.0, bme.readAltitude(SEALEVELPRESSURE_HPA), bme.readHumidity());
    else
        sprintf(str, "OK BME280 0.0 0.0 0.0 0.0 "); 
    strcat(payload_str, str);

    if (mpuPresent) 	 { 
//    print_string(payload_str);	
    mpu6050.update();

//    sprintf(str, " MPU6050 %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f ",
    sprintf(str, " MPU6050 %.1f %.1f %.1f %.1f %.1f %.1f ",
      mpu6050.getGyroX(), mpu6050.getGyroY(), mpu6050.getGyroZ(), mpu6050.getAccX(), mpu6050.getAccY(), mpu6050.getAccZ()); 
    
    strcat(payload_str, str);
//    print_string(payload_str);
    }
    if (result == 'R') {
      Serial.println("OK");
      delay(100);
//      first_time = true;
      start_payload();	    
//      setup();
    }
    else if (result == 'g') {
	show_gps = !show_gps;    
    }	  
    else if (result == 'C') {
      Serial.println("Clearing stored gyro offsets in EEPROM\n");
      EEPROM.put(0, (float)0.0);
//      first_time = true;
      start_payload(); 	    
//      setup();
    }
    if ((result == '?') || first_time == true)  // commented back in
    if (true)
    {
	    	       
//      first_time = false;
      if (bmePresent) {
        Serial.print("OK BME280 ");
        Serial.print(bme.readTemperature());
        Serial.print(" ");
        Serial.print(bme.readPressure() / 100.0F);
        Serial.print(" ");
        Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
        Serial.print(" ");
        Serial.print(bme.readHumidity());
      } else
      {
        Serial.print("OK BME280 0.0 0.0 0.0 0.0");
      }
    if (mpuPresent) 	 { 
      mpu6050.update();

      Serial.print(" MPU6050 ");
      Serial.print(mpu6050.getGyroX());
      Serial.print(" ");
      Serial.print(mpu6050.getGyroY());
      Serial.print(" ");
      Serial.print(mpu6050.getGyroZ());

    Serial.print(" ");
    Serial.print(mpu6050.getAccX());   
    Serial.print(" ");
    Serial.print(mpu6050.getAccY());   
    Serial.print(" ");
    Serial.print(mpu6050.getAccZ());  
    } else
      {
        Serial.print(" MPU6050 0.0 0.0 0.0 0.0 0.0 0.0");
      }
	    
    sensorValue = analogRead(TEMPERATURE_PIN);
    //Serial.println(sensorValue);  
    Temp = T1 + (sensorValue - R1) *((T2 - T1)/(R2 - R1));

    
    Serial.print(" XS2 ");
    Serial.print(Sensor1,4);   
    Serial.print(" ");
    Serial.print(Sensor2,4);              
    Serial.print(" ");
    Serial.print(Sensor3,2);         
    Serial.print(" MQ ");
    Serial.println(sensorValue); // ,0);  	    

  if (mpuPresent) 	 { 	    
    float rotation = sqrt(mpu6050.getGyroX()*mpu6050.getGyroX() + mpu6050.getGyroY()*mpu6050.getGyroY() + mpu6050.getGyroZ()*mpu6050.getGyroZ()); 
    float acceleration = sqrt(mpu6050.getAccX()*mpu6050.getAccX() + mpu6050.getAccY()*mpu6050.getAccY() + mpu6050.getAccZ()*mpu6050.getAccZ()); 
//    Serial.print(rotation);
//    Serial.print(" ");
//    Serial.println(acceleration);
 
    if (acceleration > 1.2)
        led_set(greenLED, HIGH);
    else
        led_set(greenLED, LOW);
        
    if (rotation > 5)
        led_set(blueLED, HIGH);
    else
        led_set(blueLED, LOW);  
    } 
   }    
  }
	
//  Serial2.print("b");	
  delay(250);	

//  if (Serial2.available() > 0) {
  if (true) {
/*	  
    while (Serial2.available() > 0) // read GPS
      Serial.write(Serial2.read());
*/	  
// For one second we parse GPS data and report some key values
  newData = false;
	  
  unsigned long starting = millis();	  
  for (unsigned long start = millis(); millis() - start < 1000;) // 5000;)
  {  
    while (Serial2.available())
    {
      char c = Serial2.read();
      if (show_gps)	    	    
        Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }	  
    if (newData)
  {
    Serial.printf("GPS read new data in ms: %d\n", millis() - start);	    
    float flon, flat, flalt;
    unsigned long age;
    starting = millis();	    
//    gps.f_get_position(&flat, &flon, &age);
	    
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
	  
    flat = gps.location.lat();	  
    flon = gps.location.lng();	  
    flalt = gps.altitude.meters();	  
  }
  else
  {
    Serial.print(F("INVALID"));	  
  }
  Serial.print("\r\n");   
	    
    Sensor1 = flat;
    Sensor2 = flon;
    Sensor3 = flalt; // (float) gps.altitude.meters();
//    Serial.printf("New GPS data: %f %f %f  ms: %d\n", Sensor1, Sensor2, Sensor3, millis() - start);	    
  }    else
//	    Serial.printf("GPS read no new data: %d\n", millis() - start);	      
	;
  
    blink(50);
/*	  
    if (Serial1.available()) {	  
      char result = Serial1.read();
//    Serial1.println(result);
//      Serial.println(result);  // don't print read result
    
      if (result == 'R') {
        Serial1.println("OK");
        delay(100);
        first_read = true;
        start_payload(); 	    
//      setup();
      }
    }	
*/	  
//    if (result == '?')
    if (true)  // always send payload data over serial
    {
      if (bmePresent) {
//        Serial1.print("START_FLAGOK BME280 ");
        Serial1.print(sensor_start_flag);
        Serial1.print("OK BME280 ");
	Serial1.print(bme.readTemperature());
        Serial1.print(" ");
        Serial1.print(bme.readPressure() / 100.0F);
        Serial1.print(" ");
        Serial1.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
        Serial1.print(" ");
        Serial1.print(bme.readHumidity());
      } else
      {
//        Serial1.print("START_FLAGOK BME280 0.0 0.0 0.0 0.0");
        Serial1.print(sensor_start_flag);
        Serial1.print("OK BME280 0.0 0.0 0.0 0.0");	      
      }
      if (mpuPresent) 	 {     
      mpu6050.update();

      Serial1.print(" MPU6050 ");
      Serial1.print(mpu6050.getGyroX());
      Serial1.print(" ");
      Serial1.print(mpu6050.getGyroY());
      Serial1.print(" ");
      Serial1.print(mpu6050.getGyroZ());

    Serial1.print(" ");
    Serial1.print(mpu6050.getAccX());   
    Serial1.print(" ");
    Serial1.print(mpu6050.getAccY());   
    Serial1.print(" ");
    Serial1.print(mpu6050.getAccZ());   

      }	 else
      {
        Serial1.print(" MPU6050 0.0 0.0 0.0 0.0 0.0 0.0");
      }    

    sensorValue = analogRead(TEMPERATURE_PIN);
    //Serial.println(sensorValue);  
    Temp = T1 + (sensorValue - R1) *((T2 - T1)/(R2 - R1));

    Serial1.print(" XS2 ");
    Serial1.print(Sensor1,4);   
    Serial1.print(" ");
    Serial1.print(Sensor2,4);              
    Serial1.print(" ");
    Serial1.print(Sensor3,2);    
    Serial1.print(" MQ ");
    Serial1.print(sensorValue); //,0);  
//    Serial1.println("END_FLAG");  	    
    Serial1.println(sensor_end_flag);  	    
	    
    blink(50);
    delay(50);
    blink(50);
	    
    if (mpuPresent) 	 {     
    float rotation = sqrt(mpu6050.getGyroX()*mpu6050.getGyroX() + mpu6050.getGyroY()*mpu6050.getGyroY() + mpu6050.getGyroZ()*mpu6050.getGyroZ()); 
    float acceleration = sqrt(mpu6050.getAccX()*mpu6050.getAccX() + mpu6050.getAccY()*mpu6050.getAccY() + mpu6050.getAccZ()*mpu6050.getAccZ()); 
//    Serial.print(rotation);
//    Serial.print(" ");
//    Serial.println(acceleration);

    if (first_read == true) {
      first_read = false;
      rest = acceleration;
    }

    if (acceleration > 1.2 * rest)
        led_set(greenLED, HIGH);
    else
        led_set(greenLED, LOW);
        
    if (rotation > 5)
        led_set(blueLED, HIGH);
    else
        led_set(blueLED, LOW);
    }
    }
  }

  delay(100);
}

/**/	
/*
void eeprom_word_write(int addr, int val)
{
  EEPROM.write(addr * 2, lowByte(val));
  EEPROM.write(addr * 2 + 1, highByte(val));
}

short eeprom_word_read(int addr)
{
  return ((EEPROM.read(addr * 2 + 1) << 8) | EEPROM.read(addr * 2));
}
*/

void blink_setup() 
{
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)  
  // initialize digital pin PB1 as an output.
  pinMode(PC13, OUTPUT);
  pinMode(PB9, OUTPUT);
  pinMode(PB8, OUTPUT);
#endif

#if defined __AVR_ATmega32U4__ || ARDUINO_ARCH_RP2040
  pinMode(RXLED, OUTPUT);  // Set RX LED as an output
  // TX LED is set as an output behind the scenes
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED,OUTPUT);
#endif
}

void blink(int length)
{
  if (wifi)	
    digitalWrite(LED_BUILTIN, HIGH);   // set the built-in LED ON
  else
    digitalWrite(led_builtin_pin, HIGH);   // set the built-in LED ON
	  
  sleep(length/1000.0); // delay(length);              // wait for a lenth of time

  if (wifi)	
    digitalWrite(LED_BUILTIN, LOW);   // set the built-in LED OFF
  else
    digitalWrite(led_builtin_pin, LOW);   // set the built-in LED OFF
}

void led_set(int ledPin, bool state)
{
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)
  if (ledPin == greenLED)
    digitalWrite(PB9, state);
  else if (ledPin == blueLED)
    digitalWrite(PB8, state);    
#endif

#if defined __AVR_ATmega32U4__ || ARDUINO_ARCH_RP2040
  digitalWrite(ledPin, state);   
#endif  
}


void sleep(float timer) {  // sleeps for intervals more than 0.01 milli seconds

  unsigned long time_us = (unsigned long)(timer * 1000000.0);	
  unsigned long startSleep = micros();	    
  while ((micros() - startSleep) < time_us)  {	  
//    busy_wait_us(100);
    delayMicroseconds(100);  
  }
}

void blinkTimes(int blinks) {
  for (int i = 0; i < blinks; i++) {
    digitalWrite(MAIN_LED_GREEN, LOW);
	  
    if (wifi)	
      digitalWrite(LED_BUILTIN, LOW);   // set the built-in LED OFF
    else
      digitalWrite(led_builtin_pin, LOW);   // set the built-in LED OFF
	  
    sleep(0.1);
    digitalWrite(MAIN_LED_GREEN, HIGH);
	  
    if (wifi)	
      digitalWrite(LED_BUILTIN, HIGH);   // set the built-in LED ON
    else
      digitalWrite(led_builtin_pin, HIGH);   // set the built-in LED ON	  
	  
    sleep(0.1);
  }
}

void blinkFastTimes(int blinks) {
  for (int i = 0; i < blinks; i++) {
    digitalWrite(MAIN_LED_GREEN, LOW);
	  
    if (wifi)	
      digitalWrite(LED_BUILTIN, LOW);   // set the built-in LED OFF
    else
      digitalWrite(led_builtin_pin, LOW);   // set the built-in LED OFF
	  
    sleep(0.05);
    digitalWrite(MAIN_LED_GREEN, HIGH);
	  
    if (wifi)	
      digitalWrite(LED_BUILTIN, HIGH);   // set the built-in LED ON
    else
      digitalWrite(led_builtin_pin, HIGH);   // set the built-in LED ON	  
	  
    sleep(0.05);
  }
}

void blink_pin(int pin, int duration) {
	
  digitalWrite(pin, HIGH);
  sleep((float)duration / 1000.00);
  digitalWrite(pin, LOW);	
	
}


bool check_for_wifi() {
	
#ifndef PICO_W
	
  Serial.println("WiFi disabled in software");
  return(false);  // skip check if not Pico W board or compilation will fail
	
#endif
	
//     stdio_init_all();

//   adc_init();
//   adc_gpio_init(29);
  pinMode(29, INPUT);	
//   adc_select_input(3);
   const float conversion_factor = 3.3f / (1 << 12);
//   uint16_t result = adc_read();
   uint16_t result = analogRead(29);
//   Serial.printf("ADC3 value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);

//  if (result < 0x100) {	
  if (result < 0x10) {
    Serial.println("\nPico W detected!\n");
    return(true);
  }
  else {
     Serial.println("\nPico detected!\n");
     return(false);  
  }
}

void show_dir() {
  LittleFS.begin();
  Dir dir = LittleFS.openDir("/");
// or Dir dir = LittleFS.openDir("/data");
  Serial.println("FS directory:");
  while (dir.next()) {
    Serial.print(dir.fileName());
    if(dir.fileSize()) {
        File f = dir.openFile("r");
        Serial.print(" ");
        Serial.println(f.size());
    }
  }
  Serial.println(">");
}

void serial_input() {
	
  if (prompt == false) {  // only query if not in the middle of prompting
 	  
  if (Serial.available() > 0) {  // check for user input on serial port
 
//    blink(50);
    char result = Serial.read();

    if ((result != '\n') && (result != '\r')) {
	    
    Serial.println(result);

    switch(result) {
     case 'h':
     case 'H':
 //      Serial.println("Help");	     
        prompt = PROMPT_HELP;		    
       break;
		   
     case 'a':
     case 'A':
       Serial.println("Change to AFSK/APRS mode");	     
       new_mode = AFSK;
       break;	
		   
     case 'm':
     case 'M':
       Serial.println("Change to CW mode");	     
       new_mode = CW;
       break;	
		   
     case 'F':
       Serial.println("Formatting flash memory");	     
       prompt = PROMPT_FORMAT;
	break;
     case 'f':
      Serial.println("Change to FSK mode");
       new_mode = FSK;	    
       break;	
		   
     case 'b':
     case 'B':
       Serial.println("Change to BPSK mode");	     
       new_mode = BPSK;
       break;	
		   
     case 's':
       Serial.println("Change to SSTV mode");	     
       new_mode = SSTV;
       break;	

     case 'S':
       Serial.println("I2C scan");	     
       prompt = PROMPT_I2CSCAN;		
       break;	
   
     case 'i':
     case 'I':
       Serial.println("Restart CubeSatsim software");	     
       prompt = PROMPT_RESTART;
       break;	
		   
     case 'c':
       Serial.println("Change the CALLSIGN");	
       prompt = PROMPT_CALLSIGN;	    
       break;	
		    
     case 'C':
       Serial.println("Debug camera");	
       debug_camera = true;
       prompt = PROMPT_CAMERA;	    
       break;			    
		   
     case 't':
     case 'T':
       Serial.println("Change the Simulated Telemetry");	     
       prompt = PROMPT_SIM;
       break;	
		   
     case 'p':
     case 'P':
       Serial.println("Reset payload EEPROM settings");	
       prompt = PROMPT_PAYLOAD;
       break;	
		   
     case 'r':
     case 'R':
       Serial.println("Change the Resets Count");	
       prompt = PROMPT_RESET;
       break;	
		   
     case 'o':
     case 'O':
       Serial.println("Read diode temperature");	
       prompt = PROMPT_TEMP;
       break;	
		    
     case 'l':
     case 'L':
      Serial.println("Change the Latitude and Longitude");	     
      prompt = PROMPT_LAT;
      break;	
		   
     case 'v':
     case 'V':
      Serial.println("Read INA219 voltage and current");	     
      prompt = PROMPT_VOLTAGE;
      break;	
		    
     case '?':
       Serial.println("Query payload sensors");	     
       prompt = PROMPT_QUERY;
       break;	
		    
     case 'd':
       Serial.println("Change debug mode");	     
       prompt = PROMPT_DEBUG;
       break;			   

     case 'w':
       Serial.println(wifi);	
       Serial.println("Connect to WiFi");    
       prompt = PROMPT_WIFI;
       break;	
		    
     default:
       Serial.println("Not a command\n");	
		   
       break;
    }	    
   }
 }
 }
}

void prompt_for_input() {	
 float float_result;

 if (!prompting) {	
  prompting = true;	 
	
  while (Serial.available() > 0)  // clear any characters in serial input buffer
    Serial.read();	  
	
  switch(prompt) {
		  
    case PROMPT_HELP:		  
       Serial.println("\nChange settings by typing the letter:");	     
       Serial.println("h  Help info");	  
       Serial.println("F  Format flash memory");
       Serial.println("S  I2C scan");	   
       Serial.println("i  Restart");	     
       Serial.println("p  Reset payload and stored EEPROM values");	
       Serial.println("?  Query sensors");	
       Serial.println("o  Read diode temperature");	
       Serial.println("d  Change debug mode");
       Serial.println("w  Connect to WiFi\n");
		  
//       Serial.printf("Software version v0.39 \nConfig file /payload.cfg contains %s %d %f %f %s\n\n", callsign, reset_count, lat_file, long_file, sim_yes);
/*		  
       switch(mode) {
		       
	case(AFSK):
	  Serial.println("AFSK mode");
	  break;
		       
	case(FSK):
	  Serial.println("FSK mode");
	  break;	
		       
	case(BPSK):
	  Serial.println("BPSK mode");
	  break;
		       
	case(SSTV):
	  Serial.println("SSTV mode");
	  break;	
		       
	case(CW):
	  Serial.println("CW mode");
	  break;			       
       }
*/		  
       break;	

    case PROMPT_REBOOT:
       Serial.println("Rebooting...");
       Serial.flush();	  
       watchdog_reboot (0, SRAM_END, 500);	 // restart Pico
       sleep(20.0);			  
       break;
		  
    case PROMPT_FORMAT:
       LittleFS.format();
//       Serial.println("Reboot or power cycle to restart the CubeSatSim");
 //      while (1) ;	    // infinite loop
       Serial.println("Rebooting...");	 
       Serial.flush();			  
       watchdog_reboot (0, SRAM_END, 500);	 // restart Pico
       sleep(20.0);			  
       break;
		  
    case PROMPT_PAYLOAD:	      
      Serial.println("Resetting the Payload");
      payload_command = PAYLOAD_RESET;
      start_payload();      		  
      break;			  

    case PROMPT_WIFI:
      Serial.println(wifi);		  
      if (wifi) {		  
      char ssid[30], pass[30];		  
      Serial.println("Enter the credentials for your WiFi network");	
  		  
      Serial.print("Enter WiFi SSID: ");
      get_serial_string();
		  
      print_string(serial_string);
		  
      if (strlen(serial_string) > 0)	{
	strcpy(ssid, serial_string);      
        Serial.print("Enter WiFi password: ");
        get_serial_string();
	if (strlen(serial_string) > 0) {
	    strcpy(pass, serial_string);
	    Serial.println("Connecting to Wifi");
//	    Serial.printf("%s%s\n",ssid, pass);
		
	    WiFi.begin(ssid, pass);
		
	    unsigned int elapsed_timer = (unsigned int) millis();			
	    while ((WiFi.status() != WL_CONNECTED) && ((millis() - elapsed_timer) < 10000)) {
    		Serial.print(".");
    		delay(500);
  	    }
	    if (((millis() - elapsed_timer) > 10000))	
	      Serial.println("Failed to connect!");
	    else
	      Serial.println("Connected to WiFi!");		    
	} else 
	    Serial.println("No password entered.");	
      } else
        Serial.println("No SSID entered.");	      
      } else
	Serial.println("WiFi not available");
		  
      break;		  		  
		  
    case PROMPT_I2CSCAN:
      Serial.print("I2C scan");

// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    https://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
// 
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//


{
  byte error, address;
  int nDevices;

  Serial.println("Scanning I2C Bus 1");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at bus 1 address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at bus 1 address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found on bus 1\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
	
  Serial.println("Scanning I2C Bus 2");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
	  
    Wire1.beginTransmission(address);
    error = Wire1.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at bus 2 address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at bus 2 address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found on bus 2\n");
  else
    Serial.println("done\n");

}		  	  
        Serial.println("complete");
      break;	


	  
/*	  
		  
    case PROMPT_SIM:
      if (sim_mode == TRUE)
	Serial.println("Simulted Telemetry mode is currently on");      
      else
	Serial.println("Simulted Telemetry mode is currently off");  
      Serial.println("Do you want Simulated Telemetry on (y/n)");
      get_serial_char();
      if ((serial_string[0] == 'y') || (serial_string[0] == 'Y'))	{  
        Serial.println("Setting Simulated telemetry to on");
	reset_min_max();      
	config_simulated_telem();     
	write_config_file();    	      
      } else if ((serial_string[0] == 'n') || (serial_string[0] == 'N')) {	      
        Serial.println("Setting Simulated telemetry to off");
	reset_min_max();   	      
	sim_mode = false;      
        if (!ina219_started)
	  start_ina219(); 
	write_config_file();    	      
      } else      
        Serial.println("No change to Simulated Telemetry mode");
      break;	
		  
    case PROMPT_LAT:

      Serial.println("Changing the  latitude and longitude  - only used for APRS telemetry");
      Serial.println("Hitting return keeps the current value.");
		  
      Serial.print("Current value of latitude is ");
      Serial.println(latitude);		  
      Serial.println("Enter latitude  (decimal degrees, positive is north): ");	  
      get_serial_string();		  
      float_result = atof(serial_string);
      if (float_result != 0.0)  {
        Serial.print("Latitude updated to ");	    
        Serial.println(float_result);		  
        latitude = float_result;
      } else
        Serial.println("Latitude not updated");	

      get_serial_clear_buffer();		  
      Serial.print("Current value of longitude is ");
      Serial.println(longitude);		  
      Serial.println("Enter longitude  (decimal degrees, positive is east): ");	  
      get_serial_string();		  
      float_result = atof(serial_string);
      if (float_result != 0.0)  {
        Serial.print("Longitude updated to ");	    
        Serial.println(float_result);		  
        longitude = float_result;
      } else
        Serial.println("Longitude not updated");
		  
      write_config_file();     	  
      if (mode == AFSK)
	set_lat_lon();
		  
      break;	
		  
    case PROMPT_QUERY:
      Serial.println("Querying payload sensors");		  		  
      payload_command = PAYLOAD_QUERY;		  
      break;
		  
   case PROMPT_CAMERA:
      show_dir();		  
      get_camera_image(debug_camera);	 
      show_dir();		  
      break;
				    
    case PROMPT_TEMP:		  
      sensorValue = analogRead(TEMPERATURE_PIN);
      Serial.print("Raw diode voltage: ");		  
      Serial.println(sensorValue);  
      Temp = T1 + (sensorValue - R1) *((T2 - T1)/(R2 - R1));
      Serial.print("Calculated temperature: ");		  
      Serial.print(Temp);  
      Serial.println(" C");		  	  
      break;
		    
    case PROMPT_VOLTAGE:
      Serial.println("Querying INA219 voltage and current sensors");
      if (!ina219_started)
	start_ina219();
      voltage_read = true;		  
      read_ina219();		  	  
      break;	

		  
    case PROMPT_RESET:
      Serial.println("Reset count is now 0");	
      reset_count = 0;
      write_config_file();    	  
      break;	
		  
    case PROMPT_RESTART:
      prompt = false;
//    Serial.println("Restart not yet implemented");
      start_payload();
//      start_ina219();
      if ((mode != CW)  || (!filter_present))
        transmit_callsign(callsign);
      sleep(0.5);	 
      config_telem();
      config_radio();
      sampleTime = (unsigned int) millis();	 		  
      break;	  
		  
    case PROMPT_DEBUG:
      Serial.print("Changing Debug Mode to ");
      debug_mode = !debug_mode;
      if (debug_mode)  
        Serial.println("on");	
      else  
        Serial.println("off");
      break;	
*/		  
		  
  }
  prompt = false;
  prompting = false;
 } 
//	else
//	 Serial.println("Already prompting!");
}

void get_serial_string() {
  int input = 0;	
  int i = 0;
  unsigned int elapsed_time = (unsigned int) millis();	
  while ((input != '\n') && (input!= '\r') && (i < 128) && ((millis() - elapsed_time) < 20000)) {
    if (Serial.available() > 0) {
      input = Serial.read();
      if ((input != '\n') && (input!= '\r')) {
	serial_string[i++] = input;
        Serial.write(input);
      }
    } 
    sleep(0.1);	  
  }
  serial_string[i] = 0;	
  Serial.println(" ");	
}

void get_serial_char() {	
  unsigned int elapsed_time = (unsigned int) millis();	
  while (((millis() - elapsed_time) < 20000) && (Serial.available() < 1)) { }
  if (Serial.available() > 0) {	 
    serial_string[0] = Serial.read();  // get character
    Serial.write(serial_string[0]);
    serial_string[1] = 0;	
    Serial.println(" ");
  } else
  {
    serial_string[0] = 0;	// timeout - no character	 
  }
}

void get_serial_clear_buffer() {
//  Serial.println("Emptying serial input buffer");	
  while (Serial.available() > 0)
   Serial.read();
	
}


void read_config() {
  LittleFS.begin();
  Serial.println("Reading config file");	
  char buff[32];		
  File mode_file = LittleFS.open("/config.txt", "r");	
//  if (!mode_file) {
//    write_mode(mode);	  
//  } else {
  //  if (mode_file.read((uint8_t *)buff, 31)) {
//      Serial.println("Reading mode from .mode file");    
      sscanf(buff, "%d", &mode);
      mode_file.close();
//      Serial.print("Mode is ");
//      Serial.print(mode);
	    
//    }
  //}		
}

void write_config(int save_mode) {

  char buff[32];	
  Serial.println("Writing config file");	
  File mode_file = LittleFS.open("/config.txt", "w+");	
	
  sprintf(buff, "%d", save_mode);
  if (debug_mode) {	
    Serial.println("Writing string");	
    print_string(buff);	
  }
	
  if (mode_file.write(buff, strlen(buff)) != strlen(buff)) {	  
//    Serial.println(mode_file.write(buff, strlen(buff)));
    Serial.println("*** config file write error! ***\n\n");	  
    blinkFastTimes(3);  
  }	  
	  
  mode_file.close();
//  Serial.println("Write complete");	
}


void get_input() {
//  if (mode != SSTV)	
//    Serial.print("+");	
//  if ((mode == CW) || (mode == SSTV))
    serial_input();  
	
// check for button press 
//  if (digitalRead(MAIN_PB_PIN) == PRESSED) // pushbutton is pressed
//      process_pushbutton();
  if (BOOTSEL)	  // boot selector button is pressed on Pico
//      process_bootsel();
      Serial.println("boot selector button pressed!");	
	
  if (prompt) {
//    Serial.println("Need to prompt for input!");
    prompt_for_input();	  
    prompt = false;	  
  }
/*	
  // check to see if the mode has changed
 if (mode != new_mode) {
    Serial.println("Changing mode");
    cw_stop = false; // enable CW or won't hear CW ID	 
///    if (mode == SSTV) {
///      ITimer1.detachInterrupt();	    
///      start_button_isr();  // restart button isr
///    }
    int old_mode = mode;
    bool config_done = false;
//    mode = new_mode;  // change modes if button pressed	 
    write_mode(new_mode);
	
    Serial.println("Rebooting...");
    Serial.flush();	 
    watchdog_reboot (0, SRAM_END, 500); //10);	 // restart Pico
	 
    sleep(20.0);	
	 */
/*	 
    if (new_mode != CW)
      transmit_callsign(callsign);
    sleep(0.5);
	 
    if (!config_done)	 
      config_telem();  // run this here for all other modes
	 
    config_radio();
    if ((mode == FSK) || (mode == BPSK)) {    
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(MAIN_LED_BLUE, HIGH);	    
    }
	
    sampleTime = (unsigned int) millis();
*/	 
// }		
	
}
