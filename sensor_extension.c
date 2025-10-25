// Use this template for adding additional sensors
// see example ...

// put your library includes here
#include <stdio.h>
#include <string.h>

// put your globals here


// put your setup code here
void sensor_setup() {
  
//  printf("Starting new sensor!\n");
//  put sensor initialization code here  

}

// put your sensor loop code here
// Very Important: only use print, not println!!
int sensor_loop(char *sensor_buffer) {

   int sensors = 0;
   sensor_buffer[0] = 0; // make sure buffer is empty
  
//   sensors = 3;  // Add 3 new sensor readings
  
//   printf("Reading new sensors!\n");
  
//   strcpy(sensor_buffer, "NEW 0.0 0.0 0.0");

//   printf("New sensor string: %s\n", sensor_buffer);

  return(sensors);
  
}
