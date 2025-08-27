// Use this template for adding additional sensors
// see example ...

// put your library includes here
#include <stdio.h>
#include <string.h>

// put your globals here


// put your setup code here
void sensor_setup() {
  
  printf("Starting new sensor!\n");

}

// put your loop code here
// Very Important: only use print, not println!!
int sensor_loop(char *sensor_buffer) {

   int sensors = 3;  // set to the number of sensor readings adding.
   sensor_buffer[0] = 0; // make sure buffer is empty
  
   printf("Reading new sensors!\n");
  
   strcpy(sensor_buffer, "NEW 0.0 0.0 0.0");

   printf("New sensor string: %s\n", sensor_buffer);

  return(sensors);
  
}
