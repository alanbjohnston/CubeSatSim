//  Pico code to get camera image from ESP32-CAM running the esp32-cam-send-jpeg-serial software
//
//  run start_camera() once to initialize. Also runs get_camera_image and returns true if got an image
//
//  run get_camera_image() to get image.  If returns true, an image was recevied.  If false,
//  the CAMERA_TIMEOUT milli second timeout was reached - might indicate no camera
//

#include <LittleFS.h>
#include <FastCRC.h>

bool finished = false;

char buffer2[100001];
int index1 = 0;
char start_flag[] = "3d99de816e5ad7742b61a37c39141783";
char end_flag[] = "f681a5c52351befe0e3524eb1a40f14b7803317a";
int flag_count = 0;
int start_flag_detected = false;
int start_flag_complete = false;
int end_flag_detected = false;
int jpeg_start = 0;
FastCRC8 CRC8;
#define CAMERA_TIMEOUT 5000 // 20000  // Camera timeout in milli seconds
#define ESP32_CAM_ENABLE_PIN 15

//#define GET_IMAGE_DEBUG

//#define DEBUG

//#define PICOW true
int led_pin = LED_BUILTIN;

bool get_camera_image(bool debug_camera);
bool start_camera();

bool start_camera() {
 
  // setup for ESP32-CAM
  
//if (PICOW)
//  led_pin = STEM_LED2_PIN;
  
   Serial.begin(115200);

//   delay(5000);

/* 
   pinMode(led_pin, OUTPUT);

    digitalWrite(led_pin, LOW);
    delay(500);  
    digitalWrite(led_pin, HIGH);  
    delay(500);  
    digitalWrite(led_pin, LOW);  
    delay(500);  
    digitalWrite(led_pin, HIGH); 
    delay(500); 
    if (PICOW)
       digitalWrite(led_pin, LOW); 
    
    delay(2000);
*/
   Serial2.setRX(9);
   delay(100);
   Serial2.setTX(8);
   delay(100);
   Serial2.begin(115200);
#ifdef GET_IMAGE_DEBUG  
   Serial.println("Started Serial2 to camera v0.2");
#endif
   LittleFS.begin();

   bool camera_present = false;	
//   int tries = 0;
//   while ((tries++ < 5) && !camera_present) {
    if (get_camera_image(false))  {
      camera_present = true;
      Serial.println("Camera detected!");
    }	    
//  } 
  if (!camera_present) {
    Serial.println("No camera detected!");
  }
 
   return(camera_present);
}

void show_dir2() {
  int count = 0;
  Dir dir = LittleFS.openDir("/");
// or Dir dir = LittleFS.openDir("/data");
  Serial.println(">");
  while (dir.next()) {
    count++;
    Serial.print(count);
    Serial.print(" ");
    Serial.print(dir.fileName());
  //  if(dir.fileSize()) {
        File f = dir.openFile("r");
        Serial.print(" ");
        Serial.println(f.size());
 //   } else
 //       Serial.println(" ");
  }
  Serial.println(">");
}


void write_jpg() {
/*
  Serial.println("---------------");
  Serial.println(buffer2[jpeg_start], HEX);
  Serial.println(buffer2[jpeg_start + 1], HEX);
  Serial.println(buffer2[jpeg_start + 2], HEX);
  Serial.println(buffer2[jpeg_start + 3], HEX);
*/      
  if ((buffer2[jpeg_start] == 0xff) && (buffer2[jpeg_start + 1] == 0xd8)
       && (buffer2[index1 - 2] == 0xff) && (buffer2[index1 - 1] == 0xd9)) {
 
    Serial.println("Received a JPEG! Writing to file.");
    File i = LittleFS.open("/cam.jpg", "w+");
    if (i) {
      i.write(&buffer2[jpeg_start], (size_t) (index1 - jpeg_start));       
      finished = true;
     } else
        Serial.println("Error opening cam.jpg");
        
//      Serial.println("---------------");
      i.close();
//    }
  } else
    Serial.println("Not a JPEG");
#ifdef GET_IMAGE_DEBUG  
    show_dir2();
#endif 
/*
    delay(2000);
    
    char read_values[2];
    File i = LittleFS.open("/cam.jpg", "r");
    while (i.available()) {
      i.readBytes(read_values, 1);
     char hexValue[5];
     sprintf(hexValue, "%02X", read_values[0]);
     Serial.print(hexValue);    
    }
    i.close();
    Serial.println("\n\n finished read");  

 */      
}

/*
void loop() {

  char input_file[] = "/cam.jpg"; 
  char output_file[] = "/cam.bin"; 
  
  get_image_file();

  Serial.println("Got image from ESP-32-CAM!");

  delay(1000);

}
*/

bool get_camera_image(bool debug_camera)  {
 
  index1 = 0;
  flag_count = 0;
  start_flag_detected = false;
  start_flag_complete = false;
  end_flag_detected = false;
  jpeg_start = 0;
 
  Serial.println("Enabling ESP32-CAM");
  pinMode(ESP32_CAM_ENABLE_PIN, OUTPUT);
  digitalWrite(ESP32_CAM_ENABLE_PIN, HIGH);
 
// #ifdef GET_IMAGE_DEBUG
 if (debug_camera)
  Serial.println("Received from ESP32-CAM hex encoded:\n");
 // #endif
  finished = false;

  unsigned long time_start = millis();	    
  while ((!finished) && ((millis() - time_start) < CAMERA_TIMEOUT)) {

   if (Serial2.available()) {      // If anything comes in Serial2
      byte octet = Serial2.read();
      if (!start_flag_detected)
        Serial.write(octet);

     if (start_flag_complete) {
//       Serial.println("Start flag complete detected");
       buffer2[index1++] = octet;
       if (octet == end_flag[flag_count]) {  // looking for end flag
//         if (end_flag_detected) {
            flag_count++;
#ifdef GET_IMAGE_DEBUG  
//       if (debug_camera)  
            Serial.println("Found part of end flag!");
#endif
            if (flag_count >= strlen(end_flag)) {  // complete image           
///              buffer2[index1++] = octet;
//              Serial.println("\nFound end flag");
//              Serial.println(octet, HEX);
              while(!Serial2.available()) { }     // Wait for another byte
//              octet = Serial2.read(); 
//              buffer2[index1++] = octet;
//              Serial.println(octet, HEX);
//              while(!Serial2.available()) { }     // Wait for another byte
              int received_crc = Serial2.read(); 
//              buffer2[index1++] = octet;
                            
              Serial.print("\nFile length: ");
              Serial.println(index1 - (int)strlen(end_flag));
//              index1 -= 1; // 40;
//              Serial.println(buffer2[index1 - 1], HEX); 
//              int received_crc = buffer2[index1];
//              index1 -= 1;

              uint8_t * data = (uint8_t *) &buffer2[0];
#ifdef GET_IMAGE_DEBUG
       Serial.println(buffer2[0], HEX);
      Serial.println(buffer2[index1 - 1], HEX);
      Serial.println(index1);            
 #endif  
     if (debug_camera) {                
      Serial.print("\nCRC received:");
      Serial.println(received_crc, HEX);   
    }
           
              int calculated_crc = CRC8.smbus(data, index1);
 //             Serial.println(calculated_crc, HEX);
              if (received_crc == calculated_crc)
                Serial.println("CRC check succeeded!");
              else  
               Serial.println("CRC check failed!"); 

              index1 -= 40;                         
              write_jpg();
              index1 = 0;           
              start_flag_complete = false;
              start_flag_detected = false; // get ready for next image 
              end_flag_detected = false;
              flag_count = 0; 
//              delay(6000);
            }
         } else {
           flag_count = 0;
         }
 ///        buffer2[index1++] = octet;
           
//#ifdef GET_IMAGE_DEBUG 
    if (debug_camera) {   
           char hexValue[5];
           if (octet != 0x66) {
             sprintf(hexValue, "%02X", octet);
             Serial.print(hexValue);
           } else {
//             Serial.println("\n********************************************* Got a 66!");
             Serial.print("66");
           } 
//             Serial.write(octet);
    }
//#endif             
           if (index1 > 100000)
             index1 = 0; 
//         }
    } else if (octet == start_flag[flag_count]) {  // looking for start flag
          start_flag_detected = true;
          flag_count++;
//          Serial.println("Found part of start flag! ");
          if (flag_count >= strlen(start_flag)) {
            flag_count = 0;
            start_flag_complete = true;
//            Serial.println("Found start flag!\n");        
          }
      } else {  // not the flag, keep looking
          start_flag_detected = false;
          flag_count = 0;
     //   Serial.println("Resetting. Not start flag.");        
       } 
    }
//    Serial.println("writing to Serial2");
  }
  if (debug_camera)                 
      Serial.print("\nCamera complete\n");
  digitalWrite(ESP32_CAM_ENABLE_PIN, LOW);
  return(finished);
}
