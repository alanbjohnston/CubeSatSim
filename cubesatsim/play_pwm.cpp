#include <LittleFS.h>

/*
void play_pwm_from_serial(int dds_pwm_pin) {
  
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
#define CAMERA_TIMEOUT 12000  // Camera timeout in milli seconds

//#define GET_IMAGE_DEBUG

//#define DEBUG

//#define PICOW true
int led_pin = LED_BUILTIN;

  

  Serial.println("Playing PWM file from serial");		
  
//  set_sys_clock_khz(133000, true);
	
//  #define DDS_PWM_PIN 26
//   = DDS_PWM_PIN;	
//  int clock = RATE; // 11.025E3; // was 22E3 50E3;
  float multiplier;
  int wrap = WRAP;  // was 10; // 5;
  int dds_pin_slice;
  pwm_config dds_pwm_config;
  int period = 1E6 / g_rate;  // clock;
  char octet;
  byte lower;
  byte upper;
  Serial.println("Playing PWM file");	
  unsigned long sstv_micro_timer;
	 
    dds_pwm_pin = 26;
   
//    multiplier = 133E6 / (clock * (wrap + 1));
    multiplier = 133E6 / (g_rate * (wrap + 1));
//    multiplier = 125E6 / (clock * (wrap + 1));
	
//    isr_period = (int) ( 1E6 / clock + 0.5);
    
    Serial.printf("Pico PWM Playback v0.3 begin\nClock: %d Wrap: %d Multiplier: %4.1f Period: %d\n", g_rate, wrap, multiplier, period);

    gpio_set_function(dds_pwm_pin, GPIO_FUNC_PWM);
    dds_pin_slice = pwm_gpio_to_slice_num(dds_pwm_pin);

    dds_pwm_config = pwm_get_default_config();
    pwm_config_set_clkdiv(&dds_pwm_config, multiplier); // was 100.0 50 75 25.0); // 33.333);  // 1.0f
    pwm_config_set_wrap(&dds_pwm_config, wrap); // 3 
    pwm_init(dds_pin_slice, &dds_pwm_config, true);
    pwm_set_gpio_level(dds_pwm_pin, 0); // (dds_pwm_config.top + 1) * 0.5);
  
//    Serial.printf("PWM config.top: %d\n", dds_pwm_config.top);
	
//  delay(1000);	 
	
// while (true) {	
	
//  output_file = LittleFS.open("/cam.pwm", "r");
  
   Serial2.setRX(9);
   delay(100);
   Serial2.setTX(8);
   delay(100);
   Serial2.begin(115200);
#ifdef GET_DEBUG  
   Serial.println("Started Serial2 to PWM v0.1");
#endif
   LittleFS.begin();  
 
  index1 = 0;
  flag_count = 0;
  start_flag_detected = false;
  start_flag_complete = false;
  end_flag_detected = false;
  jpeg_start = 0;
 
#ifdef GET_DEBUG
  Serial.println("Starting get_pwm_file");
 #endif
  finished = false;
 
  unsigned long time_start = millis();	    
  while ((!finished) && ((millis() - time_start) < CAMERA_TIMEOUT)) {

   if (Serial2.available()) {      // If anything comes in Serial2
      byte octet = Serial2.read(); 

     if (start_flag_complete) {
//       Serial.println("Start flag complete detected");
       buffer2[index1++] = octet;
       if (octet == end_flag[flag_count]) {  // looking for end flag
//         if (end_flag_detected) {
            flag_count++;
#ifdef GET_DEBUG        
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
                            
              Serial.print("File length: ");
              Serial.println(index1 - (int)strlen(end_flag));
//              index1 -= 1; // 40;
//              Serial.println(buffer2[index1 - 1], HEX); 
//              int received_crc = buffer2[index1];
//              index1 -= 1;

              uint8_t * data = (uint8_t *) &buffer2[0];
#ifdef GET_DEBUG
      Serial.println("\nCRC cacluation data:");
      Serial.println(buffer2[0], HEX);
      Serial.println(buffer2[index1 - 1], HEX);
      Serial.println(index1);
      Serial.println(received_crc, HEX);     
 #endif             
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
           
#ifdef GET_DEBUG    
           char hexValue[5];
           if (octet != 0x66) {
             sprintf(hexValue, "%02X", octet);
             Serial.print(hexValue);
           } else {
//             Serial.println("\n********************************************* Got a 66!");
             Serial.print("66");
           } 
//             Serial.write(octet);
#endif             
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
  }
   
  
  // once PWM stream is detected
	
  long prompt_count_max = 1E6 / period;
  long prompt_count = 0;	
	
  sstv_micro_timer = micros();		
  while (output_file.available() && !sstv_stop) {	
//  while (!sstv_stop) {	  
	  
//    output_file.readBytes(&octet, 1);
    lower = octet & 0x0f;
    upper = (octet & 0xf0) >> 4;
//    Serial.printf("%d\n%d\n", lower, upper);	
	  
    while ((micros() - sstv_micro_timer) < period)	{ }   	  
    pwm_set_gpio_level(dds_pwm_pin, lower);
    sstv_micro_timer = micros();	
	  
    while ((micros() - sstv_micro_timer) < period)    { }   	
    pwm_set_gpio_level(dds_pwm_pin, upper);
    sstv_micro_timer = micros();
	  
    prompt_count++;
    if (prompt_count > prompt_count_max) {
	prompt_count = 0;
//	Serial.println("Prompt!\n");   
	if (Serial.available() || BOOTSEL || !digitalRead(10))
	  sstv_stop = true;  	 
    }
	  
  }
	
  Serial.println("End");
//  output_file.close();	
	
//  delay(5000);	 	 
// }	 
}


void play_pwm_from_file(int dds_pwm_pin) {
	
#ifdef ESP32
  Serial.println("Playing PWM file");		
#else
//  set_sys_clock_khz(133000, true);
	
//  #define DDS_PWM_PIN 26
//   = DDS_PWM_PIN;	
//  int clock = RATE; // 11.025E3; // was 22E3 50E3;
  float multiplier;
  int wrap = WRAP;  // was 10; // 5;
  int dds_pin_slice;
  pwm_config dds_pwm_config;
  int period = 1E6 / g_rate;  // clock;
  char octet;
  byte lower;
  byte upper;
  Serial.println("Playing PWM file");	
  unsigned long sstv_micro_timer;
	 
    dds_pwm_pin = 26;
   
//    multiplier = 133E6 / (clock * (wrap + 1));
    multiplier = 133E6 / (g_rate * (wrap + 1));
//    multiplier = 125E6 / (clock * (wrap + 1));
	
//    isr_period = (int) ( 1E6 / clock + 0.5);
    
    Serial.printf("Pico PWM Playback v0.3 begin\nClock: %d Wrap: %d Multiplier: %4.1f Period: %d\n", g_rate, wrap, multiplier, period);

    gpio_set_function(dds_pwm_pin, GPIO_FUNC_PWM);
    dds_pin_slice = pwm_gpio_to_slice_num(dds_pwm_pin);

    dds_pwm_config = pwm_get_default_config();
    pwm_config_set_clkdiv(&dds_pwm_config, multiplier); // was 100.0 50 75 25.0); // 33.333);  // 1.0f
    pwm_config_set_wrap(&dds_pwm_config, wrap); // 3 
    pwm_init(dds_pin_slice, &dds_pwm_config, true);
    pwm_set_gpio_level(dds_pwm_pin, 0); // (dds_pwm_config.top + 1) * 0.5);
  
//    Serial.printf("PWM config.top: %d\n", dds_pwm_config.top);
	
//  delay(1000);	 
	
// while (true) {	
	
  output_file = LittleFS.open("/cam.pwm", "r");
	
  long prompt_count_max = 1E6 / period;
  long prompt_count = 0;	
	
  sstv_micro_timer = micros();		
  while (output_file.available() && !sstv_stop) {	
//  while (!sstv_stop) {	  
	  
    output_file.readBytes(&octet, 1);
    lower = octet & 0x0f;
    upper = (octet & 0xf0) >> 4;
//    Serial.printf("%d\n%d\n", lower, upper);	
	  
    while ((micros() - sstv_micro_timer) < period)	{ }   	  
    pwm_set_gpio_level(dds_pwm_pin, lower);
    sstv_micro_timer = micros();	
	  
    while ((micros() - sstv_micro_timer) < period)    { }   	
    pwm_set_gpio_level(dds_pwm_pin, upper);
    sstv_micro_timer = micros();
	  
    prompt_count++;
    if (prompt_count > prompt_count_max) {
	prompt_count = 0;
//	Serial.println("Prompt!\n");   
	if (Serial.available() || BOOTSEL || !digitalRead(10))
	  sstv_stop = true;  	 
    }
	  
  }
	
  Serial.println("End");
  output_file.close();	
#endif
	
//  delay(5000);	 	 
// }	 
}

*/
bool get_pwm_file()  {
  
   Serial2.setRX(9);
   delay(100);
   Serial2.setTX(8);
   delay(100);
   Serial2.begin(115200);
#ifdef GET_DEBUG  
   Serial.println("Started Serial2 to PWM File v0.1");
#endif
   LittleFS.begin();  
   
  
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
#define CAMERA_TIMEOUT 12000  // Camera timeout in milli seconds

//#define GET_IMAGE_DEBUG

//#define DEBUG

//#define PICOW true
int led_pin = LED_BUILTIN;

    
 
  index1 = 0;
  flag_count = 0;
  start_flag_detected = false;
  start_flag_complete = false;
  end_flag_detected = false;
  jpeg_start = 0;
 
#ifdef GET_DEBUG
  Serial.println("Starting get_pwm_file");
 #endif
  finished = false;
  
  File i = LittleFS.open("/cam.pwm", "w+");
  if (i) {
; //      i.write(&buffer2[jpeg_start], (size_t) (index1 - jpeg_start));       
  //    finished = true;
 } else
        Serial.println("Error opening cam.pwm");
 
  unsigned long time_start = millis();	    
  while ((!finished) && ((millis() - time_start) < CAMERA_TIMEOUT)) {

   if (Serial2.available()) {      // If anything comes in Serial2
      byte octet = Serial2.read(); 

     if (start_flag_complete) {
//       Serial.println("Start flag complete detected");
//       buffer2[index1++] = octet;
         i.write(&octet, 1);
         index1++;
       
       if (octet == end_flag[flag_count]) {  // looking for end flag
//         if (end_flag_detected) {
            flag_count++;
#ifdef GET_IMAGE_DEBUG        
            Serial.println("Found part of end flag!");
#endif
            if (flag_count >= strlen(end_flag)) {  // complete image           
///              buffer2[index1++] = octet;
//              Serial.println("\nFound end flag");
//              Serial.println(octet, HEX);
///              while(!Serial2.available()) { }     // Wait for another byte
//              octet = Serial2.read(); 
//              buffer2[index1++] = octet;
//              Serial.println(octet, HEX);
//              while(!Serial2.available()) { }     // Wait for another byte
///              int received_crc = Serial2.read(); 
//              buffer2[index1++] = octet;
                            
              Serial.print("File length: ");
              Serial.println(index1 - (int)strlen(end_flag));
              
              finished = true;
//              index1 -= 1; // 40;
//              Serial.println(buffer2[index1 - 1], HEX); 
//              int received_crc = buffer2[index1];
//              index1 -= 1;

/*              
              uint8_t * data = (uint8_t *) &buffer2[0];
#ifdef GET_DEBUG
      Serial.println("\nCRC cacluation data:");
      Serial.println(buffer2[0], HEX);
      Serial.println(buffer2[index1 - 1], HEX);
      Serial.println(index1);
      Serial.println(received_crc, HEX);     
 #endif             
              int calculated_crc = CRC8.smbus(data, index1);
 //             Serial.println(calculated_crc, HEX);
              if (received_crc == calculated_crc)
                Serial.println("CRC check succeeded!");
              else  
               Serial.println("CRC check failed!"); 
*/
              index1 -= 40;                         
           
              
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
           
#ifdef GET_DEBUG    
           char hexValue[5];
           if (octet != 0x66) {
             sprintf(hexValue, "%02X", octet);
             Serial.print(hexValue);
           } else {
//             Serial.println("\n********************************************* Got a 66!");
             Serial.print("66");
           } 
//             Serial.write(octet);
#endif             
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
  }
  i.close();

  return(finished);
}
