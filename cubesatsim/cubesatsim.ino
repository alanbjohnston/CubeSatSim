/*
 *  Transmits CubeSat Telemetry at 432.25MHz in AFSK, FSK, BPSK, or CW format
 *
 *  Copyright Alan B. Johnston
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// This code is an Arduino sketch for the Raspberry Pi Pico
// based on the Raspberry Pi Code

//#define PICO_0V1 // define for Pico v0.1 hardware

#include "cubesatsim.h"
#include "DumbTXSWS.h"
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <MPU6050_tockn.h>
#include <EEPROM.h>
#include <Arduino-APRS-Library.h>
#include <stdio.h>
#include "pico/stdlib.h"   // stdlib 
#include "hardware/irq.h"  // interrupts
#include "hardware/pwm.h"  // pwm 
#include "hardware/sync.h" // wait for interrupt 
#include "RPi_Pico_ISR_Timer.h"
#include "RPi_Pico_TimerInterrupt.h"
//#include <WiFi.h>
#include "hardware/gpio.h"
#include "hardware/adc.h"
//#include "SSTV-Arduino-Scottie1-Library.h"
#include "LittleFS.h"
#include <Adafruit_SI5351_Library.h>
#include "picosstvpp.h"
#include "pico/bootrom.h"
#include "hardware/watchdog.h"

// jpg files to be stored in flash storage on Pico (FS 512kB setting)
#include "sstv1.h"
#include "sstv2.h"

Adafruit_INA219 ina219_1_0x40;
Adafruit_INA219 ina219_1_0x41(0x41);
Adafruit_INA219 ina219_1_0x44(0x44);
//Adafruit_INA219 ina219_1_0x45(0x45);
Adafruit_INA219 ina219_2_0x40(0x40);
Adafruit_INA219 ina219_2_0x41(0x41);
Adafruit_INA219 ina219_2_0x44(0x44);
Adafruit_INA219 ina219_2_0x45(0x45);

Adafruit_SI5351 clockgen = Adafruit_SI5351();

unsigned long micros3;

//WiFiServer server(port);
//WiFiClient client;

//#define PICO_W    // define if Pico W board.  Otherwise, compilation fail for Pico or runtime fail if compile as Pico W

byte green_led_counter = 0;
char call[] = "AMSAT";   // put your callsign here

extern bool get_camera_image();
extern bool start_camera();

void setup() {

//  set_sys_clock_khz(133000, true);  
	
  Serial.begin(115200);
	
  delay(2000);	
	
  LittleFS.begin();
//  LittleFS.format();	// only format if files of size 0 keep showing up

  read_mode();
	
  // mode = BPSK;	// force to BPSK

  new_mode = mode;
	
  pinMode(LED_BUILTIN, OUTPUT);	
//  blinkTimes(1);	

///  sleep(5.0);	
	
// otherwise, run CubeSatSim Pico code
  
  Serial.println("CubeSatSim Pico v0.33 starting...\n");
	
  config_gpio();	
	
  start_clockgen();		
	
  EEPROM.begin(512);	
	
#ifndef ARDUINO_ARCH_RP2040
  Serial.println("This code is written for the Raspberry Pi Pico hardware.");
#endif	

// detect Pi Zero using 3.3V
// if Pi is present, run Payload OK software

///  load_files();			
/*	
  pinMode(PI_3V3_PIN, INPUT); 	
  Serial.print("Pi 3.3V: ");
  Serial.println(digitalRead(PI_3V3_PIN));
  if (digitalRead(PI_3V3_PIN) == HIGH)  {
    Serial.print("Pi Zero present, so running Payload OK code instead of CubeSatSim code.");
    start_payload();	
    while(true)	 { 
      payload_OK_only();    
    }
  }
*/
// configure STEM Payload sensors
	
//  pinMode(PI_3V3_PIN, OUTPUT);
//  digitalWrite(PI_3V3_PIN, HIGH);

//  Serial.println("CubeSatSim Pico v0.33 starting...\n");	
	
  start_payload();  // above code not working, so forcing it
	
  read_config_file();	
	
/*	
  sim_mode = FALSE;
  if (sim_mode)
    config_simulated_telem();
  else
    ; // configure ina219s
*/    	  
  start_ina219();
	
  if (i2c_bus3 == false) 
    config_simulated_telem();		
    
/*	
  if (check_for_wifi()) {
     wifi = true;	
     configure_wifi();	  
  }
*/	
///  start_button_isr(); 
	
//  setup_sstv();
  picosstvpp_begin(26);
  camera_detected = start_camera();	

//  start_pwm();
  program_radio();	
	
/**/
  Serial.println("Transmitting callsign");	
//  strcpy(callsign, call);	
  if (mode != CW)
    transmit_callsign(callsign);
//  sleep(5.0);		
  sleep(1.0);		
/**/
	
  config_telem();	

//  start_button_isr(); 	// try before start_isr
	
  start_isr();	
	
// setup radio depending on mode 
  config_radio();	
		
///  start_button_isr(); 
	
  sampleTime = (unsigned int) millis();		
	
  ready = TRUE;  // flag for core1 to start looping
	
  prompt = PROMPT_HELP;  // display input help menu	
  prompt_for_input();
	
  Serial.print("s");
  Serial.print(" ");
  Serial.println(millis());	
}

void loop() {
		
  int startSleep = millis();	
  loop_count++;
	
  if (sim_mode == TRUE) 
    generate_simulated_telem();
  else
  // query INA219 sensors and Payload sensors
   read_ina219();	
	
  read_payload();	
  
  // encode as digits (APRS or CW mode) or binary (DUV FSK)	
  if ((mode == BPSK) || (mode == FSK))  {
    get_tlm_fox();
  }
  else if ((mode == AFSK) || (mode == CW)) {
    get_tlm_ao7();	
    if (mode == AFSK) {  
      send_aprs_packet();
    } else if (mode == CW) {
      Serial.printf("DE %s \n", callsign);	    
      send_cw();
    }
  }
  else if (mode == SSTV)
  {	  
//      first_time_sstv = false;	  
      char image_file[128];
      if (first_time_sstv) {  
//      if (false) {    // turn this off for now
//        strcpy(image_file, sstv1_filename);
	  Serial.println("Using stored image file"); 
	  load_sstv_image_1_as_cam_dot_jpg(); 	      
	first_time_sstv = false;
      } else {
	if (camera_detected = get_camera_image()) {      
          Serial.println("Getting image file");   
//          Serial.println("Got image file");	      
//	  char camera_file[] = "/cam.jpg";      
//	  strcpy(image_file, camera_file);      
	} else	 {     
//	  strcpy(image_file, sstv1_filename);     // 2nd stored image
	  Serial.println("Using stored image file"); 
	  load_sstv_image_2_as_cam_dot_jpg(); 
	}
      }  
/*	  
      if (debug_mode)  {	  
        Serial.print("\nSending SSTV image ");
        print_string(image_file);
      }
*/	  
//      send_sstv("/cam.raw");
	  
//      send_sstv(image_file);
//      LittleFS.remove("/cam.bin");	  
      show_dir();
      char input_file[] = "/cam.jpg";	  
//      char output_file2[] = "/cam2.bin"; 	  
      char output_file[] = "/cam.bin"; 
//      jpeg_decode(image_file, output_file, true); // debug_mode);
      jpeg_decode(input_file, output_file, true); // debug_mode);
      show_dir();	  
//      char telem_display[] = " BATT:    STATUS:   TEMP:  ";	  

      digitalWrite(PTT_PIN, HIGH);  // shouldn't need this but
//      rotate_image(output_file2, output_file, telem_display);	  
//      show_dir();

      picosstvpp();	
	  
      if (debug_mode)	  	  
        Serial.println("Start transmit!!!");
      digitalWrite(PTT_PIN, LOW);  // start transmit
//      if (!wifi) 
        digitalWrite(LED_BUILTIN, HIGH);	
      digitalWrite(MAIN_LED_BLUE, HIGH);	    

//      scottie1_transmit_file(output_file, debug_mode);
 
//      ITimer1.stopTimer();	// turn off pushbutton timer  	   
///      ITimer1.disableTimer();	// turn off pushbutton timer  	  
      play_pwm_file(26);
//      ITimer1.restartTimer();	// turn back on pushbutton timer  	  
///      ITimer1.enableTimer();	// turn back on pushbutton timer  	  
      if (debug_mode)	  
        Serial.println("Stop transmit!");
      digitalWrite(PTT_PIN, HIGH);  // stop transmit
//      if (!wifi) 
        digitalWrite(LED_BUILTIN, LOW);	
      digitalWrite(MAIN_LED_BLUE, LOW);	    
	  
      if (debug_mode)	  
        Serial.println("\nImage sent!");
	  
  } 
  else
      Serial.println("Unknown mode!");

	
//  while ((millis() - sampleTime) < ((unsigned int)samplePeriod)) // - 250))  // was 250 100
  while ((millis() - sampleTime) < ((unsigned int)frameTime)) // - 250))  // was 250 100
    sleep(0.01); // was 0.1 sec
  sampleTime = (unsigned int) millis();	  
	
//  delay(2000);
//  test_radio();

  if ((mode == FSK) || (mode == BPSK)) {
//	  if (!wifi) 
	    digitalWrite(LED_BUILTIN, LOW);	
	  digitalWrite(MAIN_LED_BLUE, LOW);

	//  delay(3000);	
	  sleep(0.2); // 2.845); // 3.0);

//	  if (!wifi) 
	   	  digitalWrite(LED_BUILTIN, HIGH);
	  digitalWrite(MAIN_LED_BLUE, HIGH);
  }
	
	
  serial_input();  
	
// check for button press 
  if (digitalRead(MAIN_PB_PIN) == PRESSED) // pushbutton is pressed
      process_pushbutton();
  if (BOOTSEL)	  // boot selector button is pressed on Pico
      process_bootsel();
	
  if (prompt) {
//    Serial.println("Need to prompt for input!");
    prompt_for_input();	  
    prompt = false;	  
  }
	
  // check to see if the mode has changed
 if (mode != new_mode) {
    Serial.println("Changing mode");
    cw_stop = false; // enable CW or won't hear CW ID	 
///    if (mode == SSTV) {
///      ITimer1.detachInterrupt();	    
///      start_button_isr();  // restart button isr
///    }
    int old_mode = mode;
    bool config_done = false;
    mode = new_mode;  // change modes if button pressed	 
    write_mode();
/*	 
//    if ((mode == BPSK) || ((new_mode == FSK) && (old_mode == CW)))	 {
    if (mode == BPSK) 	 {
      config_telem();  // run this before cw only for BPSK mode
      config_done = true;	    
    }
*/	
    Serial.println("Rebooting...");	 
    watchdog_reboot (0, SRAM_END, 10);	 // restart Pico
	 
    sleep(20.0);	 
	 
    if (new_mode != CW)
      transmit_callsign(callsign);
    sleep(0.5);
	 
    if (!config_done)	 
      config_telem();  // run this here for all other modes
	 
    config_radio();
    if ((mode == FSK) || (mode == BPSK)) {    
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(MAIN_LED_BLUE, HIGH);	    
    }
	
    sampleTime = (unsigned int) millis();	 	 
 }		
	
  //  Calculate loop time
  if (debug_mode) {	
    Serial.print("\nLoop time: ");	
    Serial.println(millis() - startSleep);
  }
}	

bool TimerHandler1(struct repeating_timer *t) {
	
  serial_input();
	
// check for button press 
  if (digitalRead(MAIN_PB_PIN) == PRESSED) // pushbutton is pressed
      process_pushbutton();
  if (BOOTSEL)	  // boot selector button is pressed on Pico
      process_bootsel();

//  if (wifi) 
//    check_for_browser();
	
  return(true);	
}

void read_reset_count() {
	
  long stored_reset, reset_flag;
  EEPROM.get(16, reset_flag);
	  
  if (reset_flag == 0xA07)  // not first time, read stored reset count
  {	

    EEPROM.get(20, stored_reset);
    reset_count = (int) stored_reset;
    Serial.print("Reading reset count from EEPROM as ");	
    Serial.println(reset_count);		  
    stored_reset += 1;	  // increment for next boot
    EEPROM.put(20, stored_reset);
    if (EEPROM.commit()) {
      Serial.println("EEPROM successfully committed");
    } else {
      Serial.println("ERROR! EEPROM commit failed");
    }	
  } else  {                  // first time, store flag and reset count as 0

    Serial.println("Storing initial reset count in EEPROM");	  
    Serial.println("Reset count is 0");	
    reset_flag = 0xA07;
    EEPROM.put(16, reset_flag);	
    stored_reset = 0;	
    EEPROM.put(20, stored_reset + 1);
    if (EEPROM.commit()) {
      Serial.println("EEPROM successfully committed");
    } else {
      Serial.println("ERROR! EEPROM commit failed");
    }		
  }	  	
}

void read_config_file() {
  char buff[255];
  // Open configuration file with callsign and reset count
  Serial.println("Reading config file");	
  File config_file = LittleFS.open("/sim.cfg", "r");	
//  FILE * config_file = fopen("/sim.cfg", "r");
  if (!config_file) {
    Serial.println("Creating config file.");
//    config_file = fopen("/sim.cfg", "w+");
     config_file = LittleFS.open("/sim.cfg", "w+");		  
	  
//    fprintf(config_file, "%s %d", " ", 100);
//	  sprintf(buff, "%d\n", cnt);
    sprintf(buff, "%s %d", "AMSAT", 0);
    config_file.write(buff, strlen(buff));	  
	  
	  
    config_file.close();
	  
    config_file = LittleFS.open("/sim.cfg", "r");	  
  }

//  char * cfg_buf[100];
  config_file.read((uint8_t *)buff, 255);
//  sscanf(buff, "%d", &cnt);	
  sscanf(buff, "%s %d %f %f %s", callsign, & reset_count, & lat_file, & long_file, sim_yes);
  config_file.close();
  if (debug_mode)	
    Serial.printf("Config file /sim.cfg contains %s %d %f %f %s\n", callsign, reset_count, lat_file, long_file, sim_yes);
	
  reset_count = (reset_count + 1) % 0xffff;

  if ((fabs(lat_file) > 0) && (fabs(lat_file) < 90.0) && (fabs(long_file) > 0) && (fabs(long_file) < 180.0)) {
    Serial.println("Valid latitude and longitude in config file\n");
// convert to APRS DDMM.MM format
    latitude = lat_file; // toAprsFormat(lat_file);
    longitude = long_file; // toAprsFormat(long_file);
    Serial.printf("Lat/Lon updated to: %f/%f\n", latitude, longitude);
  }
//  } else { // set default
//    latitude = toAprsFormat(latitude);
//    longitude = toAprsFormat(longitude);
//  }
	
  if (strcmp(sim_yes, "yes") == 0)
	  sim_mode = true;	
	
  config_file.close();	
	
  write_config_file();	
}

void write_config_file() {
  Serial.println("Writing /sim.cfg file");	
  char buff[255];	
  File config_file = LittleFS.open("/sim.cfg", "w+");		  
	  
//  sprintf(buff, "%s %d", callsign, );
	
  if (sim_mode)
	strcpy(sim_yes, "yes");
  else
	strcpy(sim_yes, "no");
	
  sprintf(buff, "%s %d %f %f %s", callsign, reset_count, latitude, longitude, sim_yes);
//  Serial.println("Writing string");	
  if (debug_mode)	
    print_string(buff);	
  config_file.write(buff, strlen(buff));	  
	  
  config_file.close();
//  Serial.println("Write complete");	
	
}

void send_aprs_packet() {	
// encode telemetry
  get_tlm_ao7();

//  digitalWrite(LED_BUILTIN, LOW);

  char str[1000];
  char header_str[] = "hi hi ";
  strcpy(str, header_str);	
  strcpy(str, tlm_str);	
  strcat(str, payload_str);
//  print_string(str);
//  Serial.println(strlen(str));	
	
  set_status(str);		
  
  if (debug_mode)	
    Serial.println("Sending APRS packet!");
  transmit_on();
  send_packet(_FIXPOS_STATUS, debug_mode);
  transmit_off();		
}

void send_cw() {
  char de[] = " DE ";	
  char telem[1000];
  char space[] = " ";	

  if (debug_mode)	
    Serial.println("Sending CW packet!");
	
  strcpy(telem, de);
  strcat(telem, callsign);
  strcat(telem, space);
  strcat(telem, tlm_str);  // don't send payload since it isn't encoded and has "."
  if (debug_mode)	
    print_string(telem);
//  Serial.println(strlen(telem));
 
  transmit_string(telem);	
}

void transmit_on() {
  if ((mode == SSTV) || (mode == AFSK)) {  // this isn't quite right for APRS - should only do when sending APRS packet
    if (debug_mode)
      Serial.println("Transmit on!");
    digitalWrite(MAIN_LED_BLUE, HIGH);	
    digitalWrite(PTT_PIN, LOW);  
  } 
  else if ((mode == BPSK) || (mode == FSK)) {
    if (debug_mode)	  
      Serial.println("Transmit on!!!");
//  pwm_set_gpio_level(BPSK_PWM_A_PIN, (config.top + 1) * 0.5);
//  pwm_set_gpio_level(BPSK_PWM_B_PIN, (config.top + 1) * 0.5);	
/*	  
    int ret = 1;
    int i = 0;	  
    while ((i++ < 5) && (ret != 0)) {
      ret = clockgen.enableOutputs(true);	    
      Serial.println("Enable clock outputs!");
    }	
*/	
      if (clockgen.enableOutputs(true)) {	  
	  start_clockgen();
	  if (mode == BPSK)
		  clockgen.setClockBPSK();
	      else
		      clockgen.setClockFSK();
	  clockgen.enableOutputs(true);
	  Serial.println("Enable clock outputs!"); 
      } else {
	 Serial.println("Enable clock outputs");           
      }
  }
  else if (mode == CW) {
 //   Serial.println("Transmit on!");
    cw_stop = false;
  }
  else	  
    Serial.println("No transmit!");
}

void transmit_off() {
  digitalWrite(PTT_PIN, HIGH);
  if (debug_mode)	
   Serial.println("Transmit off!");
  digitalWrite(MAIN_LED_BLUE, LOW);
  digitalWrite(LED_BUILTIN, LOW);	
  if ((mode == BPSK) || (mode == FSK)) {
    digitalWrite(BPSK_CONTROL_A, LOW); 	  
    digitalWrite(BPSK_CONTROL_B, LOW); 	  	  
//    pwm_set_gpio_level(BPSK_PWM_A_PIN, 0);	
//    pwm_set_gpio_level(BPSK_PWM_B_PIN, 0);
/*	  
    int ret = 1;
    int i = 0;	  
    while ((i++ < 5) && (ret != 0)) {
      ret = clockgen.enableOutputs(false);	    
      Serial.println("Disable clock outputs!");
    }	  
//      clockgen.enableOutputs(false)	  
*/	
      if (clockgen.enableOutputs(false)) {	  
	  start_clockgen();    
	  clockgen.enableOutputs(false);
	  Serial.println("Disable clock outputs!");      
      } else {
	 Serial.println("Disable clock outputs");           
      }
	  
  }
  else if (mode == SSTV) 
    sstv_end();
  else if (mode == CW)
    cw_stop = true;
}

void config_telem() {
	
  frameCnt = 1; 
  
  // Serial.println("v1 Present with UHF BPF\n");
  txLed = 2;
  txLedOn = HIGH;
  txLedOff = LOW;
  vB5 = TRUE;
  onLed = 27;
  onLedOn = HIGH;
  onLedOff = LOW;
  transmit = TRUE;  
  
  if (mode == FSK) {
    Serial.println("\nConfiguring for FSK");
    bitRate = 200;
    delay_time = 1E6 / bitRate;		  
//    Serial.println(delay_time);	  
    rsFrames = 1;
    payloads = 1;
    rsFrameLen = 64;
    headerLen = 6;
    dataLen = 58;
    syncBits = 10;
    syncWord = 0b0011111010;
    parityLen = 32;
    amplitude = 32767 / 3;
    sample_rate = 200;	  
//    samples = S_RATE / bitRate;
    samples = sample_rate / bitRate;
//    Serial.println(samples);	  
    bufLen = (frameCnt * (syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))) * samples);
    bufLen = 970; // 2000;	  
//    Serial.println(bufLen);	  
    samplePeriod =  (int) (((float)((syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen)))) / (float) bitRate) * 1000 - 500);
    sleepTime = 0.1;
//    Serial.println(samplePeriod);
	  
    frameTime = ((float)((float)bufLen / (samples * frameCnt * bitRate))) * 1000; // frame time in ms
//    Serial.println(frameTime);
//    printf("\n FSK Mode, %d bits per frame, %d bits per second, %d ms per frame, %d ms sample period\n",
//      bufLen / (samples * frameCnt), bitRate, frameTime, samplePeriod);
    memset(buffer, 0xa5, sizeof(buffer)); 
  } else if (mode == BPSK) {
    Serial.println("\nConfiguring for BPSK");
    bitRate = 1200;
//    delay_time = (1.0 / 1200.0);	
    delay_time = 1E6 / bitRate;	
//    Serial.println(delay_time);	  
    rsFrames = 3;
    payloads = 6;
    rsFrameLen = 159;
    headerLen = 8;
    dataLen = 78;
    syncBits = 31;
    syncWord = 0b1000111110011010010000101011101;
    parityLen = 32;
    amplitude = 32767;
    sample_rate = 1200;	  
//    samples = S_RATE / bitRate;
    samples = sample_rate / bitRate;
//    Serial.println(samples);	
//    bufLen = (frameCnt * (syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))) * samples); // * 2;  // 2 *
    bufLen = 5751; // instead of 5841	  
//    Serial.println(bufLen);	
//    samplePeriod = ((float)((syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))))/(float)bitRate) * 1000 - 500;
    samplePeriod = ((float)((syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))))/(float)bitRate) * 1000; //  - 500;
    //    samplePeriod = 3000;
    //    sleepTime = 3.0;
    //samplePeriod = 2200; // reduce dut to python and sensor querying delays
    sleepTime = 2.2f;
 //   Serial.println(samplePeriod);
    frameTime = ((float)((float)bufLen / (samples * frameCnt * bitRate))) * 1000; // frame time in ms	  
//    Serial.println(frameTime);
//    printf("\n BPSK Mode, bufLen: %d,  %d bits per frame, %d bits per second, %d ms per frame %d ms sample period\n",
//      bufLen, bufLen / (samples * frameCnt), bitRate, frameTime, samplePeriod);

//    sin_samples = S_RATE/freq_Hz;	 		
  //      printf("Sin map: ");	 		
//    for (int j = 0; j < sin_samples; j++) {	 		
//      sin_map[j] = (short int)(amplitude * sin((float)(2 * M_PI * j / sin_samples)));	 		
  //	printf(" %d", sin_map[j]);	 		
  //   }	 		
  //      printf("\n");
//     }
    memset(buffer, 0xa5, sizeof(buffer)); 	  
  } else if (mode == AFSK) {

    Serial.println("\nConfiguring for AFSK");
	  
    set_pin(AUDIO_OUT_PIN);
	  
    char destination[] = "APCSS";	  
    set_callsign(callsign, destination);
	  
    set_lat_lon();
	  
    samplePeriod = 5000;
    frameTime = 5000;	  
    bufLen = 1000;
  }   else if (mode == SSTV) {
//    ITimer1.stopTimer();	// turn off pushbutton timer  
    Serial.println("\nConfiguring for SSTV");
//    set_sstv_pin(AUDIO_OUT_PIN);    	  
    samplePeriod = 5000;
    frameTime = 5000;	  
    bufLen = 1000;
  } else if (mode == CW) {
    Serial.println("\nConfiguring for CW");	  
  }
// clearing min and max values
  if (debug_mode)	
    Serial.println("Clearing min and max telemetry values");	
	
  for (int i = 0; i < 9; i++) {
    voltage_min[i] = 1000.0;
    current_min[i] = 1000.0;
    voltage_max[i] = -1000.0;
    current_max[i] = -1000.0;
  }
  for (int i = 0; i < 17; i++) {
    sensor_min[i] = 1000.0;
    sensor_max[i] = -1000.0;
 //   printf("Sensor min and max initialized!");
  }
  for (int i = 0; i < 3; i++) {
    other_min[i] = 1000.0;
    other_max[i] = -1000.0;
  }		
	
  firstTime = TRUE;	
}

void get_tlm_ao7() {

//  for (int l = 0; l < frameCnt; l++) {
	  
//    fflush(stdout);
//    fflush(stderr);
	  
    int tlm[7][5];
    memset(tlm, 0, sizeof tlm);
	  
    tlm[1][A] = (int)(voltage[mapping[BUS]] / 15.0 + 0.5) % 100; // Current of 5V supply to Pi
    tlm[1][B] = (int)(99.5 - current[mapping[PLUS_X]] / 10.0) % 100; // +X current [4]
    tlm[1][C] = (int)(99.5 - current[mapping[MINUS_X]] / 10.0) % 100; // X- current [10] 
    tlm[1][D] = (int)(99.5 - current[mapping[PLUS_Y]] / 10.0) % 100; // +Y current [7]

    tlm[2][A] = (int)(99.5 - current[mapping[MINUS_Y]] / 10.0) % 100; // -Y current [10] 
    tlm[2][B] = (int)(99.5 - current[mapping[PLUS_Z]] / 10.0) % 100; // +Z current [10] // was 70/2m transponder power, AO-7 didn't have a Z panel
    tlm[2][C] = (int)(99.5 - current[mapping[MINUS_Z]] / 10.0) % 100; // -Z current (was timestamp)
    tlm[2][D] = (int)(50.5 + current[mapping[BAT]] / 10.0) % 100; // NiMH Battery current

//    tlm[3][A] = abs((int)((voltage[mapping[BAT]] * 10.0) - 65.5) % 100);
    if (voltage[mapping[BAT]] > 4.6)	 
    	tlm[3][A] = (int)((voltage[mapping[BAT]] * 10.0) - 65.5) % 100;  // 7.0 - 10.0 V for old 9V battery
    else
    	tlm[3][A] = (int)((voltage[mapping[BAT]] * 10.0) + 44.5) % 100;  // 0 - 4.5 V for new 3 cell battery
	    
    tlm[3][B] = (int)(voltage[mapping[BUS]] * 10.0) % 100; // 5V supply to Pi

//    tlm[4][A] = (int)((95.8 - other[IHU_TEMP]) / 1.48 + 0.5) % 100;  // was [B] but didn't display in online TLM spreadsheet
    tlm[4][A] = (int)((95.8 - analogReadTemp()) / 1.48 + 0.5) % 100;  // was [B] but didn't display in online TLM spreadsheet
		
    tlm[6][B] = 0;
    tlm[6][D] = 49 + rand() % 3;

/*	  
    // Display tlm
    int k, j;
    Serial.print("TLM: ");
    for (k = 1; k < 7; k++) {
      for (j = 1; j < 5; j++) {
        Serial.print(tlm[k][j]);
	Serial.print(" ");
      }
    }
    Serial.println(" ");
*/
    char str[1000];
    int channel;
    char header_str[] = "hi hi ";
    strcpy(tlm_str, header_str);
	
    for (channel = 1; channel < 7; channel++) {
      sprintf(str, "%d%d%d %d%d%d %d%d%d %d%d%d ",
        channel, upper_digit(tlm[channel][1]), lower_digit(tlm[channel][1]),
        channel, upper_digit(tlm[channel][2]), lower_digit(tlm[channel][2]),
        channel, upper_digit(tlm[channel][3]), lower_digit(tlm[channel][3]),
        channel, upper_digit(tlm[channel][4]), lower_digit(tlm[channel][4]));
      //        printf("%s",str);
      strcat(tlm_str, str);
    }
//    print_string(str);
//    strcat(str, payload_str);	
//    print_string(str);
//    Serial.println(strlen(str));
//    set_status(str);	
//  }	
}

void generate_simulated_telem() {

      double time_stamp = ((long int)millis() - time_start) / 1000.0;

      if ((time_stamp - eclipse_time) > period) {
        eclipse = (eclipse == 1) ? 0 : 1;
        eclipse_time = time_stamp;
        if (debug_mode)	      
          Serial.println("\n\nSwitching eclipse mode! \n\n");
      }

      double Xi = eclipse * amps_max[0] * (float) sin(2.0 * 3.14 * time_stamp / (46.0 * rotation_speed)) + rnd_float(-2, 2);
      double Yi = eclipse * amps_max[1] * (float) sin((2.0 * 3.14 * time_stamp / (46.0 * rotation_speed)) + (3.14 / 2.0)) + rnd_float(-2, 2);
      double Zi = eclipse * amps_max[2] * (float) sin((2.0 * 3.14 * time_stamp / (46.0 * rotation_speed)) + 3.14 + angle[2]) + rnd_float(-2, 2);

      double Xv = eclipse * volts_max[0] * (float) sin(2.0 * 3.14 * time_stamp / (46.0 * rotation_speed)) + rnd_float(-0.2, 0.2);
      double Yv = eclipse * volts_max[1] * (float) sin((2.0 * 3.14 * time_stamp / (46.0 * rotation_speed)) + (3.14 / 2.0)) + rnd_float(-0.2, 0.2);
      double Zv = 2.0 * eclipse * volts_max[2] * (float) sin((2.0 * 3.14 * time_stamp / (46.0 * rotation_speed)) + 3.14 + angle[2]) + rnd_float(-0.2, 0.2);

      // printf("Yi: %f Zi: %f %f %f Zv: %f \n", Yi, Zi, amps_max[2], angle[2], Zv);

      current[mapping[PLUS_X]] = (Xi >= 0) ? Xi : 0;
      current[mapping[MINUS_X]] = (Xi >= 0) ? 0 : ((-1.0f) * Xi);
      current[mapping[PLUS_Y]] = (Yi >= 0) ? Yi : 0;
      current[mapping[MINUS_Y]] = (Yi >= 0) ? 0 : ((-1.0f) * Yi);
      current[mapping[PLUS_Z]] = (Zi >= 0) ? Zi : 0;
      current[mapping[MINUS_Z]] = (Zi >= 0) ? 0 : ((-1.0f) * Zi);

      voltage[mapping[PLUS_X]] = (Xv >= 1) ? Xv : rnd_float(0.9, 1.1);
      voltage[mapping[MINUS_X]] = (Xv <= -1) ? ((-1.0f) * Xv) : rnd_float(0.9, 1.1);
      voltage[mapping[PLUS_Y]] = (Yv >= 1) ? Yv : rnd_float(0.9, 1.1);
      voltage[mapping[MINUS_Y]] = (Yv <= -1) ? ((-1.0f) * Yv) : rnd_float(0.9, 1.1);
      voltage[mapping[PLUS_Z]] = (Zv >= 1) ? Zv : rnd_float(0.9, 1.1);
      voltage[mapping[MINUS_Z]] = (Zv <= -1) ? ((-1.0f) * Zv) : rnd_float(0.9, 1.1);

      // printf("temp: %f Time: %f Eclipse: %d : %f %f | %f %f | %f %f\n",tempS, time, eclipse, voltage[map[PLUS_X]], voltage[map[MINUS_X]], voltage[map[PLUS_Y]], voltage[map[MINUS_Y]], current[map[PLUS_Z]], current[map[MINUS_Z]]);

      tempS += (eclipse > 0) ? ((temp_max - tempS) / 50.0f) : ((temp_min - tempS) / 50.0f);
      tempS += +rnd_float(-1.0, 1.0);
      //  IHUcpuTemp = (int)((tempS + rnd_float(-1.0, 1.0)) * 10 + 0.5);
      other[IHU_TEMP] = tempS;

      voltage[mapping[BUS]] = rnd_float(5.0, 5.005);
      current[mapping[BUS]] = rnd_float(158, 171);

      //  float charging = current[map[PLUS_X]] + current[map[MINUS_X]] + current[map[PLUS_Y]] + current[map[MINUS_Y]] + current[map[PLUS_Z]] + current[map[MINUS_Z]];
      float charging = eclipse * (fabs(amps_max[0] * 0.707) + fabs(amps_max[1] * 0.707) + rnd_float(-4.0, 4.0));

      current[mapping[BAT]] = ((current[mapping[BUS]] * voltage[mapping[BUS]]) / batt) - charging;

      //  printf("charging: %f bat curr: %f bus curr: %f bat volt: %f bus volt: %f \n",charging, current[map[BAT]], current[map[BUS]], batt, voltage[map[BUS]]);

      batt -= (batt > 3.5) ? current[mapping[BAT]] / 30000 : current[mapping[BAT]] / 3000;
      if (batt < 3.0) {
        batt = 3.0;
        SafeMode = 1;
        printf("Safe Mode!\n");
      } else
        SafeMode= 0;

      if (batt > 4.5)
        batt = 4.5;

      voltage[mapping[BAT]] = batt + rnd_float(-0.01, 0.01);

      // end of simulated telemetry
}

void config_simulated_telem()
{
    sim_mode = TRUE;
	    
    Serial.println("Simulated telemetry mode!");

//    srand((unsigned int)time(0));

    axis[0] = rnd_float(-0.2, 0.2);
    if (axis[0] == 0)
      axis[0] = rnd_float(-0.2, 0.2);
    axis[1] = rnd_float(-0.2, 0.2);
    axis[2] = (rnd_float(-0.2, 0.2) > 0) ? 1.0 : -1.0;

    angle[0] = (float) atan(axis[1] / axis[2]);
    angle[1] = (float) atan(axis[2] / axis[0]);
    angle[2] = (float) atan(axis[1] / axis[0]);

    volts_max[0] = rnd_float(4.5, 5.5) * (float) sin(angle[1]);
    volts_max[1] = rnd_float(4.5, 5.5) * (float) cos(angle[0]);
    volts_max[2] = rnd_float(4.5, 5.5) * (float) cos(angle[1] - angle[0]);

    float amps_avg = rnd_float(150, 300);

    amps_max[0] = (amps_avg + rnd_float(-25.0, 25.0)) * (float) sin(angle[1]);
    amps_max[1] = (amps_avg + rnd_float(-25.0, 25.0)) * (float) cos(angle[0]);
    amps_max[2] = (amps_avg + rnd_float(-25.0, 25.0)) * (float) cos(angle[1] - angle[0]);

    batt = rnd_float(3.8, 4.3);
    rotation_speed = rnd_float(1.0, 2.5);
    eclipse = (rnd_float(-1, +4) > 0) ? 1.0 : 0.0;
    period = rnd_float(150, 300);
    tempS = rnd_float(20, 55);
    temp_max = rnd_float(50, 70);
    temp_min = rnd_float(10, 20);

//    #ifdef DEBUG_LOGGING
//    for (int i = 0; i < 3; i++)
//      printf("axis: %f angle: %f v: %f i: %f \n", axis[i], angle[i], volts_max[i], amps_max[i]);
//    printf("batt: %f speed: %f eclipse_time: %f eclipse: %f period: %f temp: %f max: %f min: %f\n", batt, speed, eclipse_time, eclipse, period, tempS, temp_max, temp_min);
//    #endif

    time_start = (long int) millis();

    eclipse_time = (long int)(millis() / 1000.0);
    if (eclipse == 0.0)
      eclipse_time -= period / 2; // if starting in eclipse, shorten interval	
//  }

//  tx_freq_hz -= tx_channel * 50000;
	
}

void get_tlm_fox() {
//  Serial.println("get_tlm_fox");
	
  int i;
  long int sync = syncWord;
  smaller = (int) (S_RATE / (2 * freq_Hz));
  short int b[dataLen];
  short int b_max[dataLen];
  short int b_min[dataLen];
	
  memset(b, 0, sizeof(b));
  memset(b_max, 0, sizeof(b_max));
  memset(b_min, 0, sizeof(b_min));
	
  short int h[headerLen];
  memset(h, 0, sizeof(h));
//  memset(buffer, 0xa5, sizeof(buffer));
  short int rs_frame[rsFrames][223];
  unsigned char parities[rsFrames][parityLen], inputByte;
  int id, frm_type = 0x01, NormalModeFailure = 0, groundCommandCount = 0;
  int PayloadFailure1 = 0, PayloadFailure2 = 0;
  int PSUVoltage = 0, PSUCurrent = 0, Resets = 0, Rssi = 2048;
  int batt_a_v = 0, batt_b_v = 0, batt_c_v = 0, battCurr = 0;
  int posXv = 0, negXv = 0, posYv = 0, negYv = 0, posZv = 0, negZv = 0;
  int posXi = 0, negXi = 0, posYi = 0, negYi = 0, posZi = 0, negZi = 0;
  int head_offset = 0;
  //short int buffer_test[bufLen];
  //int buffSize;
  //buffSize = (int) sizeof(buffer_test);
	
  ctr = 0;  	
	
  parse_payload();	
	
//  for (int n = 0; n < 17; n++)
//	  sensor[n] = 1.0;
	
  if (mode == FSK)
    id = 7;
  else
    id = 0; // 99 in h[6]
//  Serial.println("About to do frame loop");
	
  //  for (int frames = 0; frames < FRAME_CNT; frames++) 
  for (int frames = 0; frames < frameCnt; frames++) {
//    Serial.println("Frame loop"); 
	
//    if (mode == FSK) 
    {  // just moved
      for (int count1 = 0; count1 < 8; count1++) {
        if (voltage[count1] < voltage_min[count1])
          voltage_min[count1] = voltage[count1];
        if (current[count1] < current_min[count1])
          current_min[count1] = current[count1];
	      
        if (voltage[count1] > voltage_max[count1])
          voltage_max[count1] = voltage[count1];
        if (current[count1] > current_max[count1])
          current_max[count1] = current[count1];
//         printf("Vmin %4.2f Vmax %4.2f Imin %4.2f Imax %4.2f \n", voltage_min[count1], voltage_max[count1], current_min[count1], current_max[count1]);
//	Serial.print(voltage_min[count1]);
//	Serial.print(" ");
      }
//      Serial.println(" ");	    
       for (int count1 = 0; count1 < 3; count1++) {
        if (other[count1] < other_min[count1])
          other_min[count1] = other[count1];
        if (other[count1] > other_max[count1])
          other_max[count1] = other[count1];
//	Serial.print(other_min[count1]);
//	Serial.print(" ");
      }
//      Serial.println(" ");
          if (mode == FSK)
	  {
//	      Serial.println("Starting");	  
	      if (loop_count % 32 == 0) {  // was 8  /// was loop now loop_count
		if (debug_mode)      
		  Serial.println("Sending MIN frame");
		frm_type = 0x03;
		for (int count1 = 0; count1 < 17; count1++) {
		  if (count1 < 3)
		    other[count1] = other_min[count1];
		  if (count1 < 8) {
		    voltage[count1] = voltage_min[count1];
		    current[count1] = current_min[count1];
		  }
		  if (sensor_min[count1] != 1000.0) // make sure values are valid
		    sensor[count1] = sensor_min[count1];
		}
	      }
	      if ((loop_count + 16) % 32 == 0) {  // was 8
		if (debug_mode)      
		  Serial.println("Sending MAX frame");
		frm_type = 0x02;
		for (int count1 = 0; count1 < 17; count1++) {
		  if (count1 < 3)
		    other[count1] = other_max[count1];
		  if (count1 < 8) {
		    voltage[count1] = voltage_max[count1];
		    current[count1] = current_max[count1];
		  }
		  if (sensor_max[count1] != -1000.0) // make sure values are valid
		    sensor[count1] = sensor_max[count1];
		}
	      }
//	      Serial.println("Here");	  
	  }
	  else
	  	frm_type = 0x02;  // BPSK always send MAX MIN frame
    } 
//    Serial.println("Continuing");	  
    sensor_payload[0] = 0;  // clear for next payload
	  
//   if (mode == FSK) {	// remove this 
//   }
    memset(rs_frame, 0, sizeof(rs_frame));
    memset(parities, 0, sizeof(parities));
//    Serial.println("After memset");   
   uptime = (int)(millis() / 1000.0);	  

    h[0] = (short int) ((h[0] & 0xf8) | (id & 0x07)); // 3 bits
//    Serial.println("After h[0]");	  
     if (uptime != 0)	  // if uptime is 0, leave reset count at 0
    {
//      Serial.println("After uptime test");
      h[0] = (short int) ((h[0] & 0x07) | ((reset_count & 0x1f) << 3));
      h[1] = (short int) ((reset_count >> 5) & 0xff);
      h[2] = (short int) ((h[2] & 0xf8) | ((reset_count >> 13) & 0x07));
    }
    h[2] = (short int) ((h[2] & 0x0e) | ((uptime & 0x1f) << 3));
    h[3] = (short int) ((uptime >> 5) & 0xff);
    h[4] = (short int) ((uptime >> 13) & 0xff);
    h[5] = (short int) ((h[5] & 0xf0) | ((uptime >> 21) & 0x0f));
    h[5] = (short int) ((h[5] & 0x0f) | (frm_type << 4));
//      Serial.println("h[5]");	  
    if (mode == BPSK)
      h[6] = 99;	  
    posXi = (int)(current[mapping[PLUS_X]] + 0.5) + 2048;
    posYi = (int)(current[mapping[PLUS_Y]] + 0.5) + 2048;
    posZi = (int)(current[mapping[PLUS_Z]] + 0.5) + 2048;
    negXi = (int)(current[mapping[MINUS_X]] + 0.5) + 2048;
    negYi = (int)(current[mapping[MINUS_Y]] + 0.5) + 2048;
    negZi = (int)(current[mapping[MINUS_Z]] + 0.5) + 2048;
    posXv = (int)(voltage[mapping[PLUS_X]] * 100);
    posYv = (int)(voltage[mapping[PLUS_Y]] * 100);
    posZv = (int)(voltage[mapping[PLUS_Z]] * 100);
    negXv = (int)(voltage[mapping[MINUS_X]] * 100);
    negYv = (int)(voltage[mapping[MINUS_Y]] * 100);
    negZv = (int)(voltage[mapping[MINUS_Z]] * 100);
    batt_c_v = (int)(voltage[mapping[BAT]] * 100);
    battCurr = (int)(current[mapping[BAT]] + 0.5) + 2048;
    PSUVoltage = (int)(voltage[mapping[BUS]] * 100);
    PSUCurrent = (int)(current[mapping[BUS]] + 0.5) + 2048;
/*	  
    Serial.print("voltage[PLUS_X] = ");
    Serial.println(voltage[PLUS_X]);
    Serial.print("voltage[mapping[PLUS_X]] = ");
    Serial.println(voltage[mapping[PLUS_X]]);
    Serial.print("posXv = ");
    Serial.println(posXv);		  
*/	  
    if (payload == ON)
      STEMBoardFailure = 0;
    else	  
      STEMBoardFailure = 1;
	  
    // read payload sensor if available
//    Serial.println("Before encoding");
    encodeA(b, 0 + head_offset, batt_a_v);
//    Serial.println("After encoding");	  
    encodeB(b, 1 + head_offset, batt_b_v);
    encodeA(b, 3 + head_offset, batt_c_v);
    encodeB(b, 4 + head_offset, (int)(sensor[ACCEL_X] * 100 + 0.5) + 2048); // Xaccel
    encodeA(b, 6 + head_offset, (int)(sensor[ACCEL_Y] * 100 + 0.5) + 2048); // Yaccel
    encodeB(b, 7 + head_offset, (int)(sensor[ACCEL_Z] * 100 + 0.5) + 2048); // Zaccel
    encodeA(b, 9 + head_offset, battCurr);
    encodeB(b, 10 + head_offset, (int)(sensor[TEMP] * 10 + 0.5)); // Temp	
//	      Serial.println("A");
    if (mode == FSK) {
      encodeA(b, 12 + head_offset, posXv);
      encodeB(b, 13 + head_offset, negXv);
      encodeA(b, 15 + head_offset, posYv);
      encodeB(b, 16 + head_offset, negYv);
      encodeA(b, 18 + head_offset, posZv);
      encodeB(b, 19 + head_offset, negZv);
      encodeA(b, 21 + head_offset, posXi);
      encodeB(b, 22 + head_offset, negXi);
      encodeA(b, 24 + head_offset, posYi);
      encodeB(b, 25 + head_offset, negYi);
      encodeA(b, 27 + head_offset, posZi);
      encodeB(b, 28 + head_offset, negZi);
//	    	      Serial.println("B");
    } else // BPSK
    {
      encodeA(b, 12 + head_offset, posXv);
      encodeB(b, 13 + head_offset, posYv);
      encodeA(b, 15 + head_offset, posZv);
      encodeB(b, 16 + head_offset, negXv);
      encodeA(b, 18 + head_offset, negYv);
      encodeB(b, 19 + head_offset, negZv);
      encodeA(b, 21 + head_offset, posXi);
      encodeB(b, 22 + head_offset, posYi);
      encodeA(b, 24 + head_offset, posZi);
      encodeB(b, 25 + head_offset, negXi);
      encodeA(b, 27 + head_offset, negYi);
      encodeB(b, 28 + head_offset, negZi);
	    
      encodeA(b_max, 12 + head_offset, (int)(voltage_max[mapping[PLUS_X]] * 100));
      encodeB(b_max, 13 + head_offset, (int)(voltage_max[mapping[PLUS_Y]] * 100));
      encodeA(b_max, 15 + head_offset, (int)(voltage_max[mapping[PLUS_Z]] * 100));
      encodeB(b_max, 16 + head_offset, (int)(voltage_max[mapping[MINUS_X]] * 100));
      encodeA(b_max, 18 + head_offset, (int)(voltage_max[mapping[MINUS_Y]] * 100));
      encodeB(b_max, 19 + head_offset, (int)(voltage_max[mapping[MINUS_Z]] * 100));
      encodeA(b_max, 21 + head_offset, (int)(current_max[mapping[PLUS_X]] + 0.5) + 2048);
      encodeB(b_max, 22 + head_offset, (int)(current_max[mapping[PLUS_Y]] + 0.5) + 2048);
      encodeA(b_max, 24 + head_offset, (int)(current_max[mapping[PLUS_Z]] + 0.5) + 2048);
      encodeB(b_max, 25 + head_offset, (int)(current_max[mapping[MINUS_X]] + 0.5) + 2048);
      encodeA(b_max, 27 + head_offset, (int)(current_max[mapping[MINUS_Y]] + 0.5) + 2048);
      encodeB(b_max, 28 + head_offset, (int)(current_max[mapping[MINUS_Z]] + 0.5) + 2048);	    
      encodeA(b_max, 9 + head_offset, (int)(current_max[mapping[BAT]] + 0.5) + 2048);
      encodeA(b_max, 3 + head_offset, (int)(voltage_max[mapping[BAT]] * 100));
      encodeA(b_max, 30 + head_offset, (int)(voltage_max[mapping[BUS]] * 100));
      encodeB(b_max, 46 + head_offset, (int)(current_max[mapping[BUS]] + 0.5) + 2048);
	    
      encodeB(b_max, 37 + head_offset, (int)(other_max[RSSI] + 0.5) + 2048);	    
      encodeA(b_max, 39 + head_offset, (int)(other_max[IHU_TEMP] * 10 + 0.5));
      encodeB(b_max, 31 + head_offset, ((int)(other_max[SPIN] * 10)) + 2048);
	    
      if (sensor_min[0] != 1000.0) // make sure values are valid
      {	        	    
	      encodeB(b_max, 4 + head_offset, (int)(sensor_max[ACCEL_X] * 100 + 0.5) + 2048); // Xaccel
	      encodeA(b_max, 6 + head_offset, (int)(sensor_max[ACCEL_Y] * 100 + 0.5) + 2048); // Yaccel
	      encodeB(b_max, 7 + head_offset, (int)(sensor_max[ACCEL_Z] * 100 + 0.5) + 2048); // Zaccel	    
	      encodeA(b_max, 33 + head_offset, (int)(sensor_max[PRES] + 0.5)); // Pressure
	      encodeB(b_max, 34 + head_offset, (int)(sensor_max[ALT] * 10.0 + 0.5)); // Altitude
	      encodeB(b_max, 40 + head_offset, (int)(sensor_max[GYRO_X] + 0.5) + 2048);
	      encodeA(b_max, 42 + head_offset, (int)(sensor_max[GYRO_Y] + 0.5) + 2048);
	      encodeB(b_max, 43 + head_offset, (int)(sensor_max[GYRO_Z] + 0.5) + 2048);
	      encodeA(b_max, 48 + head_offset, (int)(sensor_max[XS1] * 10 + 0.5) + 2048);
	      encodeB(b_max, 49 + head_offset, (int)(sensor_max[XS2] * 10 + 0.5) + 2048);
	      encodeB(b_max, 10 + head_offset, (int)(sensor_max[TEMP] * 10 + 0.5)); 	
	      encodeA(b_max, 45 + head_offset, (int)(sensor_max[HUMI] * 10 + 0.5));
      }	  
      else
      {	        	    
	      encodeB(b_max, 4 + head_offset, 2048); // 0
	      encodeA(b_max, 6 + head_offset, 2048); // 0
	      encodeB(b_max, 7 + head_offset, 2048); // 0	    
	      encodeB(b_max, 40 + head_offset, 2048);
	      encodeA(b_max, 42 + head_offset, 2048);
	      encodeB(b_max, 43 + head_offset, 2048);
	      encodeA(b_max, 48 + head_offset, 2048);
	      encodeB(b_max, 49 + head_offset, 2048);
      }	  	      
      encodeA(b_min, 12 + head_offset, (int)(voltage_min[mapping[PLUS_X]] * 100));
      encodeB(b_min, 13 + head_offset, (int)(voltage_min[mapping[PLUS_Y]] * 100));
      encodeA(b_min, 15 + head_offset, (int)(voltage_min[mapping[PLUS_Z]] * 100));
      encodeB(b_min, 16 + head_offset, (int)(voltage_min[mapping[MINUS_X]] * 100));
      encodeA(b_min, 18 + head_offset, (int)(voltage_min[mapping[MINUS_Y]] * 100));
      encodeB(b_min, 19 + head_offset, (int)(voltage_min[mapping[MINUS_Z]] * 100));
      encodeA(b_min, 21 + head_offset, (int)(current_min[mapping[PLUS_X]] + 0.5) + 2048);
      encodeB(b_min, 22 + head_offset, (int)(current_min[mapping[PLUS_Y]] + 0.5) + 2048);
      encodeA(b_min, 24 + head_offset, (int)(current_min[mapping[PLUS_Z]] + 0.5) + 2048);
      encodeB(b_min, 25 + head_offset, (int)(current_min[mapping[MINUS_X]] + 0.5) + 2048);
      encodeA(b_min, 27 + head_offset, (int)(current_min[mapping[MINUS_Y]] + 0.5) + 2048);
      encodeB(b_min, 28 + head_offset, (int)(current_min[mapping[MINUS_Z]] + 0.5) + 2048);	
	    
      encodeA(b_min, 9 + head_offset, (int)(current_min[mapping[BAT]] + 0.5) + 2048);
      encodeA(b_min, 3 + head_offset, (int)(voltage_min[mapping[BAT]] * 100));
      encodeA(b_min, 30 + head_offset, (int)(voltage_min[mapping[BUS]] * 100));
      encodeB(b_min, 46 + head_offset, (int)(current_min[mapping[BUS]] + 0.5) + 2048);
	    
      encodeB(b_min, 31 + head_offset, ((int)(other_min[SPIN] * 10)) + 2048);
      encodeB(b_min, 37 + head_offset, (int)(other_min[RSSI] + 0.5) + 2048);	    
      encodeA(b_min, 39 + head_offset, (int)(other_min[IHU_TEMP] * 10 + 0.5));
	    
      if (sensor_min[0] != 1000.0) // make sure values are valid
      {	        
	      encodeB(b_min, 4 + head_offset, (int)(sensor_min[ACCEL_X] * 100 + 0.5) + 2048); // Xaccel
	      encodeA(b_min, 6 + head_offset, (int)(sensor_min[ACCEL_Y] * 100 + 0.5) + 2048); // Yaccel
	      encodeB(b_min, 7 + head_offset, (int)(sensor_min[ACCEL_Z] * 100 + 0.5) + 2048); // Zaccel	
	      encodeA(b_min, 33 + head_offset, (int)(sensor_min[PRES] + 0.5)); // Pressure
	      encodeB(b_min, 34 + head_offset, (int)(sensor_min[ALT] * 10.0 + 0.5)); // Altitude
	      encodeB(b_min, 40 + head_offset, (int)(sensor_min[GYRO_X] + 0.5) + 2048);
	      encodeA(b_min, 42 + head_offset, (int)(sensor_min[GYRO_Y] + 0.5) + 2048);
	      encodeB(b_min, 43 + head_offset, (int)(sensor_min[GYRO_Z] + 0.5) + 2048);
	      encodeA(b_min, 48 + head_offset, (int)(sensor_min[XS1] * 10 + 0.5) + 2048);
	      encodeB(b_min, 49 + head_offset, (int)(sensor_min[XS2] * 10 + 0.5) + 2048);
	      encodeB(b_min, 10 + head_offset, (int)(sensor_min[TEMP] * 10 + 0.5)); 	    
	      encodeA(b_min, 45 + head_offset, (int)(sensor_min[HUMI] * 10 + 0.5));
    }      
      else
      {	        	    
	      encodeB(b_min, 4 + head_offset, 2048); // 0
	      encodeA(b_min, 6 + head_offset, 2048); // 0
	      encodeB(b_min, 7 + head_offset, 2048); // 0	    
	      encodeB(b_min, 40 + head_offset, 2048);
	      encodeA(b_min, 42 + head_offset, 2048);
	      encodeB(b_min, 43 + head_offset, 2048);
	      encodeA(b_min, 48 + head_offset, 2048);
	      encodeB(b_min, 49 + head_offset, 2048);
      }	 
    }
//	  	      Serial.println("C");
    encodeA(b, 30 + head_offset, PSUVoltage);
    encodeB(b, 31 + head_offset, ((int)(other[SPIN] * 10)) + 2048);
    encodeA(b, 33 + head_offset, (int)(sensor[PRES] + 0.5)); // Pressure
    encodeB(b, 34 + head_offset, (int)(sensor[ALT] * 10.0 + 0.5)); // Altitude
    encodeA(b, 36 + head_offset, Resets);
    encodeB(b, 37 + head_offset, (int)(other[RSSI] + 0.5) + 2048);
    encodeA(b, 39 + head_offset, (int)(other[IHU_TEMP] * 10 + 0.5));
    encodeB(b, 40 + head_offset, (int)(sensor[GYRO_X] + 0.5) + 2048);
    encodeA(b, 42 + head_offset, (int)(sensor[GYRO_Y] + 0.5) + 2048);
    encodeB(b, 43 + head_offset, (int)(sensor[GYRO_Z] + 0.5) + 2048);
    encodeA(b, 45 + head_offset, (int)(sensor[HUMI] * 10 + 0.5)); // in place of sensor1
    encodeB(b, 46 + head_offset, PSUCurrent);
    encodeA(b, 48 + head_offset, (int)(sensor[XS1] * 10 + 0.5) + 2048);
    encodeB(b, 49 + head_offset, (int)(sensor[XS2] * 10 + 0.5) + 2048);
//	  	      Serial.println("D");	  
    int status = STEMBoardFailure + SafeMode * 2 + sim_mode * 4 + PayloadFailure1 * 8 +
      (i2c_bus0 == false) * 16 + (i2c_bus1 == false) * 32 + (i2c_bus3 == false) * 64 + (camera_detected == OFF) * 128 + groundCommandCount * 256;
    encodeA(b, 51 + head_offset, status);
    encodeB(b, 52 + head_offset, rxAntennaDeployed + txAntennaDeployed * 2);
//	  	      Serial.println("E");	  
    if (txAntennaDeployed == 0) {
      txAntennaDeployed = 1;
//      Serial.println("TX Antenna Deployed!");
    }
//	  	      Serial.println("F");    
    if (mode == BPSK) {  // wod field experiments
      unsigned long val = 0xffff;
      encodeA(b, 64 + head_offset, 0xff & val); 
      encodeA(b, 65 + head_offset, val >> 8); 	    
      encodeA(b, 63 + head_offset, 0x00); 
      encodeA(b, 62 + head_offset, 0x01);
      encodeB(b, 74 + head_offset, 0xfff); 
    }
//    Serial.println("Finished encoding");	  
    short int data10[headerLen + rsFrames * (rsFrameLen + parityLen)];
    short int data8[headerLen + rsFrames * (rsFrameLen + parityLen)];
    int ctr1 = 0;
    int ctr3 = 0;
    for (i = 0; i < rsFrameLen; i++) {
      for (int j = 0; j < rsFrames; j++) {
        if (!((i == (rsFrameLen - 1)) && (j == 2))) // skip last one for BPSK
        {
          if (ctr1 < headerLen) {
            rs_frame[j][i] = h[ctr1];
            update_rs(parities[j], h[ctr1]);
            //      				printf("header %d rs_frame[%d][%d] = %x \n", ctr1, j, i, h[ctr1]);
            data8[ctr1++] = rs_frame[j][i];
            //				printf ("data8[%d] = %x \n", ctr1 - 1, rs_frame[j][i]);
          } else {
	     if (mode == FSK)
	     {
            	rs_frame[j][i] = b[ctr3 % dataLen];
            	update_rs(parities[j], b[ctr3 % dataLen]);
	     }  else // BPSK
		if ((int)(ctr3/dataLen) == 3)  
		{
            		rs_frame[j][i] = b_max[ctr3 % dataLen];
            		update_rs(parities[j], b_max[ctr3 % dataLen]);
		}
		else if ((int)(ctr3/dataLen) == 4)  
		{
            		rs_frame[j][i] = b_min[ctr3 % dataLen];
            		update_rs(parities[j], b_min[ctr3 % dataLen]);
		}		
		else
		{
            		rs_frame[j][i] = b[ctr3 % dataLen];
            		update_rs(parities[j], b[ctr3 % dataLen]);
		}
	     {
	    }
		  
            //  				printf("%d rs_frame[%d][%d] = %x %d \n", 
            //  					ctr1, j, i, b[ctr3 % DATA_LEN], ctr3 % DATA_LEN);
            data8[ctr1++] = rs_frame[j][i];
            //			printf ("data8[%d] = %x \n", ctr1 - 1, rs_frame[j][i]);
            ctr3++;
          }
        }
      }
    }
    ///#ifdef DEBUG_LOGGING
    //	printf("\nAt end of data8 write, %d ctr1 values written\n\n", ctr1);
    /*
    	  printf("Parities ");
    		for (int m = 0; m < parityLen; m++) {
    		 	printf("%d ", parities[0][m]);
    		}
    		printf("\n");
    */
   /// #endif
	   
    int ctr2 = 0;
    memset(data10, 0, sizeof(data10));
    for (i = 0; i < dataLen * payloads + headerLen; i++) // 476 for BPSK
    {
      data10[ctr2] = (Encode_8b10b[rd][((int) data8[ctr2])] & 0x3ff);
      nrd = (Encode_8b10b[rd][((int) data8[ctr2])] >> 10) & 1;
      //		printf ("data10[%d] = encoded data8[%d] = %x \n",
      //		 	ctr2, ctr2, data10[ctr2]); 
/*      if (firstTime) {
        Serial.print(data10[ctr2], HEX);
	Serial.print(" ");
      }
*/
      rd = nrd; // ^ nrd;
      ctr2++;
    }
//    {
      for (i = 0; i < parityLen; i++) {
        for (int j = 0; j < rsFrames; j++) {
          if ((uptime != 0) || (i != 0))	// don't correctly update parties if uptime is 0 so the frame will fail the FEC check and be discarded  
            data10[ctr2++] = (Encode_8b10b[rd][((int) parities[j][i])] & 0x3ff);
	  nrd = (Encode_8b10b[rd][((int) parities[j][i])] >> 10) & 1;
        //	printf ("data10[%d] = encoded parities[%d][%d] = %x \n",
        //		 ctr2 - 1, j, i, data10[ctr2 - 1]); 
/*          if (firstTime) {
	    Serial.print(data10[ctr2-1], HEX);  // added -1 due to ++
	    Serial.print(" ");
          }
*/
          rd = nrd;
        }
      }
 //   }
 ///   #ifdef DEBUG_LOGGING
    // 	printf("\nAt end of data10 write, %d ctr2 values written\n\n", ctr2);
 ///   #endif

    int data;
    int val;
    //int offset = 0;

///    #ifdef DEBUG_LOGGING
    //	printf("\nAt start of buffer loop, syncBits %d samples %d ctr %d\n", syncBits, samples, ctr);
///    #endif

    for (i = 1; i <= syncBits * samples; i++) {
      write_wave(ctr, buffer);
      //		printf("%d ",ctr);
      if ((i % samples) == 0) {
        int bit = syncBits - i / samples + 1;
        val = sync;
        data = val & 1 << (bit - 1);
        //   	printf ("%d i: %d new frame %d sync bit %d = %d \n",
        //  		 ctr/SAMPLES, i, frames, bit, (data > 0) );
        if (mode == FSK) {
          phase = ((data != 0) * 2) - 1;
        } else {
          if (data == 0) {
            phase *= -1;
        }
 //	printf("Sending a %d\n", phase);
		
       }
      }
    }
///    #ifdef DEBUG_LOGGING
    //	printf("\n\nValue of ctr after header: %d Buffer Len: %d\n\n", ctr, buffSize);
///    #endif
//    Serial.println(10 * (headerLen + dataLen * payloads + rsFrames * parityLen) * samples);	  
    for (i = 1; i <= (10 * (headerLen + dataLen * payloads + rsFrames * parityLen) * samples); i++) // 572   
//    for (i = 1; i <= ((headerLen + dataLen * payloads + rsFrames * parityLen) * samples); i++) // Not 10 * anymore 572   
    {
      write_wave(ctr, buffer);
      if ((i % samples) == 0) {
        int symbol = (int)((i - 1) / (samples * 10));
        int bit = 10 - (i - symbol * samples * 10) / samples + 1;
        val = data10[symbol];
        data = val & 1 << (bit - 1);
        //		printf ("%d i: %d new frame %d data10[%d] = %x bit %d = %d \n",
        //	    		 ctr/SAMPLES, i, frames, symbol, val, bit, (data > 0) );
//	Serial.print(data, BIN);   // Debugging print!!!  
//	Serial.print(" ");      
        if (mode == FSK) {
          phase = ((data != 0) * 2) - 1;
//          Serial.printf("Sending a %d\n", phase);
        } else {
          if (data == 0) {
            phase *= -1;
//            if ((ctr - smaller) > 0) {
//              for (int j = 1; j <= smaller; j++)
//                buffer[ctr - j] = buffer[ctr - j] * 0.4;
          }
//            flip_ctr = ctr;
          }
///        }
//	Serial.println("AA");      
      }
//	Serial.println("BB");     
    }
//	Serial.println("CC");     
  }
  if (debug_mode) {		
    Serial.println(" ");	
    Serial.print("get_fox_tlm returning with counter: ");
    Serial.println(ctr);
  }
  if (firstTime) {
    Serial.println(" ");
    firstTime = FALSE;	
  }
}

//void write_wave(int i, short int *buffer)
void write_wave(int i, byte *buffer)
{
    buffer[ctr++] = (byte)(phase == 1); 
//    Serial.printf("buffer is %d \n", buffer[ctr - 1]);
//	Serial.print(ctr);
//	Serial.print(" ");
	
    if (ctr > bufLen) {
	ctr = ctr - bufLen;
//	if (debug_mode) {
//	  Serial.print("ctr reset ");
    if (bufLen != 0) {
      float avg_time = (float)(micros() - micros3)/(float)bufLen;
//      if ((avg_time > (delay_time * 1.15)) || debug_mode) {	    
        Serial.print("r Microseconds: ");
        Serial.println(avg_time);
//      }
    }
//  }	  
    micros3 = micros();	    
	    
//	  Serial.print(" ");
//	  Serial.println(millis());
//	}
    }
//	Serial.printf(" b: %d ", buffer[ctr - 1]);

}

int encodeA(short int  *b, int index, int val) {
//    printf("Encoding A\n");
    b[index] = val & 0xff;
    b[index + 1] = (short int) ((b[index + 1] & 0xf0) | ((val >> 8) & 0x0f));
    return 0;	
}

int encodeB(short int  *b, int index, int val) {
//    printf("Encoding B\n");
    b[index] =  (short int) ((b[index] & 0x0f)  |  ((val << 4) & 0xf0));
    b[index + 1] = (val >> 4 ) & 0xff;
    return 0;	
}

int twosToInt(int val,int len) {   // Convert twos compliment to integer
// from https://www.raspberrypi.org/forums/viewtopic.php?t=55815
	
      if(val & (1 << (len - 1)))
         val = val - (1 << len);

      return(val);
}

float rnd_float(double min,double max) {   // returns 2 decimal point random number
	
	int val = (rand() % ((int)(max*100) - (int)(min*100) + 1)) + (int)(min*100);
	float ret = ((float)(val)/100);
	
      return(ret);
}

float toAprsFormat(float input) {
// converts decimal coordinate (latitude or longitude) to APRS DDMM.MM format	
    int dd = (int) input;
    int mm1 = (int)((input - dd) * 60.0);
    int mm2 = (int)((input - dd - (float)mm1/60.0) * 60.0 * 60.0);
    float output = dd * 100 + mm1 + (float)mm2 * 0.01;
    return(output);	
}



/*
 * TelemEncoding.c
 *
   Fox-1 telemetry encoder
   January 2014 Phil Karn KA9Q
   This file has two external functions:
      void update_rs(unsigned char parity[32],unsigned char data);
      int encode_8b10b(int *state,int data).
   update_rs() is the Reed-Solomon encoder. Its first argument is the 32-byte
   encoder shift register, the second is the 8-bit data byte being encoded. It updates
   the shift register in place and returns void. At the end of each frame, it contains
   the parities ready for transmission, starting with parity[0].
   Be sure to zero this array before each new frame!
   encode_8b10b() is the 8b10b encoder. Its first argument is a pointer to a single integer
   with the 1-bit encoder state (the current run disparity, or RD). Initialize it to 0
   JUST ONCE at startup (not between frames).
   The second argument is the data byte being encoded. It updates the state and returns
   an integer containing the 10-bit encoded word, right justified.
   Transmit this word from left to right.
   The data argument is an int so it can hold the special value -1 to indicate end of frame;
   it generates the 8b10b control word K.28.5, which is used as an inter-frame flag.
   Some assert() calls are made to verify legality of arguments. These can be turned off in
   production code.
   sample frame transmission code:
   unsigned char data[64]; // Data block to be sent
   unsigned char parity[32]; // RS parities
   void transmit_word(int);  // User provided transmit function: 10 bits of data in bits 9....0
   int state,i;
   state = 0; // Only once at startup, not between frames
   memset(parity,0,sizeof(parity); // Do this before every frame
   // Transmit the data, updating the RS encoder
   for(i=0;i<64;i++){
     update_rs(parity,data[i]);
     transmit_word(encode_8b10b(&state,data[i]);
   }
   // get the RS parities
   for(i=0;i<32;i++)
     transmit_word(encode_8b10b(&state,parity[i]);
   transmit_word(encode_8b10b(&state,-1); // Transmit end-of-frame flag
*/


#include <string.h>
//#include "Fox.h"
//#include "TelemEncoding.h"
#include <assert.h>
#include <stdio.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#define NN (0xff) // Frame size in symbols
#define A0 (NN)   // special value for log(0)

int Encode_8b10b[][256] = {
		   // RD = -1 cases
		{
		   /* 00 */ 0x274,
		   /* 01 */ 0x1d4,
		   /* 02 */ 0x2d4,
		   /* 03 */ 0x71b,
		   /* 04 */ 0x354,
		   /* 05 */ 0x69b,
		   /* 06 */ 0x59b,
		   /* 07 */ 0x78b,
		   /* 08 */ 0x394,
		   /* 09 */ 0x65b,
		   /* 0a */ 0x55b,
		   /* 0b */ 0x74b,
		   /* 0c */ 0x4db,
		   /* 0d */ 0x6cb,
		   /* 0e */ 0x5cb,
		   /* 0f */ 0x174,
		   /* 10 */ 0x1b4,
		   /* 11 */ 0x63b,
		   /* 12 */ 0x53b,
		   /* 13 */ 0x72b,
		   /* 14 */ 0x4bb,
		   /* 15 */ 0x6ab,
		   /* 16 */ 0x5ab,
		   /* 17 */ 0x3a4,
		   /* 18 */ 0x334,
		   /* 19 */ 0x66b,
		   /* 1a */ 0x56b,
		   /* 1b */ 0x364,
		   /* 1c */ 0x4eb,
		   /* 1d */ 0x2e4,
		   /* 1e */ 0x1e4,
		   /* 1f */ 0x2b4,
		   /* 20 */ 0x679,
		   /* 21 */ 0x5d9,
		   /* 22 */ 0x6d9,
		   /* 23 */ 0x319,
		   /* 24 */ 0x759,
		   /* 25 */ 0x299,
		   /* 26 */ 0x199,
		   /* 27 */ 0x389,
		   /* 28 */ 0x799,
		   /* 29 */ 0x259,
		   /* 2a */ 0x159,
		   /* 2b */ 0x349,
		   /* 2c */ 0x0d9,
		   /* 2d */ 0x2c9,
		   /* 2e */ 0x1c9,
		   /* 2f */ 0x579,
		   /* 30 */ 0x5b9,
		   /* 31 */ 0x239,
		   /* 32 */ 0x139,
		   /* 33 */ 0x329,
		   /* 34 */ 0x0b9,
		   /* 35 */ 0x2a9,
		   /* 36 */ 0x1a9,
		   /* 37 */ 0x7a9,
		   /* 38 */ 0x739,
		   /* 39 */ 0x269,
		   /* 3a */ 0x169,
		   /* 3b */ 0x769,
		   /* 3c */ 0x0e9,
		   /* 3d */ 0x6e9,
		   /* 3e */ 0x5e9,
		   /* 3f */ 0x6b9,
		   /* 40 */ 0x675,
		   /* 41 */ 0x5d5,
		   /* 42 */ 0x6d5,
		   /* 43 */ 0x315,
		   /* 44 */ 0x755,
		   /* 45 */ 0x295,
		   /* 46 */ 0x195,
		   /* 47 */ 0x385,
		   /* 48 */ 0x795,
		   /* 49 */ 0x255,
		   /* 4a */ 0x155,
		   /* 4b */ 0x345,
		   /* 4c */ 0x0d5,
		   /* 4d */ 0x2c5,
		   /* 4e */ 0x1c5,
		   /* 4f */ 0x575,
		   /* 50 */ 0x5b5,
		   /* 51 */ 0x235,
		   /* 52 */ 0x135,
		   /* 53 */ 0x325,
		   /* 54 */ 0x0b5,
		   /* 55 */ 0x2a5,
		   /* 56 */ 0x1a5,
		   /* 57 */ 0x7a5,
		   /* 58 */ 0x735,
		   /* 59 */ 0x265,
		   /* 5a */ 0x165,
		   /* 5b */ 0x765,
		   /* 5c */ 0x0e5,
		   /* 5d */ 0x6e5,
		   /* 5e */ 0x5e5,
		   /* 5f */ 0x6b5,
		   /* 60 */ 0x673,
		   /* 61 */ 0x5d3,
		   /* 62 */ 0x6d3,
		   /* 63 */ 0x31c,
		   /* 64 */ 0x753,
		   /* 65 */ 0x29c,
		   /* 66 */ 0x19c,
		   /* 67 */ 0x38c,
		   /* 68 */ 0x793,
		   /* 69 */ 0x25c,
		   /* 6a */ 0x15c,
		   /* 6b */ 0x34c,
		   /* 6c */ 0x0dc,
		   /* 6d */ 0x2cc,
		   /* 6e */ 0x1cc,
		   /* 6f */ 0x573,
		   /* 70 */ 0x5b3,
		   /* 71 */ 0x23c,
		   /* 72 */ 0x13c,
		   /* 73 */ 0x32c,
		   /* 74 */ 0x0bc,
		   /* 75 */ 0x2ac,
		   /* 76 */ 0x1ac,
		   /* 77 */ 0x7a3,
		   /* 78 */ 0x733,
		   /* 79 */ 0x26c,
		   /* 7a */ 0x16c,
		   /* 7b */ 0x763,
		   /* 7c */ 0x0ec,
		   /* 7d */ 0x6e3,
		   /* 7e */ 0x5e3,
		   /* 7f */ 0x6b3,
		   /* 80 */ 0x272,
		   /* 81 */ 0x1d2,
		   /* 82 */ 0x2d2,
		   /* 83 */ 0x71d,
		   /* 84 */ 0x352,
		   /* 85 */ 0x69d,
		   /* 86 */ 0x59d,
		   /* 87 */ 0x78d,
		   /* 88 */ 0x392,
		   /* 89 */ 0x65d,
		   /* 8a */ 0x55d,
		   /* 8b */ 0x74d,
		   /* 8c */ 0x4dd,
		   /* 8d */ 0x6cd,
		   /* 8e */ 0x5cd,
		   /* 8f */ 0x172,
		   /* 90 */ 0x1b2,
		   /* 91 */ 0x63d,
		   /* 92 */ 0x53d,
		   /* 93 */ 0x72d,
		   /* 94 */ 0x4bd,
		   /* 95 */ 0x6ad,
		   /* 96 */ 0x5ad,
		   /* 97 */ 0x3a2,
		   /* 98 */ 0x332,
		   /* 99 */ 0x66d,
		   /* 9a */ 0x56d,
		   /* 9b */ 0x362,
		   /* 9c */ 0x4ed,
		   /* 9d */ 0x2e2,
		   /* 9e */ 0x1e2,
		   /* 9f */ 0x2b2,
		   /* a0 */ 0x67a,
		   /* a1 */ 0x5da,
		   /* a2 */ 0x6da,
		   /* a3 */ 0x31a,
		   /* a4 */ 0x75a,
		   /* a5 */ 0x29a,
		   /* a6 */ 0x19a,
		   /* a7 */ 0x38a,
		   /* a8 */ 0x79a,
		   /* a9 */ 0x25a,
		   /* aa */ 0x15a,
		   /* ab */ 0x34a,
		   /* ac */ 0x0da,
		   /* ad */ 0x2ca,
		   /* ae */ 0x1ca,
		   /* af */ 0x57a,
		   /* b0 */ 0x5ba,
		   /* b1 */ 0x23a,
		   /* b2 */ 0x13a,
		   /* b3 */ 0x32a,
		   /* b4 */ 0x0ba,
		   /* b5 */ 0x2aa,
		   /* b6 */ 0x1aa,
		   /* b7 */ 0x7aa,
		   /* b8 */ 0x73a,
		   /* b9 */ 0x26a,
		   /* ba */ 0x16a,
		   /* bb */ 0x76a,
		   /* bc */ 0x0ea,
		   /* bd */ 0x6ea,
		   /* be */ 0x5ea,
		   /* bf */ 0x6ba,
		   /* c0 */ 0x676,
		   /* c1 */ 0x5d6,
		   /* c2 */ 0x6d6,
		   /* c3 */ 0x316,
		   /* c4 */ 0x756,
		   /* c5 */ 0x296,
		   /* c6 */ 0x196,
		   /* c7 */ 0x386,
		   /* c8 */ 0x796,
		   /* c9 */ 0x256,
		   /* ca */ 0x156,
		   /* cb */ 0x346,
		   /* cc */ 0x0d6,
		   /* cd */ 0x2c6,
		   /* ce */ 0x1c6,
		   /* cf */ 0x576,
		   /* d0 */ 0x5b6,
		   /* d1 */ 0x236,
		   /* d2 */ 0x136,
		   /* d3 */ 0x326,
		   /* d4 */ 0x0b6,
		   /* d5 */ 0x2a6,
		   /* d6 */ 0x1a6,
		   /* d7 */ 0x7a6,
		   /* d8 */ 0x736,
		   /* d9 */ 0x266,
		   /* da */ 0x166,
		   /* db */ 0x766,
		   /* dc */ 0x0e6,
		   /* dd */ 0x6e6,
		   /* de */ 0x5e6,
		   /* df */ 0x6b6,
		   /* e0 */ 0x271,
		   /* e1 */ 0x1d1,
		   /* e2 */ 0x2d1,
		   /* e3 */ 0x71e,
		   /* e4 */ 0x351,
		   /* e5 */ 0x69e,
		   /* e6 */ 0x59e,
		   /* e7 */ 0x78e,
		   /* e8 */ 0x391,
		   /* e9 */ 0x65e,
		   /* ea */ 0x55e,
		   /* eb */ 0x74e,
		   /* ec */ 0x4de,
		   /* ed */ 0x6ce,
		   /* ee */ 0x5ce,
		   /* ef */ 0x171,
		   /* f0 */ 0x1b1,
		   /* f1 */ 0x637,
		   /* f2 */ 0x537,
		   /* f3 */ 0x72e,
		   /* f4 */ 0x4b7,
		   /* f5 */ 0x6ae,
		   /* f6 */ 0x5ae,
		   /* f7 */ 0x3a1,
		   /* f8 */ 0x331,
		   /* f9 */ 0x66e,
		   /* fa */ 0x56e,
		   /* fb */ 0x361,
		   /* fc */ 0x4ee,
		   /* fd */ 0x2e1,
		   /* fe */ 0x1e1,
		   /* ff */ 0x2b1,
		},   // RD = +1 cases 
		{
		   /* 00 */ 0x58b,
		   /* 01 */ 0x62b,
		   /* 02 */ 0x52b,
		   /* 03 */ 0x314,
		   /* 04 */ 0x4ab,
		   /* 05 */ 0x294,
		   /* 06 */ 0x194,
		   /* 07 */ 0x074,
		   /* 08 */ 0x46b,
		   /* 09 */ 0x254,
		   /* 0a */ 0x154,
		   /* 0b */ 0x344,
		   /* 0c */ 0x0d4,
		   /* 0d */ 0x2c4,
		   /* 0e */ 0x1c4,
		   /* 0f */ 0x68b,
		   /* 10 */ 0x64b,
		   /* 11 */ 0x234,
		   /* 12 */ 0x134,
		   /* 13 */ 0x324,
		   /* 14 */ 0x0b4,
		   /* 15 */ 0x2a4,
		   /* 16 */ 0x1a4,
		   /* 17 */ 0x45b,
		   /* 18 */ 0x4cb,
		   /* 19 */ 0x264,
		   /* 1a */ 0x164,
		   /* 1b */ 0x49b,
		   /* 1c */ 0x0e4,
		   /* 1d */ 0x51b,
		   /* 1e */ 0x61b,
		   /* 1f */ 0x54b,
		   /* 20 */ 0x189,
		   /* 21 */ 0x229,
		   /* 22 */ 0x129,
		   /* 23 */ 0x719,
		   /* 24 */ 0x0a9,
		   /* 25 */ 0x699,
		   /* 26 */ 0x599,
		   /* 27 */ 0x479,
		   /* 28 */ 0x069,
		   /* 29 */ 0x659,
		   /* 2a */ 0x559,
		   /* 2b */ 0x749,
		   /* 2c */ 0x4d9,
		   /* 2d */ 0x6c9,
		   /* 2e */ 0x5c9,
		   /* 2f */ 0x289,
		   /* 30 */ 0x249,
		   /* 31 */ 0x639,
		   /* 32 */ 0x539,
		   /* 33 */ 0x729,
		   /* 34 */ 0x4b9,
		   /* 35 */ 0x6a9,
		   /* 36 */ 0x5a9,
		   /* 37 */ 0x059,
		   /* 38 */ 0x0c9,
		   /* 39 */ 0x669,
		   /* 3a */ 0x569,
		   /* 3b */ 0x099,
		   /* 3c */ 0x4e9,
		   /* 3d */ 0x119,
		   /* 3e */ 0x219,
		   /* 3f */ 0x149,
		   /* 40 */ 0x185,
		   /* 41 */ 0x225,
		   /* 42 */ 0x125,
		   /* 43 */ 0x715,
		   /* 44 */ 0x0a5,
		   /* 45 */ 0x695,
		   /* 46 */ 0x595,
		   /* 47 */ 0x475,
		   /* 48 */ 0x065,
		   /* 49 */ 0x655,
		   /* 4a */ 0x555,
		   /* 4b */ 0x745,
		   /* 4c */ 0x4d5,
		   /* 4d */ 0x6c5,
		   /* 4e */ 0x5c5,
		   /* 4f */ 0x285,
		   /* 50 */ 0x245,
		   /* 51 */ 0x635,
		   /* 52 */ 0x535,
		   /* 53 */ 0x725,
		   /* 54 */ 0x4b5,
		   /* 55 */ 0x6a5,
		   /* 56 */ 0x5a5,
		   /* 57 */ 0x055,
		   /* 58 */ 0x0c5,
		   /* 59 */ 0x665,
		   /* 5a */ 0x565,
		   /* 5b */ 0x095,
		   /* 5c */ 0x4e5,
		   /* 5d */ 0x115,
		   /* 5e */ 0x215,
		   /* 5f */ 0x145,
		   /* 60 */ 0x18c,
		   /* 61 */ 0x22c,
		   /* 62 */ 0x12c,
		   /* 63 */ 0x713,
		   /* 64 */ 0x0ac,
		   /* 65 */ 0x693,
		   /* 66 */ 0x593,
		   /* 67 */ 0x473,
		   /* 68 */ 0x06c,
		   /* 69 */ 0x653,
		   /* 6a */ 0x553,
		   /* 6b */ 0x743,
		   /* 6c */ 0x4d3,
		   /* 6d */ 0x6c3,
		   /* 6e */ 0x5c3,
		   /* 6f */ 0x28c,
		   /* 70 */ 0x24c,
		   /* 71 */ 0x633,
		   /* 72 */ 0x533,
		   /* 73 */ 0x723,
		   /* 74 */ 0x4b3,
		   /* 75 */ 0x6a3,
		   /* 76 */ 0x5a3,
		   /* 77 */ 0x05c,
		   /* 78 */ 0x0cc,
		   /* 79 */ 0x663,
		   /* 7a */ 0x563,
		   /* 7b */ 0x09c,
		   /* 7c */ 0x4e3,
		   /* 7d */ 0x11c,
		   /* 7e */ 0x21c,
		   /* 7f */ 0x14c,
		   /* 80 */ 0x58d,
		   /* 81 */ 0x62d,
		   /* 82 */ 0x52d,
		   /* 83 */ 0x312,
		   /* 84 */ 0x4ad,
		   /* 85 */ 0x292,
		   /* 86 */ 0x192,
		   /* 87 */ 0x072,
		   /* 88 */ 0x46d,
		   /* 89 */ 0x252,
		   /* 8a */ 0x152,
		   /* 8b */ 0x342,
		   /* 8c */ 0x0d2,
		   /* 8d */ 0x2c2,
		   /* 8e */ 0x1c2,
		   /* 8f */ 0x68d,
		   /* 90 */ 0x64d,
		   /* 91 */ 0x232,
		   /* 92 */ 0x132,
		   /* 93 */ 0x322,
		   /* 94 */ 0x0b2,
		   /* 95 */ 0x2a2,
		   /* 96 */ 0x1a2,
		   /* 97 */ 0x45d,
		   /* 98 */ 0x4cd,
		   /* 99 */ 0x262,
		   /* 9a */ 0x162,
		   /* 9b */ 0x49d,
		   /* 9c */ 0x0e2,
		   /* 9d */ 0x51d,
		   /* 9e */ 0x61d,
		   /* 9f */ 0x54d,
		   /* a0 */ 0x18a,
		   /* a1 */ 0x22a,
		   /* a2 */ 0x12a,
		   /* a3 */ 0x71a,
		   /* a4 */ 0x0aa,
		   /* a5 */ 0x69a,
		   /* a6 */ 0x59a,
		   /* a7 */ 0x47a,
		   /* a8 */ 0x06a,
		   /* a9 */ 0x65a,
		   /* aa */ 0x55a,
		   /* ab */ 0x74a,
		   /* ac */ 0x4da,
		   /* ad */ 0x6ca,
		   /* ae */ 0x5ca,
		   /* af */ 0x28a,
		   /* b0 */ 0x24a,
		   /* b1 */ 0x63a,
		   /* b2 */ 0x53a,
		   /* b3 */ 0x72a,
		   /* b4 */ 0x4ba,
		   /* b5 */ 0x6aa,
		   /* b6 */ 0x5aa,
		   /* b7 */ 0x05a,
		   /* b8 */ 0x0ca,
		   /* b9 */ 0x66a,
		   /* ba */ 0x56a,
		   /* bb */ 0x09a,
		   /* bc */ 0x4ea,
		   /* bd */ 0x11a,
		   /* be */ 0x21a,
		   /* bf */ 0x14a,
		   /* c0 */ 0x186,
		   /* c1 */ 0x226,
		   /* c2 */ 0x126,
		   /* c3 */ 0x716,
		   /* c4 */ 0x0a6,
		   /* c5 */ 0x696,
		   /* c6 */ 0x596,
		   /* c7 */ 0x476,
		   /* c8 */ 0x066,
		   /* c9 */ 0x656,
		   /* ca */ 0x556,
		   /* cb */ 0x746,
		   /* cc */ 0x4d6,
		   /* cd */ 0x6c6,
		   /* ce */ 0x5c6,
		   /* cf */ 0x286,
		   /* d0 */ 0x246,
		   /* d1 */ 0x636,
		   /* d2 */ 0x536,
		   /* d3 */ 0x726,
		   /* d4 */ 0x4b6,
		   /* d5 */ 0x6a6,
		   /* d6 */ 0x5a6,
		   /* d7 */ 0x056,
		   /* d8 */ 0x0c6,
		   /* d9 */ 0x666,
		   /* da */ 0x566,
		   /* db */ 0x096,
		   /* dc */ 0x4e6,
		   /* dd */ 0x116,
		   /* de */ 0x216,
		   /* df */ 0x146,
		   /* e0 */ 0x58e,
		   /* e1 */ 0x62e,
		   /* e2 */ 0x52e,
		   /* e3 */ 0x311,
		   /* e4 */ 0x4ae,
		   /* e5 */ 0x291,
		   /* e6 */ 0x191,
		   /* e7 */ 0x071,
		   /* e8 */ 0x46e,
		   /* e9 */ 0x251,
		   /* ea */ 0x151,
		   /* eb */ 0x348,
		   /* ec */ 0x0d1,
		   /* ed */ 0x2c8,
		   /* ee */ 0x1c8,
		   /* ef */ 0x68e,
		   /* f0 */ 0x64e,
		   /* f1 */ 0x231,
		   /* f2 */ 0x131,
		   /* f3 */ 0x321,
		   /* f4 */ 0x0b1,
		   /* f5 */ 0x2a1,
		   /* f6 */ 0x1a1,
		   /* f7 */ 0x45e,
		   /* f8 */ 0x4ce,
		   /* f9 */ 0x261,
		   /* fa */ 0x161,
		   /* fb */ 0x49e,
		   /* fc */ 0x0e1,
		   /* fd */ 0x51e,
		   /* fe */ 0x61e,
		   /* ff */ 0x54e,
		} };
	

// GF Antilog lookup table table
static unsigned char CCSDS_alpha_to[NN+1] = {
0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x87,0x89,0x95,0xad,0xdd,0x3d,0x7a,0xf4,
0x6f,0xde,0x3b,0x76,0xec,0x5f,0xbe,0xfb,0x71,0xe2,0x43,0x86,0x8b,0x91,0xa5,0xcd,
0x1d,0x3a,0x74,0xe8,0x57,0xae,0xdb,0x31,0x62,0xc4,0x0f,0x1e,0x3c,0x78,0xf0,0x67,
0xce,0x1b,0x36,0x6c,0xd8,0x37,0x6e,0xdc,0x3f,0x7e,0xfc,0x7f,0xfe,0x7b,0xf6,0x6b,
0xd6,0x2b,0x56,0xac,0xdf,0x39,0x72,0xe4,0x4f,0x9e,0xbb,0xf1,0x65,0xca,0x13,0x26,
0x4c,0x98,0xb7,0xe9,0x55,0xaa,0xd3,0x21,0x42,0x84,0x8f,0x99,0xb5,0xed,0x5d,0xba,
0xf3,0x61,0xc2,0x03,0x06,0x0c,0x18,0x30,0x60,0xc0,0x07,0x0e,0x1c,0x38,0x70,0xe0,
0x47,0x8e,0x9b,0xb1,0xe5,0x4d,0x9a,0xb3,0xe1,0x45,0x8a,0x93,0xa1,0xc5,0x0d,0x1a,
0x34,0x68,0xd0,0x27,0x4e,0x9c,0xbf,0xf9,0x75,0xea,0x53,0xa6,0xcb,0x11,0x22,0x44,
0x88,0x97,0xa9,0xd5,0x2d,0x5a,0xb4,0xef,0x59,0xb2,0xe3,0x41,0x82,0x83,0x81,0x85,
0x8d,0x9d,0xbd,0xfd,0x7d,0xfa,0x73,0xe6,0x4b,0x96,0xab,0xd1,0x25,0x4a,0x94,0xaf,
0xd9,0x35,0x6a,0xd4,0x2f,0x5e,0xbc,0xff,0x79,0xf2,0x63,0xc6,0x0b,0x16,0x2c,0x58,
0xb0,0xe7,0x49,0x92,0xa3,0xc1,0x05,0x0a,0x14,0x28,0x50,0xa0,0xc7,0x09,0x12,0x24,
0x48,0x90,0xa7,0xc9,0x15,0x2a,0x54,0xa8,0xd7,0x29,0x52,0xa4,0xcf,0x19,0x32,0x64,
0xc8,0x17,0x2e,0x5c,0xb8,0xf7,0x69,0xd2,0x23,0x46,0x8c,0x9f,0xb9,0xf5,0x6d,0xda,
0x33,0x66,0xcc,0x1f,0x3e,0x7c,0xf8,0x77,0xee,0x5b,0xb6,0xeb,0x51,0xa2,0xc3,0x00,
};

// GF log lookup table. Special value represents log(0)
static unsigned char CCSDS_index_of[NN+1] = {
 A0,  0,  1, 99,  2,198,100,106,  3,205,199,188,101,126,107, 42,
  4,141,206, 78,200,212,189,225,102,221,127, 49,108, 32, 43,243,
  5, 87,142,232,207,172, 79,131,201,217,213, 65,190,148,226,180,
103, 39,222,240,128,177, 50, 53,109, 69, 33, 18, 44, 13,244, 56,
  6,155, 88, 26,143,121,233,112,208,194,173,168, 80,117,132, 72,
202,252,218,138,214, 84, 66, 36,191,152,149,249,227, 94,181, 21,
104, 97, 40,186,223, 76,241, 47,129,230,178, 63, 51,238, 54, 16,
110, 24, 70,166, 34,136, 19,247, 45,184, 14, 61,245,164, 57, 59,
  7,158,156,157, 89,159, 27,  8,144,  9,122, 28,234,160,113, 90,
209, 29,195,123,174, 10,169,145, 81, 91,118,114,133,161, 73,235,
203,124,253,196,219, 30,139,210,215,146, 85,170, 67, 11, 37,175,
192,115,153,119,150, 92,250, 82,228,236, 95, 74,182,162, 22,134,
105,197, 98,254, 41,125,187,204,224,211, 77,140,242, 31, 48,220,
130,171,231, 86,179,147, 64,216, 52,176,239, 38, 55, 12, 17, 68,
111,120, 25,154, 71,116,167,193, 35, 83,137,251, 20, 93,248,151,
 46, 75,185, 96, 15,237, 62,229,246,135,165, 23, 58,163, 60,183,
};

// Only half the coefficients are given here because the
// generator polynomial is palindromic; G0 = G32, G1 = G31, etc.
// Only G16 is unique
static unsigned char CCSDS_poly[] = {
  0,249,  59, 66,  4,  43,126,251, 97,  30,   3,213, 50, 66,170,   5,
  24,
};

static inline int modnn(int x){
  while (x >= NN) {
    x -= NN;
    x = (x >> 8) + (x & NN);
  }
  return x;
}

// Update Reed-Solomon encoder
// parity -> 32-byte reed-solomon encoder state; clear this to zero before each frame
void update_rs(
   unsigned char parity[32], // 32-byte encoder state; zero before each frame
   unsigned char c)          // Current data byte to update
{
  unsigned char feedback;
  int j,t;

  assert(parity != NULL);
  feedback = CCSDS_index_of[c ^ parity[0]];
  if(feedback != A0){ // only if feedback is non-zero
    // Take advantage of palindromic polynomial to halve the multiplies
    // Do G1...G15, which is the same as G17...G31
    for(j=1;j<(int)(NP/2);j++){
      t = CCSDS_alpha_to[modnn(feedback + CCSDS_poly[j])];
      parity[j] ^= t;
      parity[NP-j] ^= t;
    }
    // Do G16, which is used in only parity[16]
    t = CCSDS_alpha_to[modnn(feedback + CCSDS_poly[j])];
    parity[j] ^= t;
  }
  // shift left
  memmove(&parity[0],&parity[1],NP-1);
  // G0 is 1 in alpha form, 0 in index form; don't need to multiply by it
  parity[NP-1] = CCSDS_alpha_to[feedback];
  //taskYIELD();
}

#define SYNC  (0x0fa) // K.28.5, RD=-1 
 
void write_little_endian(unsigned int word, int num_bytes, FILE *wav_file)
{
	unsigned buf;
	while(num_bytes>0)
	{   buf = word & 0xff;
		fwrite(&buf, 1,1, wav_file);
		num_bytes--;
	word >>= 8;
	}
}

void config_radio()
{
  Serial.println("Configuring radio");	
/*	
  if (!wifi) 
     pinMode(LED_BUILTIN, OUTPUT);
  	
  pinMode(PTT_PIN, OUTPUT);  // PTT active LOW
  digitalWrite(PTT_PIN, HIGH);

  pinMode(PD_PIN, OUTPUT);  // PD active HIGH
  digitalWrite(PD_PIN, LOW);  

  pinMode(TEMPERATURE_PIN, INPUT);
  pinMode(AUDIO_IN_PIN, INPUT);
*/	
  if ((mode == AFSK) || (mode == SSTV) || (mode == CW)) {
	  
    digitalWrite(PD_PIN, HIGH);  // Enable SR_FRS
//    pinMode(AUDIO_OUT_PIN, OUTPUT);	  
//    program_radio();  // remove for now

  } else if (mode == BPSK)  {
/*    	 
    int ret = 1;
    int i = 0;	  
    while ((i++ < 5) && (ret != 0)) {
      ret = clockgen.setClockBPSK();	    
      Serial.println("Config clock for BPSK");
    }	 	  
*/	
	  
      if (clockgen.setClockBPSK()) {	  
	 start_clockgen();
	 clockgen.setClockBPSK();
	 Serial.println("Config clock for BPSK");     
      } else {
	 Serial.println("Config clock for BPSK");          
      }	  
    transmit_on();	
  }	
  else if (mode == FSK)  {//  || (mode == SSTV))
/*
    int ret = 1;
    int i = 0;	  
    while ((i++ < 5) && (ret != 0)) {
      ret = clockgen.setClockFSK();	    
      Serial.println("Config clock for FSK");
    }	 
*/	 
      if (clockgen.setClockFSK()) {	  
	 start_clockgen();
	 clockgen.setClockFSK();
	 Serial.println("Config clock for FSK");       
      }	else {
	 Serial.println("Config clock for FSK");          
      }	  
    transmit_on();
  }
}

void test_radio()
{
// send a carrier for 3 seconds
  Serial.println("Testing radio...\n\n");

  digitalWrite(MAIN_LED_BLUE, HIGH);	
  digitalWrite(PTT_PIN, LOW);
  sleep(3.0); // delay(3000);
  digitalWrite(PTT_PIN, HIGH);
  digitalWrite(MAIN_LED_BLUE, LOW);
}

void read_ina219()
{
  unsigned long read_time = millis();
	
  if (voltage_read && !i2c_bus1 && !i2c_bus3)
    Serial.println("Nothing to read");
/*	
#ifdef PICO_0V1	
  digitalWrite(PI_3V3_PIN, HIGH);
#endif	
*/	
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
	
  if (i2c_1) {	
    shuntvoltage = ina219_1_0x40.getShuntVoltage_mV();
    busvoltage = ina219_1_0x40.getBusVoltage_V();
    current_mA = ina219_1_0x40.getCurrent_mA();
    loadvoltage = busvoltage + (shuntvoltage / 1000);

    if ((debug_mode) || (voltage_read))	{    
      Serial.print("+X   (1 0x40) Voltage:  "); 
      Serial.print(loadvoltage);
      Serial.print("V  Current: "); 
      Serial.print(current_mA); 
      Serial.println(" mA");
    }
    voltage[PLUS_X] = loadvoltage;
    current[PLUS_X] = current_mA;
  } else {
    voltage[PLUS_X] = 0.0;
    current[PLUS_X] = 0.0;	  
  }
	
  if (i2c2) {		  
  shuntvoltage = ina219_1_0x41.getShuntVoltage_mV();
  busvoltage = ina219_1_0x41.getBusVoltage_V();
  current_mA = ina219_1_0x41.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  if ((debug_mode) || (voltage_read))	{  	  
    Serial.print("+Y   (1 0x41) Voltage:  "); 
    Serial.print(loadvoltage);
    Serial.print("V  Current: "); 
    Serial.print(current_mA); 
    Serial.println(" mA");
  }
  voltage[PLUS_Y] = loadvoltage;
  current[PLUS_Y] = current_mA;
  } else {
    voltage[PLUS_Y] = 0.0;
    current[PLUS_Y] = 0.0;	  
  }
	
  if (i2c3) {		
  shuntvoltage = ina219_1_0x44.getShuntVoltage_mV();
  busvoltage = ina219_1_0x44.getBusVoltage_V();
  current_mA = ina219_1_0x44.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
	  
  if ((debug_mode) || (voltage_read))	{  	
  Serial.print("+Bat (1 0x44) Voltage:  "); 
  Serial.print(loadvoltage);
  Serial.print("V  Current: "); 
  Serial.print(current_mA); 
  Serial.println(" mA");
  }
  voltage[BAT] = loadvoltage;
  current[BAT] = current_mA;
/*
  shuntvoltage = ina219_1_0x45.getShuntVoltage_mV();
  busvoltage = ina219_1_0x45.getBusVoltage_V();
  current_mA = ina219_1_0x45.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
	  
  if ((debug_mode) || (voltage_read))	{  	
  Serial.print("1 0x45 Voltage:  "); 
  Serial.print(loadvoltage);
  Serial.print("V  Current: "); 
  Serial.print(current_mA); 
  Serial.println(" mA");
  }
*/  
  voltage[BUS] = loadvoltage;  // since battery directly supplies, make BUS same as BAT for FoxTelem
  current[BUS] = current_mA;	
	  
	  
  } else {
    voltage[BAT] = 0.0;
    current[BAT] = 0.0;	  
  }
	
  if (i2c5) {
  shuntvoltage = ina219_2_0x40.getShuntVoltage_mV();
  busvoltage = ina219_2_0x40.getBusVoltage_V();
  current_mA = ina219_2_0x40.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
	  
  if ((debug_mode) || (voltage_read))	{  	
  Serial.print("+Z   (2 0x40) Voltage:  "); 
  Serial.print(loadvoltage);
  Serial.print("V  Current: "); 
  Serial.print(current_mA); 
  Serial.println(" mA");
  }
  voltage[PLUS_Z] = loadvoltage;
  current[PLUS_Z] = current_mA;
  } else {
    voltage[PLUS_Z] = 0.0;
    current[PLUS_Z] = 0.0;	  
  }
	
  if (i2c6) {	
  shuntvoltage = ina219_2_0x41.getShuntVoltage_mV();
  busvoltage = ina219_2_0x41.getBusVoltage_V();
  current_mA = ina219_2_0x41.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
	  
  if ((debug_mode) || (voltage_read))	{  	
  Serial.print("-X   (2 0x41) Voltage:  "); 
  Serial.print(loadvoltage);
  Serial.print("V  Current: "); 
  Serial.print(current_mA); 
  Serial.println(" mA");
  }
  voltage[MINUS_X] = loadvoltage;
  current[MINUS_X] = current_mA;
  } else {
    voltage[MINUS_X] = 0.0;
    current[MINUS_X] = 0.0;	  
  }
	
  if (i2c7) {		
  shuntvoltage = ina219_2_0x44.getShuntVoltage_mV();
  busvoltage = ina219_2_0x44.getBusVoltage_V();
  current_mA = ina219_2_0x44.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  if ((debug_mode) || (voltage_read))	{  	  
  Serial.print("-Y   (2 0x44) Voltage:  "); 
  Serial.print(loadvoltage);
  Serial.print("V  Current: "); 
  Serial.print(current_mA); 
  Serial.println(" mA");
  }
  voltage[MINUS_Y] = loadvoltage;
  current[MINUS_Y] = current_mA;
  } else {
    voltage[MINUS_Y] = 0.0;
    current[MINUS_Y] = 0.0;	  
  }
	
  if (i2c8) {	
  shuntvoltage = ina219_2_0x45.getShuntVoltage_mV();
  busvoltage = ina219_2_0x45.getBusVoltage_V();
  current_mA = ina219_2_0x45.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
	  
  if ((debug_mode) || (voltage_read))	{  	
  Serial.print("-Z   (2 0x45) Voltage:  "); 
  Serial.print(loadvoltage);
  Serial.print("V  Current: "); 
  Serial.print(current_mA); 
  Serial.println(" mA");
  }
  voltage[MINUS_Z] = loadvoltage;
  current[MINUS_Z] = current_mA;
  } else {
    voltage[MINUS_Z] = 0.0;
    current[MINUS_Z] = 0.0;	  
  }
  voltage_read = false;	
	
  if ((millis() - read_time) > 1000)
    Serial.println("There is an I2C sensor problem");
}

void read_sensors()
{
	
}	

//  Returns lower digit of a number which must be less than 99
//
int lower_digit(int number) {

  int digit = 0;
  if (number < 100)
    digit = number - ((int)(number / 10) * 10);
  else
   Serial.println("ERROR: Not a digit in lower_digit!\n");
  return digit;
}

// Returns upper digit of a number which must be less than 99
//
int upper_digit(int number) {

  int digit = 0;
  if (number < 100)

    digit = (int)(number / 10);
  else
    Serial.println("ERROR: Not a digit in upper_digit!\n");
  return digit;
}

void print_string(char *string)
{
  int count = 0;
  while ((count < 250) && (string[count] != 0))
  {
    Serial.print(string[count++]);	  
  }
  Serial.println(" ");	
}

void start_payload() {
	
  Serial1.begin(115200);  // Pi UART faster speed

  Serial.println("Starting payload!");
  
  blink_setup();
/*
  blink(500);
  sleep(0.25); // delay(250);
  blink(500);
  sleep(0.25); // delay(250);
  led_set(greenLED, HIGH);
  sleep(0.25); // delay(250);
  led_set(greenLED, LOW);
  led_set(blueLED, HIGH);
  sleep(0.25); // delay(250);
  led_set(blueLED, LOW);
*/ 
	
  if (bme.begin()) {
    bmePresent = 1;
  } else {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    bmePresent = 0;
  }
	
  Wire.begin();
  Wire.beginTransmission(0x68);
  if (Wire.endTransmission() != 0)  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    mpuPresent = 0;
  }
  else {
    mpuPresent = 1;
    mpu6050.begin();	  
	  
  long flag;
  float xOffset;
  float yOffset;	  
  float zOffset;		  
  EEPROM.get(0, flag);
	  
  if ((flag == 0xA07) && (payload_command != PAYLOAD_RESET))
  {
    Serial.println("Reading gyro offsets from EEPROM\n");

    EEPROM.get(4, xOffset);
    EEPROM.get(8, yOffset);
    EEPROM.get(12, zOffset);
	  
    Serial.println(xOffset, DEC);
    Serial.println(yOffset, DEC);
    Serial.println(zOffset, DEC);

    mpu6050.setGyroOffsets(xOffset, yOffset, zOffset);
	  	  
  }
  else
  {
    payload_command = false;
	  
    Serial.println("Calculating gyro offsets and storing in EEPROM\n");

    mpu6050.calcGyroOffsets(true);
/*
    eeprom_word_write(0, 0xA07);
    eeprom_word_write(1, (int)(mpu6050.getGyroXoffset() * 100.0) + 0.5);
    eeprom_word_write(2, (int)(mpu6050.getGyroYoffset() * 100.0) + 0.5);
    eeprom_word_write(3, (int)(mpu6050.getGyroZoffset() * 100.0) + 0.5);
*/
    flag = 0xA07;
    xOffset = mpu6050.getGyroXoffset();
    yOffset = mpu6050.getGyroYoffset();
    zOffset = mpu6050.getGyroZoffset();
	  
    EEPROM.put(0, flag);
    EEPROM.put(4, xOffset);
    EEPROM.put(8, yOffset);
    EEPROM.put(12, zOffset);
	  
    if (EEPROM.commit()) {
      Serial.println("EEPROM successfully committed");
    } else {
      Serial.println("ERROR! EEPROM commit failed");
    }
	  
    Serial.println(" ");	  
    float f;	  
    EEPROM.get(0, flag);  		  
    Serial.println(flag, HEX);
    EEPROM.get(4, f);  
    Serial.println(f);		  
    EEPROM.get(8, f);  
    Serial.println(f);	
    EEPROM.get(12, f);  
    Serial.println(f);		  
  }
  }		

  if (!(payload = bmePresent || mpuPresent))
    Serial.println("No payload sensors detected");
	
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
	
}

void read_payload()
{
  payload_str[0] = '\0';  // clear the payload string
//  print_string(payload_str);	
	
//  if ((Serial.available() > 0)|| first_time == true) 
  {
//    blink(50);
    char result = Serial.read();
    char header[] = "OK BME280 ";
    char str[100];
	  
    strcpy(payload_str, header);
//    print_string(payload_str);		  
    if (bmePresent) 
//    	sprintf(str, "%4.2f %6.2f %6.2f %5.2f ", 
   	sprintf(str, "%.1f %.2f %.1f %.2f ", 
	  bme.readTemperature(), bme.readPressure() / 100.0, bme.readAltitude(SEALEVELPRESSURE_HPA), bme.readHumidity());
    else
        sprintf(str, "0.0 0.0 0.0 0.0 "); 
    strcat(payload_str, str);
//    print_string(payload_str);		  

    if (mpuPresent) 	 { 
//    print_string(payload_str);	
      mpu6050.update();

//    sprintf(str, " MPU6050 %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f ",
      sprintf(str, "MPU6050 %.1f %.1f %.1f %.1f %.1f %.1f ",
        mpu6050.getGyroX(), mpu6050.getGyroY(), mpu6050.getGyroZ(), mpu6050.getAccX(), mpu6050.getAccY(), mpu6050.getAccZ()); 

      float rotation = sqrt(mpu6050.getGyroX()*mpu6050.getGyroX() + mpu6050.getGyroY()*mpu6050.getGyroY() + mpu6050.getGyroZ()*mpu6050.getGyroZ()); 
      float acceleration = sqrt(mpu6050.getAccX()*mpu6050.getAccX() + mpu6050.getAccY()*mpu6050.getAccY() + mpu6050.getAccZ()*mpu6050.getAccZ()); 
//    Serial.print(rotation);
//    Serial.print(" ");
//    Serial.println(acceleration);
 
      if (acceleration > 1.2)
        led_set(STEM_LED_GREEN, HIGH);
      else
        led_set(STEM_LED_GREEN, LOW);
        
      if (rotation > 5)
        led_set(STEM_LED_BLUE, HIGH);
      else
        led_set(STEM_LED_BLUE, LOW);  	    
	    
    }   else
        sprintf(str, "MPU6050 0.0 0.0 0.0 0.0 0.0 0.0 ");     
    strcat(payload_str, str);
//    print_string(payload_str);		  
	  
    sensorValue = analogRead(TEMPERATURE_PIN);
    //Serial.println(sensorValue);  
    Temp = T1 + (sensorValue - R1) *((T2 - T1)/(R2 - R1));
//    sprintf(str, "XS %.1f %.1f\n", Temp, Sensor1);
    sprintf(str, "XS %.1f 0.0\n", Temp);
    strcat(payload_str, str);
	  
    if ((debug_mode) || (payload_command == PAYLOAD_QUERY)) {
      payload_command = false;	    
      print_string(payload_str);
    }
	  
/*	  
    if (result == 'R') {
      Serial.println("OK");
      delay(100);
      first_time = true;
      setup();
    }
    else if (result == 'C') {
      Serial.println("Clearing stored gyro offsets in EEPROM\n");
      EEPROM.put(0, (float)0.0);
      first_time = true;
      setup();
    }
*/	  
//    if ((result == '?') || first_time == true)
	  
/*
    if (true)
    {
      first_time = false;
      if (bmePresent) {
        Serial.print("OK BME280 ");
        Serial.print(bme.readTemperature());
        Serial.print(" ");
        Serial.print(bme.readPressure() / 100.0F);
        Serial.print(" ");
        Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
        Serial.print(" ");
        Serial.print(bme.readHumidity());
      } else
      {
        Serial.print("OK BME280 0.0 0.0 0.0 0.0");
      }
    if (mpuPresent) 	 { 
      mpu6050.update();

      Serial.print(" MPU6050 ");
      Serial.print(mpu6050.getGyroX());
      Serial.print(" ");
      Serial.print(mpu6050.getGyroY());
      Serial.print(" ");
      Serial.print(mpu6050.getGyroZ());

    Serial.print(" ");
    Serial.print(mpu6050.getAccX());   
    Serial.print(" ");
    Serial.print(mpu6050.getAccY());   
    Serial.print(" ");
    Serial.print(mpu6050.getAccZ());  
    }
   else
        Serial.print(" MPU6050 0.0 0.0 0.0 0.0 0.0 0.0 "); 	    

    Serial.print(" XS ");
    Serial.print(Temp);   
    Serial.print(" ");
    Serial.println(Sensor1);    	  
  
  if (mpuPresent) 	 { 	    
    float rotation = sqrt(mpu6050.getGyroX()*mpu6050.getGyroX() + mpu6050.getGyroY()*mpu6050.getGyroY() + mpu6050.getGyroZ()*mpu6050.getGyroZ()); 
    float acceleration = sqrt(mpu6050.getAccX()*mpu6050.getAccX() + mpu6050.getAccY()*mpu6050.getAccY() + mpu6050.getAccZ()*mpu6050.getAccZ()); 
//    Serial.print(rotation);
//    Serial.print(" ");
//    Serial.println(acceleration);
 
    if (acceleration > 1.2)
        led_set(greenLED, HIGH);
    else
        led_set(greenLED, LOW);
        
    if (rotation > 5)
        led_set(blueLED, HIGH);
    else
        led_set(blueLED, LOW);  
    } 
*/
	  
//   }    
  }

  if (Serial1.available() > 0) {
 
//    blink(50);
    char result = Serial1.read();
    //    Serial1.println(result);

    if (result == 'R') {
      Serial1.println("OK");
      delay(100);
      first_read = true;
      setup();
    }

    if (result == '?')
    {
      if (bmePresent) {
        Serial1.print("OK BME280 ");
        Serial1.print(bme.readTemperature());
        Serial1.print(" ");
        Serial1.print(bme.readPressure() / 100.0F);
        Serial1.print(" ");
        Serial1.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
        Serial1.print(" ");
        Serial1.print(bme.readHumidity());
      } else
      {
        Serial1.print("OK BME280 0.0 0.0 0.0 0.0");
      }
      mpu6050.update();

      Serial1.print(" MPU6050 ");
      Serial1.print(mpu6050.getGyroX());
      Serial1.print(" ");
      Serial1.print(mpu6050.getGyroY());
      Serial1.print(" ");
      Serial1.print(mpu6050.getGyroZ());

    Serial1.print(" ");
    Serial1.print(mpu6050.getAccX());   
    Serial1.print(" ");
    Serial1.print(mpu6050.getAccY());   
    Serial1.print(" ");
    Serial1.print(mpu6050.getAccZ());   

    sensorValue = analogRead(TEMPERATURE_PIN);
    //Serial.println(sensorValue);  
    Temp = T1 + (sensorValue - R1) *((T2 - T1)/(R2 - R1));

    Serial1.print(" XS ");
    Serial1.print(Temp);   
    Serial1.print(" ");
    Serial1.println(Sensor2);              
     
    float rotation = sqrt(mpu6050.getGyroX()*mpu6050.getGyroX() + mpu6050.getGyroY()*mpu6050.getGyroY() + mpu6050.getGyroZ()*mpu6050.getGyroZ()); 
    float acceleration = sqrt(mpu6050.getAccX()*mpu6050.getAccX() + mpu6050.getAccY()*mpu6050.getAccY() + mpu6050.getAccZ()*mpu6050.getAccZ()); 
//    Serial.print(rotation);
//    Serial.print(" ");
//    Serial.println(acceleration);

    if (first_read == true) {
      first_read = false;
      rest = acceleration;
    }

    if (acceleration > 1.2 * rest)
        led_set(greenLED, HIGH);
    else
        led_set(greenLED, LOW);
        
    if (rotation > 5)
        led_set(blueLED, HIGH);
    else
        led_set(blueLED, LOW);
    }
  }

//  delay(100);
}

/*
void payload_OK_only()
{
  payload_str[0] = '\0';  // clear the payload string
	
  if ((Serial.available() > 0)|| first_time == true)   // commented back in
  {
    blink(50);
    char result = Serial.read();
    char header[] = "OK BME280 ";
    char str[100];
	  
    strcpy(payload_str, header);
//    print_string(str);		  
    if (bmePresent) 
//    	sprintf(str, "%4.2f %6.2f %6.2f %5.2f ", 
   	sprintf(str, "%.1f %.2f %.1f %.2f ", 
	  bme.readTemperature(), bme.readPressure() / 100.0, bme.readAltitude(SEALEVELPRESSURE_HPA), bme.readHumidity());
    else
        sprintf(str, "OK BME280 0.0 0.0 0.0 0.0 "); 
    strcat(payload_str, str);

    if (mpuPresent) 	 { 
//    print_string(payload_str);	
    mpu6050.update();

//    sprintf(str, " MPU6050 %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f ",
    sprintf(str, " MPU6050 %.1f %.1f %.1f %.1f %.1f %.1f ",
      mpu6050.getGyroX(), mpu6050.getGyroY(), mpu6050.getGyroZ(), mpu6050.getAccX(), mpu6050.getAccY(), mpu6050.getAccZ()); 
    
    strcat(payload_str, str);
//    print_string(payload_str);
    }
    if (result == 'R') {
      Serial.println("OK");
      delay(100);
      first_time = true;
      setup();
    }
    else if (result == 'C') {
      Serial.println("Clearing stored gyro offsets in EEPROM\n");
      EEPROM.put(0, (float)0.0);
      first_time = true;
      setup();
    }
    if ((result == '?') || first_time == true)  // commented back in
    if (true)
    {
      first_time = false;
      if (bmePresent) {
        Serial.print("OK BME280 ");
        Serial.print(bme.readTemperature());
        Serial.print(" ");
        Serial.print(bme.readPressure() / 100.0F);
        Serial.print(" ");
        Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
        Serial.print(" ");
        Serial.print(bme.readHumidity());
      } else
      {
        Serial.print("OK BME280 0.0 0.0 0.0 0.0");
      }
    if (mpuPresent) 	 { 
      mpu6050.update();

      Serial.print(" MPU6050 ");
      Serial.print(mpu6050.getGyroX());
      Serial.print(" ");
      Serial.print(mpu6050.getGyroY());
      Serial.print(" ");
      Serial.print(mpu6050.getGyroZ());

    Serial.print(" ");
    Serial.print(mpu6050.getAccX());   
    Serial.print(" ");
    Serial.print(mpu6050.getAccY());   
    Serial.print(" ");
    Serial.print(mpu6050.getAccZ());  
    }
	    
    sensorValue = analogRead(TEMPERATURE_PIN);
    //Serial.println(sensorValue);  
    Temp = T1 + (sensorValue - R1) *((T2 - T1)/(R2 - R1));

    
    Serial.print(" XS ");
    Serial.print(Temp);   
    Serial.print(" ");
    Serial.println(Sensor1);               
  
  if (mpuPresent) 	 { 	    
    float rotation = sqrt(mpu6050.getGyroX()*mpu6050.getGyroX() + mpu6050.getGyroY()*mpu6050.getGyroY() + mpu6050.getGyroZ()*mpu6050.getGyroZ()); 
    float acceleration = sqrt(mpu6050.getAccX()*mpu6050.getAccX() + mpu6050.getAccY()*mpu6050.getAccY() + mpu6050.getAccZ()*mpu6050.getAccZ()); 
//    Serial.print(rotation);
//    Serial.print(" ");
//    Serial.println(acceleration);
 
    if (acceleration > 1.2)
        led_set(greenLED, HIGH);
    else
        led_set(greenLED, LOW);
        
    if (rotation > 5)
        led_set(blueLED, HIGH);
    else
        led_set(blueLED, LOW);  
    } 
   }    
  }

  if (Serial1.available() > 0) {
 
    blink(50);
    char result = Serial1.read();
    //    Serial1.println(result);

    if (result == 'R') {
      Serial1.println("OK");
      delay(100);
      first_read = true;
      setup();
    }

    if (result == '?')
    {
      if (bmePresent) {
        Serial1.print("OK BME280 ");
        Serial1.print(bme.readTemperature());
        Serial1.print(" ");
        Serial1.print(bme.readPressure() / 100.0F);
        Serial1.print(" ");
        Serial1.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
        Serial1.print(" ");
        Serial1.print(bme.readHumidity());
      } else
      {
        Serial1.print("OK BME280 0.0 0.0 0.0 0.0");
      }
      mpu6050.update();

      Serial1.print(" MPU6050 ");
      Serial1.print(mpu6050.getGyroX());
      Serial1.print(" ");
      Serial1.print(mpu6050.getGyroY());
      Serial1.print(" ");
      Serial1.print(mpu6050.getGyroZ());

    Serial1.print(" ");
    Serial1.print(mpu6050.getAccX());   
    Serial1.print(" ");
    Serial1.print(mpu6050.getAccY());   
    Serial1.print(" ");
    Serial1.print(mpu6050.getAccZ());   

    sensorValue = analogRead(TEMPERATURE_PIN);
    //Serial.println(sensorValue);  
    Temp = T1 + (sensorValue - R1) *((T2 - T1)/(R2 - R1));

    Serial1.print(" XS ");
    Serial1.print(Temp);   
    Serial1.print(" ");
    Serial1.println(Sensor2);              
     
    float rotation = sqrt(mpu6050.getGyroX()*mpu6050.getGyroX() + mpu6050.getGyroY()*mpu6050.getGyroY() + mpu6050.getGyroZ()*mpu6050.getGyroZ()); 
    float acceleration = sqrt(mpu6050.getAccX()*mpu6050.getAccX() + mpu6050.getAccY()*mpu6050.getAccY() + mpu6050.getAccZ()*mpu6050.getAccZ()); 
//    Serial.print(rotation);
//    Serial.print(" ");
//    Serial.println(acceleration);

    if (first_read == true) {
      first_read = false;
      rest = acceleration;
    }

    if (acceleration > 1.2 * rest)
        led_set(greenLED, HIGH);
    else
        led_set(greenLED, LOW);
        
    if (rotation > 5)
        led_set(blueLED, HIGH);
    else
        led_set(blueLED, LOW);
    }
  }

  delay(100);
}

*/	
/*
void eeprom_word_write(int addr, int val)
{
  EEPROM.write(addr * 2, lowByte(val));
  EEPROM.write(addr * 2 + 1, highByte(val));
}

short eeprom_word_read(int addr)
{
  return ((EEPROM.read(addr * 2 + 1) << 8) | EEPROM.read(addr * 2));
}
*/

void blink_setup() 
{
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)  
  // initialize digital pin PB1 as an output.
  pinMode(PC13, OUTPUT);
  pinMode(PB9, OUTPUT);
  pinMode(PB8, OUTPUT);
#endif

#if defined __AVR_ATmega32U4__ || ARDUINO_ARCH_RP2040
  pinMode(RXLED, OUTPUT);  // Set RX LED as an output
  // TX LED is set as an output behind the scenes
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED,OUTPUT);
#endif
}

void blink(int length)
{
  digitalWrite(LED_BUILTIN, HIGH);   // set the built-in LED ON
  
  sleep(length/1000.0); // delay(length);              // wait for a lenth of time

  digitalWrite(LED_BUILTIN, LOW);   // set the built-in LED off
}

void led_set(int ledPin, bool state)
{
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)
  if (ledPin == greenLED)
    digitalWrite(PB9, state);
  else if (ledPin == blueLED)
    digitalWrite(PB8, state);    
#endif

#if defined __AVR_ATmega32U4__ || ARDUINO_ARCH_RP2040
  digitalWrite(ledPin, state);   
#endif  
}

void start_ina219() {
//#define PI_3V3_PIN 9 // for v0.1 hardware
  Serial.println("Starting INA219");
//  Serial.println(PI_3V3_PIN);	

	
  i2c_1 = i2c2 = i2c3 = i2c4 = i2c5 = i2c6 = i2c7 = i2c8 = false;	

  if (!ina219_started)	{
#ifndef PICO_0V1	
  // check if Pi is present by 3.3V voltage
  pinMode(PI_3V3_PIN, INPUT); 	
//  Serial.print("Pi 3.3V: ");
//  Serial.println(digitalRead(PI_3V3_PIN));
  if (digitalRead(PI_3V3_PIN) == LOW)  {
//  if (true)  {
    Serial.println("Pico powering INA219s through 3.3V pin");  
    pinMode(PI_3V3_PIN, OUTPUT);
    digitalWrite(PI_3V3_PIN, HIGH);	  
  }  else {
    Serial.println("Not powering INA219s since Pi Zero is present");	  
  }
#else
    Serial.println("Powering INA219s through 3.3V pin");  
    pinMode(PI_3V3_PIN, OUTPUT);
    digitalWrite(PI_3V3_PIN, HIGH);	
#endif
  }	
  sleep(0.1);
  if (!(i2c_1 = ina219_1_0x40.begin()))  // check i2c bus 1
    Serial.println("I2C +X sensor (bus 1 0x40) not found");
  if (!(i2c2 = ina219_1_0x41.begin()))
    Serial.println("I2C +Y sensor (bus 1 0x41) not found");
  if (!(i2c3 = ina219_1_0x44.begin()))
    Serial.println("I2C Batt sensor (bus 1 0x44) not found");
//  ina219_1_0x45.begin();
	
  i2c_bus1 = i2c_1 || i2c2 || i2c3; 	
   
  Wire1.setSDA(2); 
  Wire1.setSCL(3);
  Wire1.begin(); 
	
  if (!(i2c5 = ina219_2_0x40.begin(&Wire1)))  // check i2c bus 2
    Serial.println("I2C +Z sensor (bus 2 0x40) not found");
  if (!(i2c6 = ina219_2_0x41.begin(&Wire1)))
    Serial.println("I2C -X sensor (bus 2 0x41) not found");
  if (!(i2c7 = ina219_2_0x44.begin(&Wire1)))
    Serial.println("I2C -Y sensor (bus 2 0x44) not found");
  if (!(i2c8 = ina219_2_0x45.begin(&Wire1)))
    Serial.println("I2C -Z sensor (bus 2 0x45) not found");
	
  i2c_bus3 = i2c5 || i2c6 || i2c7 || i2c8; 		
	
  Serial.print("I2C bus 1: ");
  Serial.print(i2c_bus1);	
  Serial.print(" I2C bus 3: ");
  Serial.println(i2c_bus3);
	
/*	
  if (i2c_bus1) {	
  ina219_1_0x40.setCalibration_16V_400mA(); 
  ina219_1_0x41.setCalibration_16V_400mA(); 
  ina219_1_0x44.setCalibration_16V_400mA(); 
  ina219_1_0x45.setCalibration_16V_400mA(); 	
  }
  if (i2c_bus3) {	 	
  ina219_2_0x40.setCalibration_16V_400mA(); 
  ina219_2_0x41.setCalibration_16V_400mA(); 
  ina219_2_0x44.setCalibration_16V_400mA(); 
  ina219_2_0x45.setCalibration_16V_400mA(); 	
  }
*/  
  ina219_started = true;	
}

void start_pwm() {
// based on code https://github.com/rgrosset/pico-pwm-audio
// and
//  https://qiita.com/keyyum/items/8cb419f5278e13b6db4d	
//
  Serial.println("Starting pwm f=432.250 MHz!");
	
//  pwm_value = 128 - pwm_amplitude;
	
//  set_sys_clock_khz(125000, true); 
//  set_sys_clock_khz(133000, true); 	
  gpio_set_function(BPSK_PWM_A_PIN, GPIO_FUNC_PWM);
  gpio_set_function(BPSK_PWM_B_PIN, GPIO_FUNC_PWM);
	
  bpsk_pin_slice_A = pwm_gpio_to_slice_num(BPSK_PWM_A_PIN);
  bpsk_pin_slice_B = pwm_gpio_to_slice_num(BPSK_PWM_B_PIN);
  if (debug_mode) {	
    Serial.print(pwm_gpio_to_slice_num(BPSK_PWM_A_PIN));
    Serial.print(" ");	
    Serial.print(pwm_gpio_to_channel(BPSK_PWM_A_PIN));
    Serial.print(" ");		
    Serial.print(pwm_gpio_to_slice_num(BPSK_PWM_B_PIN));
    Serial.print(" ");	
    Serial.print(pwm_gpio_to_channel(BPSK_PWM_B_PIN));
    Serial.println(" ");	
  }
/*
  // Setup PWM interrupt to fire when PWM cycle is complete
  pwm_clear_irq(bpsk_pin_slice);
  pwm_set_irq_enabled(audio_pin_slice, true);
  // set the handle function above
  irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler); 
  irq_set_enabled(PWM_IRQ_WRAP, true);	
*/	
    config = pwm_get_default_config();
//    pwm_config_set_clkdiv(&config, 8.0); //16.0);   // 8.0f);  was 16 for some reason
    pwm_config_set_clkdiv(&config, 1.0f); // 2.0f); // 1.0f); // 16.0); //1.0f); // 1.0f); 
//    pwm_config_set_wrap(&config, 178); // 250); 
    pwm_config_set_wrap(&config, 3); // 255); // 3);
	
//    pwm_config_set_output_polarity( &config, polarity, polarity);	
    pwm_config_set_output_polarity( &config, true, false);	
    pwm_init(bpsk_pin_slice_A, &config, true);
    pwm_init(bpsk_pin_slice_B, &config, true);
//    pwm_set_gpio_level(BPSK_PWM_A_PIN, (config.top + 1) * 0.5);
//    pwm_set_gpio_level(BPSK_PWM_B_PIN, (config.top + 1) * 0.5);	
    pwm_set_gpio_level(BPSK_PWM_A_PIN, 0);  // start with off
    pwm_set_gpio_level(BPSK_PWM_B_PIN, 0);	
}
/*
void pwm_interrupt_handler() {
// based on code https://github.com/rgrosset/pico-pwm-audio
//	
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_OUT_PIN));    
//    if (wav_position < (WAV_DATA_LENGTH<<3) - 1) { 
    if (wav_position > (BUFFER_SIZE - 1)) { 
        // set pwm level 
        // allow the pwm value to repeat for 8 cycles this is >>3 
        pwm_set_gpio_level(AUDIO_OUT_PIN, buffer[wav_position]);  
        wav_position++;
    } else {
        // reset to start
        wav_position = 0;
    }
}
*/
/*	
void pwm_interrupt_handler() {
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_OUT_PIN)); 
    	pwm_counter++;   
        if (pwm_counter > pwm_counter_max) {
          pwm_counter -= pwm_counter_max;
//          if (random(0,2) == 1)
//            pwm_rnd_bit *= (-1.0);

          pwm_rnd_bit = (buffer[wav_position] > 0) ? 1 : 0;
//	  Serial.print(pwm_rnd_bit);
//	  Serial.print(" ");
		
          if ((pwm_value == (128 - pwm_amplitude)) && (pwm_rnd_bit == 1)) {
            pwm_value = 128 + pwm_amplitude;
//            Serial.print("-");
          }
          else {
            pwm_value = 128 - pwm_amplitude; 
//            Serial.print("_");
          }
	  pwm_set_gpio_level(AUDIO_OUT_PIN, pwm_value);
//	  Serial.println("wav_position: ");
//	  Serial.println(wav_position);
	  if (wav_position++ > BUFFER_SIZE) { // 300) {
		wav_position = wav_position - BUFFER_SIZE;
//		Serial.print("R");
	  }
        }  

}
*/

/*  ///
void setup1() {
  Serial.begin(9600);
  sleep(5.0);
	
  Serial.println("Setup1");	

  while(!ready) { // wait for core0 to start
    sleep(0.1);
    Serial.println(ready);	  
  }	
  Serial.print("S");
  Serial.print(" ");
  Serial.println(millis());	
}

void loop1() {
  Serial.print("l1 ");
  Serial.print(wav_position);
  Serial.print(" ");

  if (mode == FSK) 
  {
        tx_bit = (buffer[wav_position++] > 0) ? HIGH: LOW;
	
	digitalWrite(AUDIO_OUT_PIN, tx_bit);	
		
  } 
  else if (mode == BPSK)  {
	  ;
*	  
    tx_bit = (buffer[wav_position++] > 0) ? true: false;
	  
    pwm_config_set_output_polarity( &config, tx_bit, tx_bit);	
    pwm_init(bpsk_pin_slice, &config, true);
    pwm_set_gpio_level(BPSK_PWM_PIN, (config.top + 1) * 0.5);	  
*	  
  }
	
//  if (wav_position++ > BUFFER_SIZE) { // 300) {
//	wav_position = wav_position - BUFFER_SIZE;
  if (wav_position > bufLen) { // 300) {
	wav_position = wav_position - bufLen;
	Serial.print("R");
	Serial.print(" ");
	Serial.println(millis());	
  }
  delay(1); //2 1);
//  sleep(delay_time);	
	
// check pushbutton
//    int pb_value;
//    pb_value = digitalRead(MAIN_PB_PIN);
//    Serial.print("PB: ");
//    Serial.println(pb_value);
//    if (pb_value == PRESSED) 
    if (digitalRead(MAIN_PB_PIN) == PRESSED) 
      process_pushbutton();
}	    

*///

/*
void sleep(float time) {  // sleeps for intervals more than 0.1 seconds

  unsigned long time_ms = (unsigned long)(time * 1000.0);	
  unsigned long startSleep = millis();	    
  while ((millis() - startSleep) < time_ms)  {	  
    delay(100);		 
  }
}
*/

void sleep(float timer) {  // sleeps for intervals more than 0.01 milli seconds

  unsigned long time_us = (unsigned long)(timer * 1000000.0);	
  unsigned long startSleep = micros();	    
  while ((micros() - startSleep) < time_us)  {	  
//    busy_wait_us(100);
    delayMicroseconds(100);  
  }
}

void process_pushbutton() {
	
//  Serial.println("PB pressed!");  
	
  int release = FALSE;
	
//  Serial.println(digitalRead(MAIN_PB_PIN));
	
//  return;  /// just skip for now
	
//  if (!wifi) 	   	
   digitalWrite(LED_BUILTIN, HIGH);  // make sure built in LED is on before starting to blink
	
  sleep(1.0);
	
  int pb_value = digitalRead(MAIN_PB_PIN);
  if (pb_value == RELEASED) {
    Serial.println("PB: Reboot!");
    release = TRUE;
    prompt = PROMPT_REBOOT;  
  }
	
  blinkTimes(1);
  sleep(1.5);
	
  pb_value = digitalRead(MAIN_PB_PIN);
  if ((pb_value == RELEASED) && (release == FALSE)) {
    Serial.println("PB: Switch to AFSK");
    release = TRUE;
    new_mode = AFSK;
//    setup();	  
  }
	
  if (release == FALSE) {
    blinkTimes(2);
    sleep(1.5);
  }
	  
  pb_value = digitalRead(MAIN_PB_PIN);
  if ((pb_value == RELEASED) && (release == FALSE)) {
    Serial.println("PB: Switch to FSK");
    release = TRUE;
    new_mode = FSK;
  }
	
  if (release == FALSE) {
    blinkTimes(3);
    sleep(1.5);
  }
	  
  pb_value = digitalRead(MAIN_PB_PIN);
  if ((pb_value == RELEASED) && (release == FALSE)) {
    Serial.println("PB: Switch to BPSK");
    release = TRUE;
    new_mode = BPSK;	  
  }
	
  if (release == FALSE) {
    blinkTimes(4);
    sleep(1.5);
  }	
	  
  pb_value = digitalRead(MAIN_PB_PIN);
  if ((pb_value == RELEASED) && (release == FALSE)) {
    Serial.println("PB: Switch to SSTV");
    new_mode = SSTV;
    release = TRUE;
  }
	
  if (release == FALSE) {
    blinkTimes(5);
    sleep(1.5);
  }	
	  
  pb_value = digitalRead(MAIN_PB_PIN);
  if ((pb_value == RELEASED) && (release == FALSE)) {
    Serial.println("PB: Switch to CW");
    new_mode = CW;
    release = TRUE;
  }
	
  if (release == FALSE) {
    Serial.println("PB: Shutdown!");
    digitalWrite(MAIN_LED_GREEN, LOW);
    sleep(0.5);
    digitalWrite(MAIN_LED_GREEN, HIGH);
    sleep(0.5);
    digitalWrite(MAIN_LED_GREEN, LOW);
    sleep(0.5);
    digitalWrite(MAIN_LED_GREEN, HIGH);
    sleep(0.5);	  
    digitalWrite(MAIN_LED_GREEN, LOW);
    sleep(0.5);
    digitalWrite(MAIN_LED_GREEN, HIGH);
    sleep(0.5);	 
 	  
  }
  if (new_mode != mode)
    transmit_off();
  sleep(2.0);	

   digitalWrite(LED_BUILTIN, LOW);	// make sure built-in LED is off	
}

void process_bootsel() {

//  Serial.println("BOOTSEL pressed!");  
	
  int release = FALSE;
	
//  if (!wifi) 
    digitalWrite(LED_BUILTIN, HIGH);  // make sure built in LED is on before blinking	
	
  sleep(1.0);
	
//  int pb_value = digitalRead(MAIN_PB_PIN);
  if (!BOOTSEL) {
    Serial.println("BOOTSEL: Reboot!");
    release = TRUE;
    prompt = PROMPT_REBOOT;    
  }
	
  blinkTimes(1);
  sleep(1.5);
	
//  pb_value = digitalRead(MAIN_PB_PIN);
  if ((!BOOTSEL) && (release == FALSE)) {
    Serial.println("BOOTSEL: Switch to AFSK");
    release = TRUE;
    new_mode = AFSK;
//    setup();	  
  }
	
  if (release == FALSE) {
    blinkTimes(2);
    sleep(1.5);
  }
	  
//  pb_value = digitalRead(MAIN_PB_PIN);
  if ((!BOOTSEL) && (release == FALSE)) {
    Serial.println("BOOTSEL: Switch to FSK");
    release = TRUE;
    new_mode = FSK;
  }
	
  if (release == FALSE) {
    blinkTimes(3);
    sleep(1.5);
  }
	  
//  pb_value = digitalRead(MAIN_PB_PIN);
  if ((!BOOTSEL) && (release == FALSE)) {
    Serial.println("BOOTSEL: Switch to BPSK");
    release = TRUE;
    new_mode = BPSK;	  
  }
	
  if (release == FALSE) {
    blinkTimes(4);
    sleep(1.5);
  }	
	  
//  pb_value = digitalRead(MAIN_PB_PIN);
  if ((!BOOTSEL) && (release == FALSE)) {
    Serial.println("BOOTSEL: Switch to SSTV");
    new_mode = SSTV;
    release = TRUE;
  }
	
  if (release == FALSE) {
    blinkTimes(5);
    sleep(1.5);
  }	
	  
//  pb_value = digitalRead(MAIN_PB_PIN);
  if ((!BOOTSEL) && (release == FALSE)) {
    Serial.println("BOOTSEL: Switch to CW");
    new_mode = CW;
    release = TRUE;
  }
	
  if (release == FALSE) {
    Serial.println("BOOTSEL: Shutdown!");
    digitalWrite(MAIN_LED_GREEN, LOW);
    sleep(0.5);
    digitalWrite(MAIN_LED_GREEN, HIGH);
    sleep(0.5);
    digitalWrite(MAIN_LED_GREEN, LOW);
    sleep(0.5);
    digitalWrite(MAIN_LED_GREEN, HIGH);
    sleep(0.5);	  
    digitalWrite(MAIN_LED_GREEN, LOW);
    sleep(0.5);
    digitalWrite(MAIN_LED_GREEN, HIGH);
    sleep(0.5);	  
   }
  if (new_mode != mode)
    transmit_off();
//  sleep(2.0);	
	
   digitalWrite(LED_BUILTIN, LOW);	// make sure built-in LED is off	
}

void blinkTimes(int blinks) {
  for (int i = 0; i < blinks; i++) {
    digitalWrite(MAIN_LED_GREEN, LOW);
//    if (!wifi) 
      digitalWrite(LED_BUILTIN, LOW);
    sleep(0.1);
    digitalWrite(MAIN_LED_GREEN, HIGH);
//    if (!wifi) 
       digitalWrite(LED_BUILTIN, HIGH);
    sleep(0.1);
  }
}

void blink_pin(int pin, int duration) {
	
  digitalWrite(pin, HIGH);
  sleep((float)duration / 1000.00);
  digitalWrite(pin, LOW);	
	
}

void config_gpio() {
	
  // set all Pico GPIO pins to input	
  for (int i = 6; i < 29; i++) {
    pinMode(i, INPUT);	  
  }
  // set audio out to TXC board
  pinMode(AUDIO_OUT_PIN, OUTPUT);	

  // set LEDs and blink once	
//  if (!wifi) 
    pinMode(LED_BUILTIN, OUTPUT);  // Set LED pin to output
  pinMode(MAIN_LED_GREEN, OUTPUT);  // Set Main Green LED pin to output
  blink_pin(MAIN_LED_GREEN, 150);
  digitalWrite(MAIN_LED_GREEN, HIGH); // Leave Green LED on	
  pinMode(MAIN_LED_BLUE, OUTPUT);  // Set Main Blue LED pin to output
  blink_pin(MAIN_LED_BLUE, 150);	
  pinMode(STEM_LED_GREEN, OUTPUT);  // Set STEM Green LED pin to output
  blink_pin(STEM_LED_GREEN, 150);	
  pinMode(STEM_LED_BLUE, OUTPUT);  // Set STEM Blue LED pin to output
  blink_pin(STEM_LED_BLUE, 150);

  // set input pins and read	
  pinMode(MAIN_PB_PIN, INPUT_PULLUP);  // Read Main Board push button	
  pinMode(TXC_PIN, INPUT_PULLUP);  // Read TXC to see if present	
  pinMode(BPF_PIN, INPUT_PULLUP);  // Read LPF to see if present
//  pinMode(SQUELCH, INPUT);	// Squelch from TXC

  if (digitalRead(BPF_PIN) == FALSE)
    Serial.println("BPF present");
  else
    Serial.println("BPF not present");	  
	
  if (digitalRead(TXC_PIN) == FALSE)
    Serial.println("TXC present");
  else
    Serial.println("TXC not present");	

//  Serial.print("Squelch: ");
//  Serial.println(digitalRead(SQUELCH));
	

//  Serial.print("Pi 3.3V: ");
//  Serial.println(digitalRead(PI_3V3_PIN));

  // set anlog inputs and read	
  pinMode(TEMPERATURE_PIN, INPUT);
  Serial.print("Diode voltage (temperature): ");
  Serial.println(analogRead(TEMPERATURE_PIN));	

  pinMode(AUDIO_IN_PIN, INPUT);	
  Serial.print("Audio In: ");
  Serial.println(analogRead(AUDIO_IN_PIN));
  	
  pinMode(PTT_PIN, OUTPUT);  // PTT active LOW
  digitalWrite(PTT_PIN, HIGH);

  pinMode(PD_PIN, OUTPUT);  // PD active HIGH
  digitalWrite(PD_PIN, LOW);  	
}

bool TimerHandler0(struct repeating_timer *t) {

//  digitalWrite(STEM_LED_GREEN, !green_led_counter++);
  timer_counter = (timer_counter++) % 5;
  if ((mode == BPSK) || ((mode == FSK) && (timer_counter == 0)))  {	  // only do this if BPSK mode or every 6 times in FSK mode
//    Serial.print("l1 ");
//    Serial.print(wav_position);
//    Serial.print(" ");
    while ((micros() - micro_timer2) < delay_time)	{ } 
//    if (mode == BPSK)	  
//      busy_wait_at_least_cycles(51);	// 300 ns  
//  if ((micros() - micro_timer2) > 834)	  
//    Serial.println(micros() - micro_timer2);	  
    micro_timer2 = micros();	  	
    if (buffer[wav_position++] > 0) {	  
//      digitalWrite(BPSK_CONTROL_B, LOW);  
      digitalWrite(BPSK_CONTROL_B, LOW);
//      delayMicroseconds(10);    	  
      digitalWrite(BPSK_CONTROL_A, HIGH);  
//      Serial.print("-");	    
//      clockgen.enableOutputOnly(1);	  
    } else {
//      digitalWrite(BPSK_CONTROL_A, LOW);  
      digitalWrite(BPSK_CONTROL_A, LOW);  
//      delayMicroseconds(10);    	  
      digitalWrite(BPSK_CONTROL_B, HIGH);	
//      Serial.print("_");	 
//      clockgen.enableOutputOnly(0);	  
    }	
    if (wav_position > bufLen) { // 300) {
	wav_position = wav_position % bufLen;
//	Serial.print("\nR");
//	Serial.print(" ");
//	Serial.println(millis());
/**/
//  if ((micros() - micro_timer)/bufLen > (delay_time + 10))  {	  
/*	    
    if (bufLen != 0) {		  
      Serial.print("R Microseconds: ");
      Serial.println((float)(micros() - micro_timer)/(float)bufLen);
    }
//  }	
*/	    
    if (bufLen != 0) {
      float avg_time = (float)(micros() - micro_timer)/(float)bufLen;
      if ((avg_time > (delay_time * 1.15)) || debug_mode) {	    
        Serial.print("R Microseconds: ");
        Serial.println(avg_time);
      }
    }	    
    micro_timer = micros();
/**/	  
  } else {  
//      Serial.print("R' Microseconds: ");
//      Serial.println(micros() - micro_timer2);
//      while ((micros() - micro_timer2) < 832)	{ }  
//      micro_timer2 = micros();	  
  }
}
/*	
  if (digitalRead(MAIN_PB_PIN) == PRESSED) // pushbutton is pressed
      process_pushbutton();
  if (BOOTSEL)	  // boot selector button is pressed on Pico
      process_bootsel();
*/

  return true;	
}


void start_isr() {
	
//	return;
  if (!timer0_on) {	
//  if (true) {	                         // always start ISR handler
	Serial.println("Starting ISR for BPSK/FSK");
	  
	timer_counter = 0;
	pinMode(BPSK_CONTROL_A, OUTPUT);
	pinMode(BPSK_CONTROL_B, OUTPUT);
        digitalWrite(BPSK_CONTROL_A, LOW); 	 // start with off 
        digitalWrite(BPSK_CONTROL_B, LOW); 	  

   if (ITimer0.attachInterruptInterval(833 - 32, TimerHandler0))	
    {
      if (debug_mode) 	    
        Serial.print(F("Starting ITimer0 OK, micros() = ")); Serial.println(micros());
      timer0_on = true;	  
    }
    else
    Serial.println(F("Can't set ITimer0. Select another Timer, freq. or timer"));
	    
	  
  } else {
//     ITimer0.restartTimer();
//     Serial.println("Restarting ITimer0 for BPSK");	  
	Serial.println("Don't start ITimer0 for BPSK and FSK");	
  }
}
  
void start_button_isr() {

  Serial.println("Starting pushbutton ISR");
	
  if (ITimer1.attachInterruptInterval(10000, TimerHandler1))
  {
    if (debug_mode)	  
      Serial.print(F("Starting ITimer1 OK, micros() = ")); 
    Serial.println(micros());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another Timer, freq. or timer"));

}

/*
void client_print_string(char *string)
{
  int count = 0;
  while ((count < 250) && (string[count] != 0))
  {
    client.print(string[count++]);    
  }
  client.println(" ");  
}

bool check_for_wifi() {
	
#ifndef PICO_W

  return(false);  // skip check if not Pico W board or compilation will fail
	
#endif
	
//     stdio_init_all();

   adc_init();
   adc_gpio_init(29);
   adc_select_input(3);
   const float conversion_factor = 3.3f / (1 << 12);
   uint16_t result = adc_read();
//   Serial.printf("ADC3 value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);

  if (result < 0x100) {
    Serial.println("\nPico W detected!\n");
    return(true);
  }
  else {
     Serial.println("\nPico detected!\n");
     return(false);  
  }
}


void check_for_browser() {
  if (!wifi)
    return;
     
  client = server.available();
  if (!client) {
    return;
  }
  client.printf("CubeSatSim Pico configuration\r\n\r\n");
  while (!client.available()) {
    delay(10);
  }
  String req = client.readStringUntil('\n');
//  Serial.print("Got: ");
//  Serial.println(req);
  
  char * token;
  const char question[2] = "?";
  const char equal[2] = "=";
  const char space[2] = " ";
  char reqstr[255];
  char str2[255];
  char str3[255];
  char var[255];
  char val[255];
  strcpy(val, NULL);
  strcpy(reqstr, req.c_str());
  token = strtok(reqstr, space);
  if (token != NULL) {
//     print_string(token);
     token = strtok(NULL, space);
     if (token != NULL) {
//       print_string(token);
       strcpy(str2, token);    
       token = strtok(str2, question);
       if (token != NULL) {
//         print_string(token); 
         token = strtok(NULL, question);
         if (token != NULL) {
//           print_string(token);    
           strcpy(str3, token);    
           token = strtok(str3, equal);
           if (token != NULL) {
//             print_string(token);
             strcpy(var, token);   
             token = strtok(NULL, equal);
             if (token != NULL) {
//               print_string(token);
               strcpy(val, token);     
             }
           }
         }
       }
     }
  }

  Serial.println(" ");
  print_string(var);
  print_string(val);
  Serial.println(" "); 
  Serial.println(strlen(val));

  if (!strcmp(var, "call") && (strlen(val) > 0)) {
     Serial.print("Changing callsign to ");
     print_string(val);
     client.print("Changing callsign to ");
     client_print_string(val);
  } else if (!strcmp(var, "mode") && (strlen(val) > 0)) {
     Serial.print("Changing mode to ");
     Serial.println(atoi(val));
     client.print("Changing mode to ");
     client.println(atoi(val));
  }
//    client.println("To change the callsign, add call=W3ZM to the URL");
  client.printf("\nTo change the callsign to CALL, type %s:%d/set?call=CALL as the URL", WiFi.localIP().toString().c_str(), port);
  client.printf("\nTo change the mode to N, type %s:%d/set?mode=N as the URL\n", WiFi.localIP().toString().c_str(), port);

  client.flush();
}

void configure_wifi() {
  if (wifi) {
    WiFi.mode(WIFI_AP);
    WiFi.setHostname("PicoW2");
    Serial.printf("Setting up WiFi '%s' with password '%s'\n", ssid, password);
//    Serial.printf("\nConnected to WiFi. Connect to server at %s:%d\n", WiFi.localIP().toString().c_str(), port);
    const char no_address[] = "(IP unset)";
//    if (!strcmp(WiFi.localIP().toString().c_str(), no_address))
//       Serial.println("No WiFi");
    WiFi.begin(ssid, password);
    Serial.println("Starting WiFi");
  //  while (WiFi.status() != WL_CONNECTED) {
    while(!strcmp(WiFi.localIP().toString().c_str(), no_address)) {  
  //    if (!strcmp(WiFi.localIP().toString().c_str(), no_address))
       Serial.print("No WiFi ");
       Serial.println(WiFi.localIP().toString());
       delay(500);
    }
    delay(5000);
    Serial.printf("\nTo configure, connect to server on WiFi at http://%s:%d\n", WiFi.localIP().toString().c_str(), port);
  
    server.begin();
  }
}
*/
	
void transmit_cw(int freq, float duration) {  // freq in Hz, duration in milliseconds
 // if (!wifi) 
    digitalWrite(LED_BUILTIN, HIGH);	// Transmit LED on
  digitalWrite(MAIN_LED_BLUE, HIGH);	
	
  unsigned long start = micros();
  unsigned long duration_us = duration * 1000;
  float period_us = (0.5E6) / (float)(freq);
  bool phase = HIGH;	
  while((micros() - start) < duration_us)  {
    digitalWrite(AUDIO_OUT_PIN, phase);    // ToDo: if no TXC, just turn on PWM carrier
    phase = !phase;	 
    float time_left = (float)(start + duration_us - micros());	  
    sleep(min(time_left, period_us) / 1.0E6);  
  }
  digitalWrite(AUDIO_OUT_PIN, LOW);	
//  if (!wifi) 
    digitalWrite(LED_BUILTIN, LOW);	// Transmit LED off
  digitalWrite(MAIN_LED_BLUE, LOW);	
}

void transmit_callsign(char *callsign) {
		
  char de[] = " HI HI DE ";	
  char id[20];
  strcpy(id, de);
  strcat(id, callsign);
  Serial.print("Transmitting id: ");	
  print_string(id);	
/*	
  if (reset_count == 0) {
    program_radio();	  
  }
*/	
///  transmit_off();	
  transmit_string(id);	
//  transmit_on();	
}

void transmit_string(char *string) {
  int j = 0;
  if (debug_mode)	
    Serial.println("Transmit on");
  digitalWrite(PD_PIN, HIGH);  // Enable SR_FRS 
  digitalWrite(MAIN_LED_BLUE, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);	
  digitalWrite(PTT_PIN, LOW);	
	
  while ((string[j] != '\0') && (j < 256) && !cw_stop) {
//    Serial.print("j = ");
//    Serial.println(j);
    if (string[j] != ' ')	  
      transmit_char(string[j++]);
    else {
//      Serial.println("space between words");
      sleep((6.0 * (float)morse_timing)/1000.0);
      j++;
      if (Serial.available() || BOOTSEL || !digitalRead(10))  // check for button press of serial input
        cw_stop = true;	  
/*	    
      if (prompt) {
//    Serial.println("Need to prompt for input!");
        prompt_for_input();	  
        prompt = false;	  
      }	 
*/	    
    }
  }
  cw_stop = false;
	
  if (debug_mode)	
    Serial.println("Transmit off");
  digitalWrite(MAIN_LED_BLUE, LOW);
  digitalWrite(LED_BUILTIN, LOW);	
  digitalWrite(PTT_PIN, HIGH);	
  digitalWrite(PD_PIN, LOW);  // disable SR_FRS 
}

void transmit_char(char character) {	
  int i = 0;
  while (morse_table[(toupper(character) - '0') % 44][i] != 0) {
//    Serial.print("i = ");
//    Serial.println(i);
//    Serial.print(morse_table[(toupper(character) - '0') % 44][i]);	  
    transmit_cw(morse_freq, morse_table[(toupper(character) - '0') % 44][i++] * morse_timing);	  
    sleep((float)(morse_timing)/1000.0);
  }
  sleep((float)(morse_timing * 3.0)/1000.0);
//  Serial.println("space between characters");

}

void parse_payload() {

	if ((payload_str[0] == 'O') && (payload_str[1] == 'K')) // only process if valid payload response
	{
	  int count1;
	  char * token;

	  const char space[2] = " ";
	  token = strtok(payload_str, space);
	  for (count1 = 0; count1 < 17; count1++) {
	    if (token != NULL) {
	      sensor[count1] = (float) atof(token);
//	      Serial.print("sensor: ");
//	      Serial.println(sensor[count1]);
	      token = strtok(NULL, space);
	    }
	  }
//	  printf("\n");
//	}
//	else
//		payload = OFF;  // turn off since STEM Payload is not responding
//	}
//	if ((payload_str[0] == 'O') && (payload_str[1] == 'K')) {
	  for (int count1 = 0; count1 < 17; count1++) {
	    if (sensor[count1] < sensor_min[count1])
	      sensor_min[count1] = sensor[count1];
	    if (sensor[count1] > sensor_max[count1])
	      sensor_max[count1] = sensor[count1];
	    //  printf("Smin %f Smax %f \n", sensor_min[count1], sensor_max[count1]);
  	  }
	}		
}

void show_dir() {
  LittleFS.begin();
  Dir dir = LittleFS.openDir("/");
// or Dir dir = LittleFS.openDir("/data");
  Serial.println("FS directory:");
  while (dir.next()) {
    Serial.print(dir.fileName());
    if(dir.fileSize()) {
        File f = dir.openFile("r");
        Serial.print(" ");
        Serial.println(f.size());
    }
  }
  Serial.println(">");
}

void load_sstv_image_1_as_cam_dot_jpg() {
    LittleFS.begin();
    File f;	
    Serial.println("Loading image sstv_image_1_320_x_240.jpg into FS");
    f = LittleFS.open("cam.jpg", "w+");
    if (f.write(sstv_image_1_320_x_240, sizeof(sstv_image_1_320_x_240)) < sizeof(sstv_image_1_320_x_240)) {
       Serial.println("Loading image failed. Is Flash Size (FS) set to 1MB?");	     
       delay(2000);
    }
    f.close();	
}

void load_sstv_image_2_as_cam_dot_jpg() {	
    LittleFS.begin();
    File f;
    Serial.println("Loading image sstv_image_2_320_x_240.jpg into FS");
    f = LittleFS.open("cam.jpg", "w+");
    if (f.write(sstv_image_2_320_x_240, sizeof(sstv_image_2_320_x_240)) < sizeof(sstv_image_2_320_x_240)) {
       Serial.println("Loading image failed. Is Flash Size (FS) set to 1MB?");	     
       delay(2000);
    }
    f.close();	
}

void load_files() {
  LittleFS.begin();
  File f;
	
  f = LittleFS.open("sstv_image_1_320_x_240.jpg", "r");
//  if (f) {
  if (false) {	  
    Serial.println("Image sstv_image_1_320_x_240.jpg already in FS");
    f.close();
  } else {
    Serial.println("Loading image sstv_image_1_320_x_240.jpg into FS");
    f = LittleFS.open("sstv_image_1_320_x_240.jpg", "w+");
    if (f.write(sstv_image_1_320_x_240, sizeof(sstv_image_1_320_x_240)) < sizeof(sstv_image_1_320_x_240)) {
       Serial.println("Loading image failed. Is Flash Size (FS) set to 1MB?");	     
       delay(2000);
    }
    f.close();
  }
/*
  f = LittleFS.open("sstv_image_2_320_x_240.jpg", "r");
  if (f) {	
    Serial.println("Image sstv_image_2_320_x_240.jpg already in FS");
    f.close();
  } else {
    Serial.println("Loading image sstv_image_2_320_x_240.jpg into FS");
    f = LittleFS.open("sstv_image_2_320_x_240.jpg", "w+");
    if (f.write(sstv_image_2_320_x_240, sizeof(sstv_image_2_320_x_240)) < sizeof(sstv_image_2_320_x_240)) {
       Serial.println("Loading image failed. Is Flash Size (FS) set to 1MB?");
       delay(2000);
    }
    f.close();
  }
*/	
  show_dir();
}

void serial_input() {
	
  if (prompt == false) {  // only query if not in the middle of prompting
 	  
  if (Serial.available() > 0) {  // check for user input on serial port
 
//    blink(50);
    char result = Serial.read();

    if ((result != '\n') && (result != '\r')) {
	    
    Serial.println(result);

    switch(result) {
     case 'h':
     case 'H':
 //      Serial.println("Help");	     
        prompt = PROMPT_HELP;
 /*		    
       Serial.println("\nChange settings by typing the letter:");	     
       Serial.println("h  Help info");	  
       Serial.println("a  AFSK/APRS mode");	     
       Serial.println("c  CW mode");	     
       Serial.println("f  FSK/DUV mode");	     
       Serial.println("b  BPSK mode");	     
       Serial.println("s  SSTV mode");	     
       Serial.println("i  Restart");	     
       Serial.println("c  CALLSIGN");	     
       Serial.println("t  Simulated Telemetry");	     
       Serial.println("r  Resets Count, or payload & EEPROM");	
       Serial.println("l  Lat and Long");	     
       Serial.println("?  Query sensors\n");	 
*/		    
       break;
		   
     case 'a':
     case 'A':
       Serial.println("Change to AFSK/APRS mode");	     
       new_mode = AFSK;
       break;	
		   
     case 'm':
     case 'M':
       Serial.println("Change to CW mode");	     
       new_mode = CW;
       break;	
		   
     case 'F':
       Serial.println("Formatting flash memory");	     
       prompt = PROMPT_FORMAT;
	break;
     case 'f':
      Serial.println("Change to FSK mode");
       new_mode = FSK;	    
       break;	
		   
     case 'b':
     case 'B':
       Serial.println("Change to BPSK mode");	     
       new_mode = BPSK;
       break;	
		   
     case 's':
     case 'S':
       Serial.println("Change to SSTV mode");	     
       new_mode = SSTV;
       break;	
		   
     case 'i':
     case 'I':
       Serial.println("Restart CubeSatsim software");	     
       prompt = PROMPT_RESTART;
       break;	
		   
     case 'c':
     case 'C':
       Serial.println("Change the CALLSIGN");	
       prompt = PROMPT_CALLSIGN;	    
       break;	
		   
     case 't':
     case 'T':
       Serial.println("Change the Simulated Telemetry");	     
       prompt = PROMPT_SIM;
       break;	
		   
     case 'p':
     case 'P':
       Serial.println("Reset payload EEPROM settings");	
       prompt = PROMPT_PAYLOAD;
       break;	
		   
     case 'r':
     case 'R':
       Serial.println("Change the Resets Count");	
       prompt = PROMPT_RESET;
       break;	
		   
     case 'o':
     case 'O':
       Serial.println("Read diode temperature");	
       prompt = PROMPT_TEMP;
       break;	
		    
     case 'l':
     case 'L':
      Serial.println("Change the Latitude and Longitude");	     
      prompt = PROMPT_LAT;
      break;	
		   
     case 'v':
     case 'V':
      Serial.println("Read INA219 voltage and current");	     
      prompt = PROMPT_VOLTAGE;
      break;	
		    
     case '?':
       Serial.println("Query payload sensors");	     
       prompt = PROMPT_QUERY;
       break;	
		    
     case 'd':
       Serial.println("Change debug mode");	     
       prompt = PROMPT_DEBUG;
       break;			   

     default:
       Serial.println("Not a command\n");	
		   
       break;
    }
    if ((mode == SSTV) && prompt)  // stop SSTV if need to prompt for input
      sstv_end();	    
	    
    if (new_mode != mode)
      transmit_off();
//    sleep(2.0);		    
   }
 }
 }
}

void prompt_for_input() {
 float float_result;
	
  while (Serial.available() > 0)  // clear any characters in serial input buffer
    Serial.read();	  
	
  switch(prompt) {
		  
    case PROMPT_HELP:		  
       Serial.println("\nChange settings by typing the letter:");	     
       Serial.println("h  Help info");	  
       Serial.println("a  AFSK/APRS mode");	     
       Serial.println("m  CW mode");	     
       Serial.println("f  FSK/DUV mode");	     
       Serial.println("b  BPSK mode");	     
       Serial.println("s  SSTV mode");	     
       Serial.println("i  Restart");	     
       Serial.println("c  CALLSIGN");	     
       Serial.println("t  Simulated Telemetry");	     
       Serial.println("r  Resets Count");	
       Serial.println("p  Resets payload and stored EEPROM values");	
       Serial.println("l  Lat and Lon");	     
       Serial.println("?  Query sensors");	
       Serial.println("v  Read INA219 voltage and current");	
       Serial.println("o  Read diode temperature");	
       Serial.println("d  Change debug mode\n");
		  
       Serial.printf("Config file /sim.cfg contains %s %d %f %f %s\n\n", callsign, reset_count, lat_file, long_file, sim_yes);
		  
       break;	
		  
    case PROMPT_CALLSIGN:
      Serial.println("Editing the CALLSIGN in the onfiguration file for CubeSatSim");	
      Serial.println("Return keeps current value.");
      Serial.print("\nCurrent callsign is ");
      Serial.println(callsign);
		  
      Serial.print("Enter callsign in all capitals: ");
      get_serial_string();
		  
      print_string(serial_string);
		  
      if (strlen(serial_string) > 0)	{	  
        strcpy(callsign, serial_string);
	if (mode == AFSK) {
	  char destination[] = "APCSS";	
	  set_callsign(callsign, destination);	
	}
        Serial.println("Callsign updated!");
	write_config_file();      
      } else
        Serial.println("Callsign not updated!");	      

      break;		  		  
		  
    case PROMPT_SIM:
      if (sim_mode == TRUE)
	Serial.println("Simulted Telemetry mode is currently on");      
      else
	Serial.println("Simulted Telemetry mode is currently off");  
      Serial.println("Do you want Simulated Telemetry on (y/n)");
      get_serial_char();
      if ((serial_string[0] == 'y') || (serial_string[0] == 'Y'))	{  
        Serial.println("Setting Simulated telemetry to on");
	config_simulated_telem();     
	write_config_file();    	      
      } else if ((serial_string[0] == 'n') || (serial_string[0] == 'N')) {	      
        Serial.println("Setting Simulated telemetry to off");
	sim_mode = false;      
        if (!ina219_started)
	  start_ina219(); 
	write_config_file();    	      
      } else      
        Serial.println("No change to Simulated Telemetry mode");
      break;	
		  
    case PROMPT_LAT:

      Serial.println("Changing the  latitude and longitude  - only used for APRS telemetry");
      Serial.println("Hitting return keeps the current value.");
		  
      Serial.print("Current value of latitude is ");
      Serial.println(latitude);		  
      Serial.println("Enter latitude  (decimal degrees, positive is north): ");	  
      get_serial_string();		  
      float_result = atof(serial_string);
      if (float_result != 0.0)  {
        Serial.print("Latitude updated to ");	    
        Serial.println(float_result);		  
        latitude = float_result;
      } else
        Serial.println("Latitude not updated");	

      get_serial_clear_buffer();		  
      Serial.print("Current value of longitude is ");
      Serial.println(longitude);		  
      Serial.println("Enter longitude  (decimal degrees, positive is east): ");	  
      get_serial_string();		  
      float_result = atof(serial_string);
      if (float_result != 0.0)  {
        Serial.print("Longitude updated to ");	    
        Serial.println(float_result);		  
        longitude = float_result;
      } else
        Serial.println("Longitude not updated");
		  
      write_config_file();     	  
      if (mode == AFSK)
	set_lat_lon();
		  
      break;	
		  
    case PROMPT_QUERY:
      Serial.println("Querying payload sensors");		  		  
      payload_command = PAYLOAD_QUERY;		  
      break;
		  
    case PROMPT_TEMP:		  
      sensorValue = analogRead(TEMPERATURE_PIN);
      Serial.print("Raw diode voltage: ");		  
      Serial.println(sensorValue);  
      Temp = T1 + (sensorValue - R1) *((T2 - T1)/(R2 - R1));
      Serial.print("Calculated temperature: ");		  
      Serial.print(Temp);  
      Serial.println(" C");		  	  
      break;
		    
    case PROMPT_VOLTAGE:
      Serial.println("Querying INA219 voltage and current sensors");
      if (!ina219_started)
	start_ina219();
      voltage_read = true;		  
      read_ina219();		  	  
      break;	

    case PROMPT_REBOOT:
       Serial.println("Rebooting...");	 
       watchdog_reboot (0, SRAM_END, 10);	 // restart Pico
       sleep(20.0);			  
       break;
		  
    case PROMPT_FORMAT:
       LittleFS.format();
//       Serial.println("Reboot or power cycle to restart the CubeSatSim");
 //      while (1) ;	    // infinite loop
       Serial.println("Rebooting...");	 
       watchdog_reboot (0, SRAM_END, 10);	 // restart Pico
       sleep(20.0);			  
       break;
		  
    case PROMPT_PAYLOAD:	      
      Serial.println("Resetting the Payload");
      payload_command = PAYLOAD_RESET;
      start_payload();      		  
      break;			  
		  
    case PROMPT_RESET:
      Serial.println("Reset count is now 0");	
      reset_count = 0;
      write_config_file();    	  
      break;	
		  
    case PROMPT_RESTART:
      prompt = false;
//    Serial.println("Restart not yet implemented");
      start_payload();
//      start_ina219();
      if (mode != CW)
        transmit_callsign(callsign);
      sleep(0.5);	 
      config_telem();
      config_radio();
      sampleTime = (unsigned int) millis();	 		  
      break;	  
		  
    case PROMPT_DEBUG:
      Serial.print("Changing Debug Mode to ");
      debug_mode = !debug_mode;
      if (debug_mode)  
        Serial.println("on");	
      else  
        Serial.println("off");
      break;	
  }
  prompt = false;	
}

void get_serial_string() {
  int input = 0;	
  int i = 0;
  unsigned int elapsed_time = (unsigned int) millis();	
  while ((input != '\n') && (input!= '\r') && (i < 128) && ((millis() - elapsed_time) < 20000)) {
    if (Serial.available() > 0) {
      input = Serial.read();
      if ((input != '\n') && (input!= '\r')) {
	serial_string[i++] = input;
        Serial.write(input);
      }
    } 
    sleep(0.1);	  
  }
  serial_string[i] = 0;	
  Serial.println(" ");	
}

void get_serial_char() {	
  unsigned int elapsed_time = (unsigned int) millis();	
  while (((millis() - elapsed_time) < 20000) && (Serial.available() < 1)) { }
  if (Serial.available() > 0) {	 
    serial_string[0] = Serial.read();  // get character
    Serial.write(serial_string[0]);
    serial_string[1] = 0;	
    Serial.println(" ");
  } else
  {
    serial_string[0] = 0;	// timeout - no character	 
  }
}

void get_serial_clear_buffer() {
//  Serial.println("Emptying serial input buffer");	
  while (Serial.available() > 0)
   Serial.read();
	
}

void set_lat_lon() {
// Serial.println("Setting lat and lon for APRS");	
  char lat_string[64];
  char lon_string[64];
	  
  char sym_ovl_default = '\\'; //'H';
  char sym_tab_default = 'S'; // 'a';
  char icon[] = "\\S"; //Ha";
	  
//      latitude = toAprsFormat(latitude);
//      longitude = toAprsFormat(longitude);   
         //	sprintf(header_str2b, "=%7.2f%c%c%c%08.2f%cShi hi ",4003.79,'N',0x5c,0x5c,07534.33,'W');  // add APRS lat and long
  if (latitude > 0)
    sprintf(lat_string, "%07.2f%c", toAprsFormat(latitude), 'N'); // lat
  else
    sprintf(lat_string, "%07.2f%c", toAprsFormat(latitude) * (-1.0), 'S'); // lat
  if (longitude > 0)
    sprintf(lon_string, "%08.2f%c", toAprsFormat(longitude), 'E'); // long
  else
    sprintf(lon_string, "%08.2f%c", toAprsFormat(longitude) * (-1.0), 'W'); // long	 
	  
// print_string(lat_string);
// print_string(lon_string);
	  
  set_lat_lon_icon(lat_string, lon_string, icon);	

}

void program_radio() {
	
  Serial.println("Programming SR_FRS!");	
		
  digitalWrite(PD_PIN, HIGH);  // enable SR_FRS
  digitalWrite(PTT_PIN, HIGH);  // stop transmit	
	
  DumbTXSWS mySerial(SWTX_PIN); // TX pin
  mySerial.begin(9600);
    
  for (int i = 0; i < 5; i++) {
     sleep(0.5); // delay(500);
//  Serial1.println("AT+DMOSETGROUP=0,434.9100,434.9100,1,2,1,1\r");
//    mySerial.println("AT+DMOSETGROUP=0,434.9000,434.9000,1,2,1,1\r");    
//     mySerial.println("AT+DMOSETGROUP=0,434.9000,434.9000,0,8,0,0\r");  
//     mySerial.println("AT+DMOSETGROUP=0,432.2510,432.2510,0,8,0,0\r");  
//     mySerial.println("AT+DMOSETGROUP=0,432.2500,432.2500,0,8,0,0\r");  
     mySerial.println("AT+DMOSETGROUP=0,434.9000,434.9000,0,8,0,0\r");  
   sleep(0.5);	  
   mySerial.println("AT+DMOSETMIC=8,0\r");  // was 8
	
  }
  digitalWrite(PD_PIN, LOW);  // disable SR_FRS	
}

void read_mode() {
  LittleFS.begin();
  Serial.println("Reading mode");	
  char buff[32];		
  File mode_file = LittleFS.open("/.mode", "r");	
  if (!mode_file) {
    write_mode();	  
  } else {
    if (mode_file.read((uint8_t *)buff, 31)) {
//      Serial.println("Reading mode from .mode file");    
      sscanf(buff, "%d", &mode);
      mode_file.close();
//      Serial.print("Mode is ");
//      Serial.print(mode);
	    
    }
  }		
}

void write_mode() {

  char buff[32];	
  Serial.println("Writing .mode file");	
  File mode_file = LittleFS.open("/.mode", "w+");		  	
	
  sprintf(buff, "%d", mode);
  if (debug_mode) {	
    Serial.println("Writing string");	
    print_string(buff);	
  }
  mode_file.write(buff, strlen(buff));	  
	  
  mode_file.close();
//  Serial.println("Write complete");	
}

void start_clockgen() {

  if (clockgen.begin() != ERROR_NONE)
  {
    /* There was a problem detecting the IC ... check your connections */
    Serial.println("No Si5351 detected ... Check your wiring or I2C ADDR!");
    return;	  
  }

  Serial.println("Starting clockgen frequency 434.9 MHz");
	
//  clockgen.setClockFSK();  // default to FSK
  clockgen.enableOutputs(false);	
	
}
