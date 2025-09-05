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

   int sensors = 4;  // set to the number of sensor readings adding.
   sensor_buffer[0] = 0; // make sure buffer is empty
  
   printf("Reading new sensors!\n");

   FILE *sensor_read = sopen("/home/pi/raspberry-pi-bme280/bme280");  // read BME if present 
   fgets(sensor_buffer, 1000, sensor_read);
//   fprintf(stderr, "result: %s\n", sensor_buffer);
   fclose(sensor_read);
  
//   strcpy(sensor_buffer, "NEW 0.0 0.0 0.0");

   printf("New sensor string: %s\n", sensor_buffer);

  return(sensors);
  
}
