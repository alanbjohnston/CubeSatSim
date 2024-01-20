// code for Pico or Pro Micro or STM32 on the CubeSat Simulator STEM Payload board
// works wih CubeSatSim software v1.3.2 or later
// extra sensors can be added in payload_extension.cpp file

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <MPU6050_tockn.h>
#include <TinyGPS++.h>
#ifdef ARDUINO_ARCH_MBED_RP2040
UART Serial2(8, 9, 0, 0);
#else
#include <EEPROM.h>
#endif

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
MPU6050 mpu6050(Wire);
TinyGPSPlus gps;

long timer = 0;
int bmePresent;
int RXLED = 17;  // The RX LED has a defined Arduino pin
int greenLED = 9;
int blueLED = 8;
int Sensor1 = 0;
float Sensor2 = 0;
void ee_prom_word_write(int addr, int val);
short ee_prom_word_read(int addr);
int first_time = true;
int first_read = true;

#if defined ARDUINO_ARCH_RP2040
float T2 = 26.3; // Temperature data point 1
float R2 = 167; // Reading data point 1
float T1 = 2; // Temperature data point 2
float R1 = 179; // Reading data point 2
#endif
#if defined __AVR_ATmega32U4__ 
float T2 = 26.3; // Temperature data point 1
float R2 = 167; // Reading data point 1
float T1 = 2; // Temperature data point 2
float R1 = 179; // Reading data point 2
#endif
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)
float T2 = 25; // Temperature data point 1
float R2 = 671; // Reading data point 1
float T1 = 15.5; // Temperature data point 2
float R1 = 695; // Reading data point 2
#endif

int sensorValue;
float Temp;
float rest;

char sensor_end_flag[] = "_END_FLAG_";
char sensor_start_flag[] = "_START_FLAG_";
bool show_gps = true;  // set to false to not see all  messages
float flon = 0.0, flat = 0.0, flalt = 0.0;
void get_gps();

extern void payload_setup();  // sensor extension setup function defined in payload_extension.cpp
extern void payload_loop();  // sensor extension read function defined in payload_extension.cpp

void setup() {
	
  Serial.begin(115200); // Serial Monitor for testing
 
  Serial1.begin(115200);  // for communication with Pi Zero 

  delay(1000);		
 
  Serial.println("Starting!");

#ifdef ARDUINO_ARCH_RP2040
  Serial.println("This code is for the Raspberry Pi Pico hardware.");

  Serial.println("Starting Serial2 for optional GPS on JP12");		
//  Serial2.begin(9600);  // serial from  - some  modules need 115200
  Serial2.begin(9600);   // serial from GPS or other serial sensor.  Some GPS need 115200

  // set all Pico GPIO connected pins to input	
  for (int i = 6; i < 22; i++) { 
      pinMode(i, INPUT);	  
  }
  pinMode(26, INPUT);	
  pinMode(27, INPUT);	
  pinMode(28, INPUT);
  pinMode(15, INPUT_PULLUP);  // squelch	
 
#endif	
 
  blink_setup();
 
  blink(500);
  delay(250);
  blink(500);
  delay(250);
  led_set(greenLED, HIGH);
  delay(250);
  led_set(greenLED, LOW);
  led_set(blueLED, HIGH);
  delay(250);
  led_set(blueLED, LOW);

  if (bme.begin(0x76)) {
    bmePresent = 1;
  } else {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    bmePresent = 0;
  }
 
  mpu6050.begin();
	
  if (eeprom_word_read(0) == 0xA07)
  {
    Serial.println("Reading gyro offsets from EEPROM\n");
 
    float xOffset = ((float)eeprom_word_read(1)) / 100.0;
    float yOffset = ((float)eeprom_word_read(2)) / 100.0;
    float zOffset = ((float)eeprom_word_read(3)) / 100.0;
 
    Serial.println(xOffset, DEC);
    Serial.println(yOffset, DEC);
    Serial.println(zOffset, DEC);
 
    mpu6050.setGyroOffsets(xOffset, yOffset, zOffset);
  }
  else
  {
    Serial.println("Calculating gyro offsets\n");
    mpu6050.calcGyroOffsets(true);	  
#ifndef ARDUINO_ARCH_RP2040	  	  
    Serial.println("Storing gyro offsets in EEPROM\n");
 
    eeprom_word_write(0, 0xA07);
    eeprom_word_write(1, (int)(mpu6050.getGyroXoffset() * 100.0) + 0.5);
    eeprom_word_write(2, (int)(mpu6050.getGyroYoffset() * 100.0) + 0.5);
    eeprom_word_write(3, (int)(mpu6050.getGyroZoffset() * 100.0) + 0.5);
 
    Serial.println(eeprom_word_read(0), HEX);
    Serial.println(((float)eeprom_word_read(1)) / 100.0, DEC);
    Serial.println(((float)eeprom_word_read(2)) / 100.0, DEC);
    Serial.println(((float)eeprom_word_read(3)) / 100.0, DEC);
#endif	  
  }
  payload_setup();  // sensor extension setup function defined in payload_extension.cpp   
}
 
void loop() {

  blink(50);
	
  if (Serial1.available() > 0) {
    Serial.print("Received serial data!!!\n");	 
    delay(10);	  
    while (Serial1.available() > 0) {	  
      char result = Serial1.read();
      Serial.print(result);
    }
    Serial.println(" ");
  }
  {
//    if (result == '?')
    {
      if (bmePresent) {
        Serial1.print(sensor_start_flag);
        Serial1.print("OK BME280 ");
        Serial1.print(bme.readTemperature());
        Serial1.print(" ");
        Serial1.print(bme.readPressure() / 100.0F);
        Serial1.print(" ");
        Serial1.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
        Serial1.print(" ");
        Serial1.print(bme.readHumidity());

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
        Serial1.print(sensor_start_flag);
        Serial1.print("OK BME280 0.0 0.0 0.0 0.0");

        Serial.print("OK BME280 0.0 0.0 0.0 0.0");
      }
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
     
      sensorValue = read_analog();
     
//  Serial.println(sensorValue);  
    Temp = T1 + (sensorValue - R1) *((T2 - T1)/(R2 - R1));
 
//    Serial1.print(" GPS 0 0 0 TMP ");

    Serial1.print(" GPS ");
    Serial1.print(flat,4);   
    Serial1.print(" ");
    Serial1.print(flon,4);              
    Serial1.print(" ");
    Serial1.print(flalt,2);  

    Serial1.print(" TMP ");	    
    Serial1.print(Temp);  
	    
//    Serial1.print(" ");
//    Serial1.println(Sensor2);              

    Serial.print(" GPS ");
    Serial.print(flat,4);   
    Serial.print(" ");
    Serial.print(flon,4);              
    Serial.print(" ");
    Serial.print(flalt,2);  	    

//    Serial.print(" GPS 0 0 0 TMP ");
    Serial.print(" TMP ");	    
    Serial.print(Temp);   
//    Serial.print(" ");
//    Serial.println(Sensor2);     
     
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

    payload_loop(); // sensor extension read function defined in payload_extension.cpp	  

//    Serial1.println(" ");
    Serial1.println(sensor_end_flag);	  
    Serial.println(" ");
	  
  }

  if (Serial.available() > 0) {
    blink(50);
    char result = Serial.read();
//    Serial.println(result);
//    Serial.println("OK");
//    Serial.println(counter++); 
#ifndef	ARDUINO_ARCH_RP2040     
  if (result == 'R') {	  
      Serial1.println("OK");
      delay(100);
      first_read = true;
      setup();
    }
  else if (result == 'C') {
      Serial.println("Clearing stored gyro offsets in EEPROM\n");
      eeprom_word_write(0, 0x00);
      first_time = true;
      setup();
    }  
#endif	  	
  }  
	  
#ifdef ARDUINO_ARCH_RP2040
  Serial.print("Squelch: ");	
  Serial.println(digitalRead(15));

  get_gps();
#else
  delay(1000);  // not needed due to gps 1 second polling delay

#endif
}
 
void eeprom_word_write(int addr, int val)
{
#ifndef ARDUINO_ARCH_RP2040 	
  EEPROM.write(addr * 2, lowByte(val));
  EEPROM.write(addr * 2 + 1, highByte(val));
#endif	
}
 
short eeprom_word_read(int addr)
{
#ifndef ARDUINO_ARCH_RP2040 	
  return ((EEPROM.read(addr * 2 + 1) << 8) | EEPROM.read(addr * 2));
#endif	
}
 
void blink_setup() 
{
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)  
  // initialize digital pin PB1 as an output.
  pinMode(PC13, OUTPUT);
  pinMode(PB9, OUTPUT);
  pinMode(PB8, OUTPUT);
#endif
 
#if defined __AVR_ATmega32U4__
  pinMode(RXLED, OUTPUT);  // Set RX LED as an output
  // TX LED is set as an output behind the scenes
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED,OUTPUT);
#endif

#if defined ARDUINO_ARCH_RP2040
     pinMode(LED_BUILTIN, OUTPUT);     
     pinMode(18, OUTPUT);  // blue LED on STEM Payload Board v1.3.2
     pinMode(19, OUTPUT);  // green LED on STEM Payload Board v1.3.2	   
#endif
}
 
void blink(int length)
{
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)
  digitalWrite(PC13, LOW);   // turn the LED on (HIGH is the voltage level)
#endif
 
#if defined __AVR_ATmega32U4__
  digitalWrite(RXLED, LOW);   // set the RX LED ON
  TXLED0; //TX LED is not tied to a normally controlled pin so a macro is needed, turn LED OFF
#endif  

#if defined ARDUINO_ARCH_RP2040
    digitalWrite(LED_BUILTIN, HIGH);   // set the built-in LED ON
#endif  

delay(length);	
 
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)
  digitalWrite(PC13, HIGH);    // turn the LED off by making the voltage LOW
#endif
 
#if defined __AVR_ATmega32U4__
  digitalWrite(RXLED, HIGH);    // set the RX LED OFF
  TXLED0; //TX LED macro to turn LED ON
#endif  

#if defined ARDUINO_ARCH_RP2040 
    digitalWrite(LED_BUILTIN, LOW);   // set the built-in LED OFF
#endif   
}
 
void led_set(int ledPin, bool state)
{
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)
  if (ledPin == greenLED)
    digitalWrite(PB9, state);
  else if (ledPin == blueLED)
    digitalWrite(PB8, state);    
#endif
 
#if defined __AVR_ATmega32U4__
  digitalWrite(ledPin, state);   
#endif  

#ifdef ARDUINO_ARCH_RP2040 
  if (ledPin == greenLED)
    digitalWrite(19, state);
  else if (ledPin == blueLED)
    digitalWrite(18, state);  
#endif	
}

int read_analog()
{
    int sensorValue;
 #if defined __AVR_ATmega32U4__ 
    sensorValue = analogRead(A3);
#endif
	
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)
    sensorValue = analogRead(PA7);
#endif
#if defined ARDUINO_ARCH_RP2040 
    sensorValue = analogRead(28);  
#endif
    return(sensorValue); 
}

void get_gps() {
#ifdef ARDUINO_ARCH_RP2040	
  bool newData = false;  
  unsigned long start = millis();	
	
//  for (unsigned long start = millis(); millis() - start < 1000;) // 5000;)
  while ((millis() - start) < 1000) // 5000;)
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
  if (newData) {
    Serial.print("GPS read new data in ms: ");	    
    Serial.println(millis() - start);	    

//    float flon = 0.0, flat = 0.0, flalt = 0.0;
//    unsigned long age;
//    starting = millis();	    
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
	    
  } else
//	    Serial.printf("GPS read no new data: %d\n", millis() - start);	      
    ;
#endif	
}
