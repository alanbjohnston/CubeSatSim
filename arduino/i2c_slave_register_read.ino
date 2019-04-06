#include <Wire.h>
#define I2C_ADDRESS 0x4B
#define REGISTER_0 0x00
#define REGISTER_1 0x01
#define REGISTER_2 0x02
#define REGISTER_3 0x03

uint8_t reg; // I2C read register
unsigned int reg_0_value = 41151;  
unsigned int reg_1_value = 53124;
unsigned int reg_2_value = 65535;
unsigned int reg_3_value = 0;

void setup() {

  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);           // start serial for output
  Serial.println();
  Serial.println("Starting");
}

void loop() {
  Serial.println("Waiting for register read");
  delay(5000);
}

void receiveEvent(int bytes) {
  // Read the first byte to determine which register is concerned
  reg = Wire.read();
    Serial.print("Read register ");
    Serial.println(reg);
}

void requestEvent() {
  // Read from the register variable to know what to send back
  digitalWrite(LED_BUILTIN, HIGH);
  if (reg == REGISTER_0) {
    Wire.write((uint8_t *)&reg_0_value, sizeof(reg_0_value));
    Serial.print("Writing value ");
    Serial.println(reg_0_value);
  } else if (reg == REGISTER_1) {
    Wire.write((uint8_t *)&reg_1_value, sizeof(reg_1_value));
    Serial.print("Writing value ");
    Serial.println(reg_1_value);
  } else if (reg == REGISTER_2) {
    Wire.write((uint8_t *)&reg_2_value, sizeof(reg_2_value));
    Serial.print("Writing value ");
    Serial.println(reg_2_value);
  } else if (reg == REGISTER_3) {
    Wire.write((uint8_t *)&reg_3_value, sizeof(reg_3_value));
    Serial.print("Writing value ");
    Serial.println(reg_3_value);
  } else {
    Serial.println("Unknown register");
  }
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
}
