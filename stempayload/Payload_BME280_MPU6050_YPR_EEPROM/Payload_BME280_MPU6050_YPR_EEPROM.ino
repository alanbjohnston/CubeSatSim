// code for Pico or Pro Micro or STM32 on the CubeSat Simulator STEM Payload board
// works wih CubeSatSim software v1.3.2 or later
// extra sensors can be added in payload_extension.cpp file
// adds absolute orientation yaw, pitch, roll as Sensor 1, 2, and 3.

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//#include <MPU6050_tockn.h>
#include "MPU6050_6Axis_MotionApps20.h"
#if !defined(ARDUINO_ARCH_MBED_RP2040) // && defined(ARDUINO_ARCH_RP2040)
#include <EEPROM.h>
#endif

#if defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)  // if Arduino Mbed OS RP2040 Boards is used in Arduino IDE
#include <TinyGPS++.h>
TinyGPSPlus gps;
UART Serial2(8, 9, 0, 0);

#elif !defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)  // if Raspberry Pi RP2040 Boards in Arduino IDE
#include <TinyGPS++.h>
TinyGPSPlus gps;
bool check_for_wifi();
bool wifi = false;
int led_builtin_pin;

#else  // if Sparkfun Pro Micro or STM32 
#include <EEPROM.h>
#endif

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
//MPU6050 mpu6050(Wire);

MPU6050 mpu;
int const INTERRUPT_PIN = 6;
bool DMPReady = false;
uint8_t devStatus;
uint8_t MPUIntStatus;   // Holds actual interrupt status byte from MPU
uint16_t packetSize;    // Expected DMP packet size (default is 42 bytes)
uint8_t FIFOBuffer[64]; // FIFO storage buffer

Quaternion q;        // [w, x, y, z]         Quaternion container
VectorFloat gravity; // [x, y, z]            Gravity vector
float ypr[3];        // [yaw, pitch, roll]   Yaw/Pitch/Roll container and gravity vector
int16_t ax, ay, az;
int16_t gx, gy, gz;
float accelx, accely, accelz, acceleration;
float gyrox, gyroy, gyroz, rotation;
int yaw, pitch, roll;
int reset_count;

long timer = 0;
int bmePresent;
int RXLED = 17;  // The RX LED has a defined Arduino pin
int whiteLED = 9;
int yellowLED = 8;
int Sensor1 = 0;
float Sensor2 = 0;
float temp;
int calibration = 0;

void ee_prom_word_write(int addr, int val);
short ee_prom_word_read(int addr);
int first_time = true;
int first_read = true;

#if defined (ARDUINO_ARCH_MBED_RP2040) || (ARDUINO_ARCH_RP2040)
float T2 = 24; // Temperature data point 1
float R2 = 169; // Reading data point 1
float T1 = 6; // Temperature data point 2
float R1 = 181; // Reading data point 2
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

volatile bool MPUInterrupt = false; // Indicates whether MPU6050 interrupt pin has gone high
void DMPDataReady()
{
  MPUInterrupt = true;
}

void setup() {
	
  Serial.begin(115200); // Serial Monitor for testing

#if !defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)
   Serial1.setRX(1);
   delay(100);
   Serial1.setTX(0);
   delay(100);	
#endif 
	
  Serial1.begin(115200);  // for communication with Pi Zero 
	
#if !defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)  // if Raspberry Pi RP2040 Boards in Arduino IDE	
  EEPROM.begin(512);
#endif
	
  delay(2000);
	
#if defined (ARDUINO_ARCH_MBED_RP2040) && (ARDUINO_ARCH_RP2040)
  Serial.println("Pico with Mbed");
#elif !defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)
  Serial.println("Pico with RP2040");  
#elif defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)
  Serial.println("STM32");
#elif defined __AVR_ATmega32U4__
  Serial.println("Pro Micro");
#else
  Serial.println("Unknown board");
#endif  
	
  Serial.println("Starting!");

#if defined (ARDUINO_ARCH_MBED_RP2040) || (ARDUINO_ARCH_RP2040)
  Serial.println("Starting Serial2 for optional GPS on JP12");		
//  Serial2.begin(9600);  // serial from  - some  modules need 115200
  Serial2.begin(9600);   // serial from GPS or other serial sensor.  Some GPS need 115200

  // set all Pico GPIO connected pins to input	
  for (int i = 10; i < 22; i++) { 
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
  led_set(whiteLED, HIGH);
  delay(250);
  led_set(whiteLED, LOW);
  led_set(yellowLED, HIGH);
  delay(250);
  led_set(yellowLED, LOW);

  if (bme.begin(0x76)) {
    bmePresent = 1;
  } else {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    bmePresent = 0;
  }
 
//  mpu6050.begin();

  ax = 0;
  ay = 0;
  az = 0;
  gx = 0;
  gy = 0;
  gz = 0;
  yaw = 0;
  pitch = 0;
  roll = 0;
  accelx = 0;
  accely = 0;
  accelz = 0;
  acceleration = 1.0;
  gyrox = 0;
  gyroy = 0;
  gyroz = 0;
  
  mpu.initialize(ACCEL_FS::A2G, GYRO_FS::G250DPS);
//  mpu.initialize();
  
  pinMode(INTERRUPT_PIN, INPUT);

  // Verify connection
  if (mpu.testConnection())
  {
    Serial.println("MPU6050 connection successful");
  }
  else
  {
    Serial.println("MPU6050 connection failed");
  }

  /* Initializate and configure the DMP*/
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  if (eeprom_word_read(0) == 0xA08)
  {
    Serial.println("Reading gyro and accel offsets from EEPROM\n");

    int axOffset = eeprom_word_read(1);
    int ayOffset = eeprom_word_read(2);
    int azOffset = eeprom_word_read(3); 
    int gxOffset = eeprom_word_read(4);
    int gyOffset = eeprom_word_read(5);
    int gzOffset = eeprom_word_read(6);

    Serial.println(axOffset, DEC);
    Serial.println(ayOffset, DEC);
    Serial.println(azOffset, DEC);    
    Serial.println(gxOffset, DEC);
    Serial.println(gyOffset, DEC);
    Serial.println(gzOffset, DEC);
  
  mpu.setXGyroOffset(gxOffset);
  mpu.setYGyroOffset(gyOffset);
  mpu.setZGyroOffset(gzOffset);
  mpu.setXAccelOffset(axOffset);
  mpu.setYAccelOffset(ayOffset);
  mpu.setZAccelOffset(azOffset);

  mpu.PrintActiveOffsets();

    Serial.println("\nTemperature calibration data from EEPROM\n");
 
    T1 = ((float)eeprom_word_read(7)) / 10.0;
    R1 = ((float)eeprom_word_read(8));
    T2 = ((float)eeprom_word_read(9)) / 10.0;
    R2 = ((float)eeprom_word_read(10));
 
    Serial.println(T1, DEC);
    Serial.println(R1, DEC);
    Serial.println(" ");    
    Serial.println(T2, DEC);
    Serial.println(R2, DEC);
    Serial.println(" ");    
    
  }
  else
  {
 //   Serial.println("Calculating gyro offsets\n");
 //   mpu6050.calcGyroOffsets(true);

  if (devStatus == 0)
  {
 //   if (first_read == true) {
      mpu.CalibrateAccel(6); // Calibration Time: generate offsets and calibrate our MPU6050
      mpu.CalibrateGyro(6);
      Serial.println("These are the Active offsets: ");
      mpu.PrintActiveOffsets();

      int16_t *offsets;
      offsets = mpu.GetActiveOffsets();
      
  //  A_OFFSET_H_READ_A_OFFS(Data);
    Serial.print((float)offsets[0], 5); Serial.print(",\t");
    Serial.print((float)offsets[1], 5); Serial.print(",\t");
    Serial.print((float)offsets[2], 5); Serial.print(",\t");
  
  //  XG_OFFSET_H_READ_OFFS_USR(Data);
    Serial.print((float)offsets[3], 5); Serial.print(",\t");
    Serial.print((float)offsets[4], 5); Serial.print(",\t");
    Serial.print((float)offsets[5], 5); Serial.print("\n\n");

//    }
    
#if !defined(ARDUINO_ARCH_MBED_RP2040) // && defined(ARDUINO_ARCH_RP2040)  // if Raspberry Pi RP2040 Boards is used in Arduino IDE
    Serial.println("Storing gyro and accel offsets in EEPROM\n");
 
    eeprom_word_write(0, 0xA08);
 /*   eeprom_word_write(1, (int)(mpu6050.getGyroXoffset() * 100.0) + 0.5);
    eeprom_word_write(2, (int)(mpu6050.getGyroYoffset() * 100.0) + 0.5);
    eeprom_word_write(3, (int)(mpu6050.getGyroZoffset() * 100.0) + 0.5);  
 */
 /*
    Serial.println(eeprom_word_read(0), HEX);
    Serial.println(((float)eeprom_word_read(1)) / 100.0, DEC);
    Serial.println(((float)eeprom_word_read(2)) / 100.0, DEC);
    Serial.println(((float)eeprom_word_read(3)) / 100.0, DEC);
*/
    eeprom_word_write(1, offsets[0]);
    eeprom_word_write(2, offsets[1]);
    eeprom_word_write(3, offsets[2]);
    eeprom_word_write(4, offsets[3]);
    eeprom_word_write(5, offsets[4]);
    eeprom_word_write(6, offsets[5]);
        
   Serial.println("\nStoring temperature calibration data in EEPROM\n");

   eeprom_word_write(7, (int)(T1 * 10.0) + 0.5);
   eeprom_word_write(8, (int) R1);    
   eeprom_word_write(9, (int)(T2 * 10.0) + 0.5);
   eeprom_word_write(10, (int) R2);
    
   T1 = ((float)eeprom_word_read(7)) / 10.0;
   R1 = ((float)eeprom_word_read(8));
   T2 = ((float)eeprom_word_read(9)) / 10.0;
   R2 = ((float)eeprom_word_read(10));

    Serial.println(T1, DEC);
    Serial.println(R1, DEC);
    Serial.println(" ");
    Serial.println(T2, DEC);
    Serial.println(R2, DEC);
    Serial.println(" ");
    
#if !defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)  // if Raspberry Pi RP2040 Boards is used in Arduino IDE   
   if (EEPROM.commit()) {
      Serial.println("EEPROM successfully committed\n");
   } else {
      Serial.println("ERROR! EEPROM commit failed\n");
   }
#endif    
#endif    
  }
  }

  /* Supply your gyro offsets here, scaled for min sensitivity */
/*  mpu.setXGyroOffset(57);
  mpu.setYGyroOffset(99);
  mpu.setZGyroOffset(19);
  mpu.setXAccelOffset(-4922);
  mpu.setYAccelOffset(-2136);
  mpu.setZAccelOffset(3104);

    mpu.setXGyroOffset(0);
  mpu.setYGyroOffset(0);
  mpu.setZGyroOffset(0);
  mpu.setXAccelOffset(0);
  mpu.setYAccelOffset(0);
  mpu.setZAccelOffset(0);
*/
  if (devStatus == 0)
  {
/*    if (first_read == true) {
      mpu.CalibrateAccel(6); // Calibration Time: generate offsets and calibrate our MPU6050
      mpu.CalibrateGyro(6);
      Serial.println("These are the Active offsets: ");
      mpu.PrintActiveOffsets();
    }
    */
    Serial.println(F("Enabling DMP...")); // Turning ON DMP
    mpu.setDMPEnabled(true);

    Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
    Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
    Serial.println(F(")..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), DMPDataReady, RISING);
    MPUIntStatus = mpu.getIntStatus();

    /* Set the DMP Ready flag so the main loop() function knows it is okay to use it */
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    DMPReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
 //   Serial.print(packetSize);
  }
  else
  {
    Serial.print(F("DMP Initialization failed (code ")); // Print the error code
    Serial.print(devStatus);
    Serial.println(F(")"));
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
  }
	

  payload_setup();  // sensor extension setup function defined in payload_extension.cpp   

  reset_count = 100;
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
	      temp = bme.readTemperature();       
        Serial.print(temp);
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

 /*     
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
*/     

//   if (MPUInterrupt) {
//   if (true) {

   if (mpu.dmpGetCurrentFIFOPacket(FIFOBuffer))
    {
      /* Display Euler angles in degrees */
      mpu.dmpGetQuaternion(&q, FIFOBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

      accelx = ax / 16384.0; 
      accely = ay / 16384.0; 
      accelz = (az - 16384) / 16384.0; 
      acceleration = sqrt(accelx*accelx + accely*accely + accelz*accelz);

      gyrox = gx / 6.75; 
      gyroy = gy / 6.75; 
      gyroz = gz / 6.75; 
      rotation = sqrt(gyrox*gyrox + gyroy*gyroy + gyroz*gyroz);

      yaw = ypr[0] * 180 / M_PI;
      if (yaw < 0) yaw += 360;
      pitch = ypr[1] * 180 / M_PI;
      if (pitch < 0) pitch += 360;
      roll = ypr[2] * 180 / M_PI;
      if (roll < 0) roll += 360;

 //     Serial.print("_"); Serial.print(yaw); Serial.print("_");
 //   } else
 //     Serial.print("*");

    MPUInterrupt = false;
   } else
 //     Serial.print("/");
      
      Serial1.print(" ");  // Don't know why this is needed
      Serial1.print(" MPU6050 ");
      Serial1.print(gyrox); Serial1.print(" ");
      Serial1.print(gyroy); Serial1.print(" ");
      Serial1.print(gyroz); Serial1.print(" ");
      Serial1.print(accelx); Serial1.print(" ");
      Serial1.print(accely); Serial1.print(" ");
      Serial1.print(accelz); 
 
      Serial.print(" MPU6050 ");
      Serial.print(gyrox); Serial.print(" ");
      Serial.print(gyroy); Serial.print(" ");
      Serial.print(gyroz); Serial.print(" ");
      Serial.print(accelx); Serial.print(" ");
      Serial.print(accely); Serial.print(" ");
      Serial.print(accelz); 

/*      
    } else {
        Serial1.print("OK MPU6050 0.0 0.0 0.0 0.0 0.0 0.0");
        Serial.print("OK MPU6050 0.0 0.0 0.0 0.0 0.0 0.0");
    }
*/
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
     
//    float rotation = 0;
//   rotation = sqrt(mpu6050.getGyroX()*mpu6050.getGyroX() + mpu6050.getGyroY()*mpu6050.getGyroY() + mpu6050.getGyroZ()*mpu6050.getGyroZ()); 
//    float acceleration = 0;
//    acceleration = sqrt(mpu6050.getAccX()*mpu6050.getAccX() + mpu6050.getAccY()*mpu6050.getAccY() + mpu6050.getAccZ()*mpu6050.getAccZ()); 
//    Serial.println(" ");	    
//    Serial.print(rotation);
//    Serial.print(" ");
//    Serial.println(acceleration);
 
    if (first_read == true) {
      first_read = false;
      rest = acceleration;
      Serial.println(" ");
      Serial.print("rest acceleration: ");
      Serial.println(rest);	    
    }
 
    if (acceleration > 1.1 * rest)
        led_set(whiteLED, HIGH);
    else
        led_set(whiteLED, LOW);
        
    if (rotation > 20)
        led_set(yellowLED, HIGH);
    else
        led_set(yellowLED, LOW);
    }

      Serial1.print(" YPR ");
      Serial1.print(yaw);
      Serial1.print(" ");
      Serial1.print(pitch);
      Serial1.print(" ");
      Serial1.print(roll);
      Serial1.print(" ");

      Serial.print(" YPR ");
      Serial.print(yaw);
      Serial.print(" ");
      Serial.print(pitch);
      Serial.print(" ");
      Serial.print(roll);
      Serial.println(" ");
      
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
//#if !defined (ARDUINO_ARCH_RP2040)
  if (result == 'R' || result == 'r') {	  
//      Serial1.println("OK");
//      delay(100);
      Serial.println("Resetting\n");	  
      first_read = true;
      setup();
    }
  else if (result == 'D' || result == 'd') {
    Serial.println("\nCurrent temperature calibration data\n");	  
    Serial.println(T1, DEC);
    Serial.println(R1, DEC);
    Serial.println(" ");	  
    Serial.println(T2, DEC);
    Serial.println(R2, DEC);
	  
    Serial.println("\nCurrent raw temperature reading\n");	  
    Serial.println(sensorValue, DEC);	
    Serial.println(" ");
  }
  else if (result == 'C' || result == 'c') {
      Serial.println("\nClearing stored gyro and accel offsets in EEPROM\n");
      eeprom_word_write(0, 0x00);
#if !defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)  // if Raspberry Pi RP2040 Boards is used in Arduino IDE
	  
      if (EEPROM.commit()) {
      	Serial.println("EEPROM successfully committed\n");
      } else {
        Serial.println("ERROR! EEPROM commit failed\n");
      }	
#endif	  
      first_time = true;
      setup();
    }
  else if (result == 'S' || result == 's') {
    Serial.print("\nStoring temperature calibration data point "); //  in EEPROM\n");
    Serial.print(calibration + 1);	  
    Serial.print(" in EEPROM\n");
	    
    Serial.println(temp);
    Serial.println(sensorValue);
    Serial.println(" ");	  
	  
    eeprom_word_write(calibration * 2 + 7 , (int)(temp * 10.0) + 0.5);
    eeprom_word_write(calibration * 2 + 8, sensorValue);

    if (calibration == 0) {
	    T1 = temp;
	    R1 = sensorValue;
	    calibration = 1;
    } else {
	    T2 = temp;
	    R2 = sensorValue;
	    calibration = 0;
    } 	    

//    calibration = (calibration + 1) % 2;
//    Serial.println(calibration + 1);	  
	  
#if !defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)  // if Raspberry Pi RP2040 Boards is used in Arduino IDE
	  
    if (EEPROM.commit()) {
      Serial.println("EEPROM successfully committed\n");
    } else {
      Serial.println("ERROR! EEPROM commit failed\n");
    }
#endif
	  
  }  
//#endif	  	
  }  
	  
#if defined (ARDUINO_ARCH_MBED_RP2040) || (ARDUINO_ARCH_RP2040)
  Serial.print("Squelch: ");	
  Serial.println(digitalRead(15));

  get_gps();
#else
  delay(1000);  // not needed due to gps 1 second polling delay

#endif
//  if (reset_count-- == 0)
//    setup();
}
 
void eeprom_word_write(int addr, int val)
{
#if !defined(ARDUINO_ARCH_MBED_RP2040) // && defined(ARDUINO_ARCH_RP2040)  // if Raspberry Pi RP2040 Boards is used in Arduino IDE
  EEPROM.write(addr * 2, lowByte(val));
  EEPROM.write(addr * 2 + 1, highByte(val));
#endif	
}
 
short eeprom_word_read(int addr)
{
  int result = 0;	
#if !defined(ARDUINO_ARCH_MBED_RP2040) // && defined(ARDUINO_ARCH_RP2040)  // if Raspberry Pi RP2040 Boards is used in Arduino IDE
  result = ((EEPROM.read(addr * 2 + 1) << 8) | EEPROM.read(addr * 2));
#endif
  return result;	
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
  pinMode(whiteLED, OUTPUT);
  pinMode(yellowLED,OUTPUT);
#endif

#if defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)
  pinMode(LED_BUILTIN, OUTPUT);     
  pinMode(18, OUTPUT);  // yellow LED (was blue LED on STEM Payload Board v1.3.2)
  pinMode(19, OUTPUT);  // white LED (was green LED on STEM Payload Board v1.3.2)	   
#endif

#if !defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)
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
     pinMode(18, OUTPUT);
     pinMode(19, OUTPUT);	   
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

#if defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)
  digitalWrite(LED_BUILTIN, HIGH);   // set the built-in LED ON
#endif  

#if !defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)
  if (wifi)	
    digitalWrite(LED_BUILTIN, HIGH);   // set the built-in LED ON
  else
    digitalWrite(led_builtin_pin, HIGH);   // set the built-in LED ON
#endif  	

delay(length);	
 
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)
  digitalWrite(PC13, HIGH);    // turn the LED off by making the voltage LOW
#endif
 
#if defined __AVR_ATmega32U4__
  digitalWrite(RXLED, HIGH);    // set the RX LED OFF
  TXLED0; //TX LED macro to turn LED ON
#endif  

#if defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)
    digitalWrite(LED_BUILTIN, LOW);   // set the built-in LED OFF
#endif  

#if !defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)
  if (wifi)	
    digitalWrite(LED_BUILTIN, LOW);   // set the built-in LED ON
  else
    digitalWrite(led_builtin_pin, LOW);   // set the built-in LED ON
#endif  	
}
 
void led_set(int ledPin, bool state)
{
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)
  if (ledPin == whiteLED)
    digitalWrite(PB9, state);
  else if (ledPin == yellowLED)
    digitalWrite(PB8, state);    
#endif
 
#if defined __AVR_ATmega32U4__
  digitalWrite(ledPin, state);   
#endif  

#if defined (ARDUINO_ARCH_MBED_RP2040) || (ARDUINO_ARCH_RP2040)
  if (ledPin == whiteLED)
    digitalWrite(19, state);
  else if (ledPin == yellowLED)
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
#if defined (ARDUINO_ARCH_MBED_RP2040) || (ARDUINO_ARCH_RP2040)
    sensorValue = analogRead(28);  
#endif
    return(sensorValue); 
}

#if !defined(ARDUINO_ARCH_MBED_RP2040) && defined(ARDUINO_ARCH_RP2040)
bool check_for_wifi() {

  pinMode(29, INPUT);	
   const float conversion_factor = 3.3f / (1 << 12);
   uint16_t result = analogRead(29);
//   Serial.printf("ADC3 value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);

  if (result < 0x10) {
    Serial.println("\nPico W detected!\n");
    return(true);
  }
  else {
     Serial.println("\nPico detected!\n");
     return(false);  
  }
}
#endif

#if defined (ARDUINO_ARCH_MBED_RP2040) || (ARDUINO_ARCH_RP2040)
void get_gps() {
//  Serial.println("Getting GPS data");	
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
}
#endif	
