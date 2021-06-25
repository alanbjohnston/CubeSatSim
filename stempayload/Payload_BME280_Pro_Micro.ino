#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)

//#define TESTING  // Define to test on Serial Monitor

Adafruit_BME280 bme;
int counter = 0;
int RXLED = 17;  // The RX LED has a defined Arduino pin

void setup() {

  Serial.begin(9600); // Serial Monitor for testing

  Serial1.begin(9600);  // Pi UART

  Serial.println("Starting!");


  digitalWrite(RXLED, LOW);   // set the RX LED ON
  TXLED0; //TX LED is not tied to a normally controlled pin so a macro is needed, turn LED OFF
  delay(50);              // wait for a second
  digitalWrite(RXLED, HIGH);   // set the RX LED ON
  TXLED0; //TX LED is not tied to a normally controlled pin so a macro is needed, turn LED OFF

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void loop() {

#ifdef TESTING
  if (Serial.available() > 0) {
    digitalWrite(RXLED, LOW);   // set the RX LED ON
    TXLED0; //TX LED is not tied to a normally controlled pin so a macro is needed, turn LED OFF
    delay(50);              // wait for a second
    digitalWrite(RXLED, HIGH);   // set the RX LED ON
    TXLED0; //TX LED is not tied to a normally controlled pin so a macro is needed, turn LED OFF
    char result = Serial.read();
    //    Serial1.println(result);
    Serial.print("OK BME280 ");
    Serial.print(bme.readTemperature());
    Serial.print(" ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.print(" ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.print(" ");
    Serial.println(bme.readHumidity());

    //  Serial1.println(counter++);
  }
#else
  if (Serial1.available() > 0) {
    digitalWrite(RXLED, LOW);   // set the RX LED ON
    TXLED0; //TX LED is not tied to a normally controlled pin so a macro is needed, turn LED OFF
    delay(50);              // wait for a second
    digitalWrite(RXLED, HIGH);   // set the RX LED ON
    TXLED0; //TX LED is not tied to a normally controlled pin so a macro is needed, turn LED OFF
    char result = Serial1.read();
    //    Serial1.println(result);
//    Serial1.println("OK ");
/**/    
    Serial1.print("OK BME280 ");
    Serial1.print(bme.readTemperature());
    Serial1.print(" ");
    Serial1.print(bme.readPressure() / 100.0F);
    Serial1.print(" ");
    Serial1.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial1.print(" ");
    Serial1.println(bme.readHumidity());
/**/
    //  Serial1.println(counter++);
  }
#endif

  delay(100);
}
