#include "Adafruit_SI1145.h"
#include <Adafruit_LIS3MDL.h>

Adafruit_SI1145 uv = Adafruit_SI1145();
Adafruit_LIS3MDL lis3mdl;

int uvPresent;
int magPresent;

float magRaw = 0;
float magRawAbs = 0;

void payload_setup() {

  if (! uv.begin()) {
    Serial.println("Si1145 sensor fault");
    uvPresent = 0;
  } else {
    uvPresent = 1;
  }

  if (! lis3mdl.begin_I2C()) {
    Serial.println("LIS3MDL sensor fault");
    magPresent = 0;
  } else {
    magPresent = 1;
  }
}

void payload_loop() {

        if (uvPresent) {
          Serial1.print(uv.readVisible());
          Serial1.print(" ");
          Serial1.print(uv.readIR());
          Serial1.print(" ");

          Serial.print(uv.readVisible());
          Serial.print(" ");
          Serial.print(uv.readIR());
          Serial.print(" ");          
      } else
      {
        Serial1.print("0.0 0.0 ");
        Serial.print("0.0 0.0 ");        
      }
      if (magPresent) {
        lis3mdl.read();
        magRaw = (((lis3mdl.x + lis3mdl.y + lis3mdl.z) / 3));
        magAbs = abs(magRaw);
        Serial1.print(magAbs);
        Serial.print(magAbs);
      } else
      {
        Serial1.print("0.0");
        Serial.print("0.0");
      }
}
