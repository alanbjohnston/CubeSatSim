// Use this template for adding additional sensors

// put your library includes here
#include "Adafruit_SI1145.h"
#include <Adafruit_LIS3MDL.h>

// put your globals here
Adafruit_SI1145 uv = Adafruit_SI1145();
Adafruit_LIS3MDL lis3mdl;

int uvPresent;
int magPresent;

float magRaw = 0;
float magRawAbs = 0;

// put your setup code here
void payload_setup() {

  Serial.println("Starting Si1145 sensor!");

  if (! uv.begin()) {
    Serial.println("Si1145 sensor fault");
    uvPresent = 0;
  } else {
    uvPresent = 1;
  }

  Serial.println("Starting LIS3MDL sensor!");

  if (! lis3mdl.begin_I2C()) {
    Serial.println("LIS3MDL sensor fault");
    magPresent = 0;
  } else {
    magPresent = 1;
  }
}

// put your loop code here
// Very Important: only use print, not println!!
void payload_loop() {

  if (uvPresent) {
    Serial1.print(" SI ");  // chose a 2-3 letter hint for your sensor
    Serial1.print(uv.readVisible());  // Serial1 sends the sensor data to the Pi Zero for transmission
    Serial1.print(" ");
    Serial1.print(uv.readIR());
  
    Serial.print(" SI ");
    Serial.print(uv.readVisible());   // Serial sends the sensor data to the Serial Monitor for debugging
    Serial.print(" ");
    Serial.print(uv.readIR());
  } else {
    Serial1.print(" SI 0.0 0.0");
    Serial.print(" SI 0.0 0.0");        
  }
  if (magPresent) {
    lis3mdl.read();
    magRaw = (((lis3mdl.x + lis3mdl.y + lis3mdl.z) / 3));
    magRawAbs = abs(magRaw);
    
    Serial1.print(" LI ");
    Serial1.print(magRawAbs);
    
    Serial1.print(" LI ");
    Serial.print(magRawAbs);
  } else {
    Serial1.print(" LI 0.0");
    Serial.print(" LI 0.0");
  }
}
