#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <MPU6050_tockn.h>

#define SEALEVELPRESSURE_HPA (1013.25)

//#define TESTING  // Define to test on Serial Monitor

Adafruit_BME280 bme;
MPU6050 mpu6050(Wire);

int counter = 0;
long timer = 0;

void setup() {

  Serial.begin(9600); // Serial Monitor for testing

  Serial1.begin(9600);  // Pi UART

  Serial.println("Starting!");

  pinMode(PC13, OUTPUT);
    digitalWrite(PC13, LOW);   // turn the LED on 
    delay(50);              // wait for a second
    digitalWrite(PC13, HIGH);    // turn the LED off
 
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {

#ifdef TESTING
  if (Serial.available() > 0) {
    
    digitalWrite(PC13, LOW);   // turn the LED on
    delay(50);              // wait for a second
    digitalWrite(PC13, HIGH);    // turn the LED off

    char result = Serial.read();
    //    Serial1.println(result);
    Serial.print("OK BME280 ");
    Serial.print(bme.readTemperature());
    Serial.print(" ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.print(" ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.print(" ");
    Serial.print(bme.readHumidity());
    
    mpu6050.update();
    
    Serial.print(" MPU6050 ");
    Serial.print(mpu6050.getGyroX());
    Serial.print(" ");
    Serial.print(mpu6050.getGyroY());
    Serial.print(" ");
    Serial.println(mpu6050.getGyroZ());

    //  Serial1.println(counter++);
  }
#else
  if (Serial1.available() > 0) {

    digitalWrite(PC13, LOW);   // turn the LED on
    delay(50);              // wait for a second
    digitalWrite(PC13, HIGH);    // turn the LED off

    char result = Serial1.read();
//    Serial1.println(result);
//    Serial1.println("OK ");
  
    Serial1.print("OK BME280 ");
    Serial1.print(bme.readTemperature());
    Serial1.print(" ");
    Serial1.print(bme.readPressure() / 100.0F);
    Serial1.print(" ");
    Serial1.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial1.print(" ");
    Serial1.print(bme.readHumidity());

    mpu6050.update();
    
    Serial1.print(" MPU6050 ");
    Serial1.print(mpu6050.getGyroX());
    Serial1.print(" ");
    Serial1.print(mpu6050.getGyroY());
    Serial1.print(" ");
    Serial1.println(mpu6050.getGyroZ());   

//  Serial1.println(counter++);
  }
#endif

  delay(100);
}
