// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

// modified by Alan Johnston to show reading registers 0 - 3
// code based on https://forum.arduino.cc/index.php?topic=211587.0

#include <Wire.h>
#define I2C_ADDRESS 0x4B
#define REGISTER_0 0x00
#define REGISTER_1 0x01
#define REGISTER_2 0x02
#define REGISTER_3 0x03

uint8_t reg; // I2C read register

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Starting");
}

void loop() {
  for (reg=0; reg < 4; reg++) {
    delay(5000);
    digitalWrite(LED_BUILTIN, HIGH);
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
  
    
    Wire.requestFrom(I2C_ADDRESS, 2);    // request 2 bytes from I2C device
    byte LSB = Wire.read();
    byte MSB = Wire.read();
  
    uint16_t register_value = ((MSB << 8) | LSB);
    
    Serial.print("Read ");
    Serial.print(register_value);
    Serial.print(" from register ");
    Serial.println(reg);
  }
}
