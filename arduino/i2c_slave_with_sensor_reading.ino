#include <Wire.h>
#define REGISTER_0 0x00
#define REGISTER_1 0x01
#define REGISTER_2 0x02
#define REGISTER_3 0x03
#define I2C_ADDRESS_SELF 0x4B

unsigned int reg_0_value = 41151;  
unsigned int reg_1_value = 0;
unsigned int reg_2_value = 0;
unsigned int reg_3_value = 0;

uint8_t master_reg; // I2C master read register
uint8_t slave_reg; // I2C slave read register

void setup()
{
  Serial.begin(9600); //Begins Serial communication
  Serial.println("Setup for sensor");

  Wire.begin(I2C_ADDRESS_SELF);        // join i2c bus
  Wire.setClock(400000); // set I2C clock for full speed
  Serial.begin(9600);  // start serial for output
  digitalWrite(A4, LOW); 
  digitalWrite(A5, LOW);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Starting");
}

void loop()
{
  delay(1000);
  
  Serial.println("Read sensor value");
  
  reg_0_value = 1;  // set register 0 value to the sensor value
  reg_1_value += 1;  // increment a count of how many values read
}

void receiveEvent(int bytes) {
  // Slave reads the first byte to determine which register is concerned
  slave_reg = Wire.read();
    Serial.print("Slave read register ");
    Serial.println(slave_reg);
}

void requestEvent() {
  // Slave uses the the register variable to know what to send back
  digitalWrite(LED_BUILTIN, HIGH);
  if (slave_reg == REGISTER_0) {
    Wire.write((uint8_t *)&reg_0_value, sizeof(reg_0_value));
    Serial.print("Slave writing value ");
    Serial.println(reg_0_value);
  } else if (slave_reg == REGISTER_1) {
    Wire.write((uint8_t *)&reg_1_value, sizeof(reg_1_value));
    Serial.print("Slave writing value ");
    Serial.println(reg_1_value);
  } else if (slave_reg == REGISTER_2) {
    Wire.write((uint8_t *)&reg_2_value, sizeof(reg_2_value));
    Serial.print("Slave writing value ");
    Serial.println(reg_2_value);
  } else if (slave_reg == REGISTER_3) {
    Wire.write((uint8_t *)&reg_3_value, sizeof(reg_3_value));
    Serial.print("Slave writing value ");
    Serial.println(reg_3_value);
  } else {
    Serial.println("Slave unknown register");
  }
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
}
