/*
 *  Transmits CubeSat Telemetry at 434.9MHz in AFSK, FSK, BPSK, or CW format
 *  Or transmits SSTV stored images or Pi camera iamges.
 *
 *  Copyright Alan B. Johnston
 *
 *  Portions Copyright (C) 2018 Jonathan Brandenburg
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "main.h"

//#define HAB  // uncomment to change APRS icon from Satellite to Balloon and only BAT telemetry

int main(int argc, char * argv[]) {
	
  printf("\n\nCubeSatSim v2.2 starting...\n\n");

  wiringPiSetup();	

  strcpy(fail_yes, "no");		
  // Open configuration file with callsign and reset count	
  FILE * config_file = fopen("/home/pi/CubeSatSim/sim.cfg", "r");
  if (config_file == NULL) {
    printf("Creating config file.");
    config_file = fopen("/home/pi/CubeSatSim/sim.cfg", "w");
    fprintf(config_file, "%s %d", " ", 100);
    fclose(config_file);
    config_file = fopen("/home/pi/CubeSatSim/sim.cfg", "r");
  }

//  char * cfg_buf[100];

  fscanf(config_file, "%s %d %f %f %s %d %s %s %s %d %d %s %d", 
	  call, &reset_count, &lat_file, &long_file, sim_yes, &squelch, tx, rx, hab_yes, &rx_pl, &tx_pl, fail_yes, &fail_time);
  fclose(config_file);
  fprintf(stderr,"Config file /home/pi/CubeSatSim/sim.cfg contains %s %d %f %f %s %d %s %s %s %d %d %s %d\n", 
	  call, reset_count, lat_file, long_file, sim_yes, squelch, tx, rx, hab_yes, rx_pl, tx_pl, fail_yes, fail_time);

  fprintf(stderr, "Transmit on %s MHz Receive on %s MHz\n", tx, rx);

  FILE * uptime_file = fopen("/proc/uptime", "r");
  fscanf(uptime_file, "%f", & uptime_sec);
  printf("Uptime sec: %f \n", uptime_sec);	  
  fclose(uptime_file);	

//  program_radio();  // do in transmit instead
  if (uptime_sec < 30.0) {	
  	reset_count = (reset_count + 1) % 0xffff;  // only increment uptime if just rebooted
	fprintf(stderr,"INFO: Reset Count: %d Uptime since Reset: %ld \n", reset_count, uptime_sec);
  }

  if ((fabs(lat_file) > 0) && (fabs(lat_file) < 90.0) && (fabs(long_file) > 0) && (fabs(long_file) < 180.0)) {
    fprintf(stderr, "Valid latitude and longitude in config file\n");
// convert to APRS DDMM.MM format
//    latitude = toAprsFormat(lat_file);
//    longitude = toAprsFormat(long_file);
    latitude = lat_file;
    longitude = long_file;	  
    fprintf(stderr, "Lat/Long %f %f\n", latitude, longitude);		  
    fprintf(stderr, "Lat/Long in APRS DDMM.MM format: %07.2f/%08.2f\n", toAprsFormat(latitude), toAprsFormat(longitude));
    newGpsTime = millis();	  
   	  
  } else { // set default
//    latitude = toAprsFormat(latitude);
//    longitude = toAprsFormat(longitude);
      newGpsTime = millis();  
  }
	
  if (strcmp(sim_yes, "yes") == 0) {
	  sim_mode = TRUE;
	  fprintf(stderr, "Sim mode is turned ON by configuration\n");
	  sim_config = TRUE;
  }
  if (strcmp(hab_yes, "yes") == 0) {
	  hab_mode = TRUE;
	  fprintf(stderr, "HAB mode is ON\n");
  }	
  if (strcmp(fail_yes, "yes") == 0) {
	  fail_rnd_mode = TRUE;
	  fprintf(stderr, "Random fail mode is ON\n");
  }		
	
  FILE * command_file = fopen("/home/pi/CubeSatSim/command_control", "r");
  if (command_file == NULL) {	  	
	    fprintf(stderr,"Command and control is OFF\n");
	    c2cStatus = DISABLED;
  } else {
	    command_file = fopen("/home/pi/CubeSatSim/command_control_direwolf", "r");
	    if (command_file == NULL)  {
		    fprintf(stderr,"Command and control Carrier (squelch) is ON\n");
		    c2cStatus = CARRIER;
	    } else {
		    fprintf(stderr,"Command and control DTMF or APRS is ON\n");
		    c2cStatus = DTMF_APRS;
	    }
  }	
  printf("c2cStatus: %d \n", c2cStatus);
	
  char resbuffer[1000];
//  const char testStr[] = "cat /proc/cpuinfo | grep 'Revision' | awk '{print $3}' | sed 's/^1000//' | grep '9000'";
  const char testStr[] = "cat /proc/cpuinfo | grep 'Revision' | awk '{print $3}' | sed 's/^1000//'";
  const char test2Str[] = "cat /proc/cpuinfo | grep 'Revision' | awk '{print $3}' | sed 's/^1000//' | grep '902120'";	
  FILE *file_test = sopen(testStr);  // see if Pi Zero 2  
  fgets(resbuffer, 1000, file_test);
  fprintf(stderr, "Pi Zero test result: %s\n", resbuffer);
  fclose(file_test);	
  
//  fprintf(stderr, "hex:  %x %x %x %x \n", resbuffer[0], resbuffer[1], resbuffer[2], resbuffer[3]);
  if ((resbuffer[0] != '9') || (resbuffer[1] != '0') || (resbuffer[2] != '0') || (resbuffer[3] != '0')) 
  {
    // voltageThreshold = 3.7;
//    if ((resbuffer[0] != '9') || (resbuffer[1] != '0') || (resbuffer[2] != '2') || (resbuffer[3] != '1'))
    FILE *file2_test = sopen(test2Str);  // see if Pi Zero 2  
    fgets(resbuffer, 1000, file2_test);
    fprintf(stderr, "Pi Zero 2 test result: %s\n", resbuffer);
    fclose(file2_test);	  
    if (strlen(resbuffer) > 5) 	{
      fprintf(stderr, "Pi Zero 2 detected\n");
      FILE * pi_zero2 = popen("touch /home/pi/CubeSatSim/pi_zero2", "r"); // store Pi Zero 2 flag
      pclose(pi_zero2); 
    }
    else
      fprintf(stderr, "Not a Pi Zero or Pi Zero 2\n");
	    
    pi_zero_2_offset = 500;
    if (uptime_sec < 30.0) {
	FILE * transmit_stop = popen("sudo systemctl start transmit", "r");
  	pclose(transmit_stop);   
        fprintf(stderr, "Sleep 5 sec\n");    
	sleep(5);  // try sleep at start to help boot
    }
  }
  else {
    fprintf(stderr,"Pi Zero detected\n");
    FILE * pi_zero2 = popen("sudo rm /home/pi/CubeSatSim/pi_zero2 &>/dev/null", "r");  // remove Pi Zero 2 flag if present
    pclose(pi_zero2); 	  
    if ((c2cStatus == DISABLED) || (c2cStatus == CARRIER))  {
      pi_zero_2_offset = 500;
    }
    if (uptime_sec < 30.0) {
      FILE * transmit_stop = popen("sudo systemctl start transmit", "r");
      pclose(transmit_stop);
      fprintf(stderr,"Sleep 10 sec\n");    
      sleep(10);
    }
  }
	
//  FILE * transmit_stop = popen("sudo systemctl stop transmit", "r");
//  FILE * transmit_stop = popen("sudo systemctl restart transmit", "r");


//  FILE * cc_start = popen("/home/pi/CubeSatSim/command &", "r");
//  pclose(cc_start);	
	
//  FILE * file_deletes = popen("sudo rm /home/pi/CubeSatSim/ready /home/pi/CubeSatSim/cwready > /dev/null", "r");
//  pclose(file_deletes);	
	
  printf("Test bus 1\n");
  fflush(stdout);	
  i2c_bus1 = (test_i2c_bus(1) != -1) ? 1 : OFF;
  printf("Test bus 3\n");	
  fflush(stdout);
  i2c_bus3 = (test_i2c_bus(3) != -1) ? 3 : OFF;
  printf("Finished testing\n");	
  fflush(stdout);

//  sleep(2);

//#ifdef HAB
  if (hab_mode)	
  	fprintf(stderr, "HAB mode enabled - in APRS balloon icon and no battery saver or low voltage shutdown\n");
//#endif
	
//  FILE * transmit_restart = popen("sudo systemctl restart transmit", "r");
//  pclose(transmit_restart);
	
  mode = BPSK;
  frameCnt = 1;

  if (argc > 1) {
    //	  strcpy(src_addr, argv[1]);
    if ( * argv[1] == 'b') {
      mode = BPSK;
      printf("Mode is BPSK\n");
    } else if ( * argv[1] == 'a') {
      mode = AFSK;
      printf("Mode is AFSK\n");
    } else if ( * argv[1] == 'm') {
      mode = CW;
      printf("Mode is CW\n");
    } else if ( * argv[1] == 'j') {
      mode = FC;
      printf("Mode is FUNcube\n");
    } else {
      printf("Mode is BPSK\n");
    }

    if (argc > 2) {
      //		  printf("String is %s %s\n", *argv[2], argv[2]);
      loop = atoi(argv[2]);
      loop_count = loop;
    }
    printf("Looping %d times \n", loop);

    if (argc > 3) {
      if ( * argv[3] == 'n') {
        cw_id = OFF;
        printf("No CW id\n");
      }
    }
  } else {
	  
    FILE * mode_file = fopen("/home/pi/CubeSatSim/.mode", "r");
    if (mode_file != NULL) {	
      char mode_string;	
      mode_string = fgetc(mode_file);
      fclose(mode_file);
      printf("Mode file /home/pi/CubeSatSim/.mode contains %c\n", mode_string);

      if ( mode_string == 'f') {
        mode = FSK;
        printf("Mode is FSK\n");
      } else if ( mode_string == 'a') {
        mode = AFSK;
        printf("Mode is AFSK\n");
      } else if ( mode_string == 's') {
        mode = SSTV;
        printf("Mode is SSTV\n");
     } else if ( mode_string == 'm') {
        mode = CW;
        printf("Mode is CW\n");
     } else if ( mode_string == 'j') {
        mode = FC;
        printf("Mode is FUNcube\n");
     } else if ( mode_string == 'e') {
        mode = REPEATER;
        printf("Mode is Repeater\n");
     } else if ( mode_string == 'n') {
        mode = TXCOMMAND;
        printf("Mode is Transmit Command\n");	      
      } else {
        printf("Mode is BPSK\n");
      }	    
    }
  } 

  // Open telemetry file with STEM Payload Data
  telem_file = fopen("/home/pi/CubeSatSim/telem.txt", "a");
  if (telem_file == NULL) 
    printf("Error opening telem file\n");
  fclose(telem_file);
  printf("Opened telem file\n");
	

  battery_saver_mode = battery_saver_check();
/**/
  if (battery_saver_mode == ON)	{
	SafeMode = 1;
  	fprintf(stderr, "Safe Mode! Battery_saver_mode is ON\n\n");
  }
  else {
	fprintf(stderr, "\nBattery_saver_mode is OFF\n\n");
	SafeMode = 0;
  }
/**/	

  fflush(stderr);
	
  txLed = 2;
  txLedOn = HIGH;
  txLedOff = LOW;
  onLed = 27;
  onLedOn = HIGH;
  onLedOff = LOW;

  pinMode(26, INPUT);
  pullUpDnControl(26, PUD_UP);

  if (digitalRead(26) != HIGH) {
    printf("LPF present\n");
    transmit = TRUE;
  }

  config_file = fopen("sim.cfg", "w");
  fprintf(config_file, "%s %d %8.4f %8.4f %s %d %s %s %s %d %d %s %d", 
	  call, reset_count, lat_file, long_file, sim_yes, squelch, tx, rx, hab_yes, rx_pl, tx_pl, fail_yes, fail_time);
  //    fprintf(config_file, "%s %d", call, reset_count);
  fclose(config_file);
  config_file = fopen("sim.cfg", "r");

  map[MINUS_X] = MINUS_Y;
  map[PLUS_Z] = MINUS_X;	
  map[MINUS_Y] = PLUS_Z;		  

  if (access("/dev/i2c-11", W_OK | R_OK) >= 0) { // Test if I2C Bus 11 is present			
      printf("/dev/i2c-11 is present\n\n");
      snprintf(busStr, 10, "%d %d", test_i2c_bus(1), test_i2c_bus(11));
  } else {
      snprintf(busStr, 10, "%d %d", i2c_bus1, i2c_bus3);
  }
 
	
  // check for camera	
//  char cmdbuffer1[1000];
  FILE * file4 = popen("vcgencmd get_camera", "r");
  fgets(cmdbuffer, 1000, file4);
  char camera_present[] = "supported=1 detected=1";
  // printf("strstr: %s \n", strstr( & cmdbuffer1, camera_present));
  camera = (strstr( (const char *)& cmdbuffer, camera_present) != NULL) ? ON : OFF;
  printf("Camera result:%s camera: %d \n", & cmdbuffer, camera);
  pclose(file4);

  #ifdef DEBUG_LOGGING
  printf("INFO: I2C bus status 0: %d 1: %d 3: %d camera: %d\n", i2c_bus0, i2c_bus1, i2c_bus3, camera);
  #endif
		
  FILE * file5 = popen("sudo rm /home/pi/CubeSatSim/camera_out.jpg > /dev/null 2>&1", "r");
  //file5 = popen("sudo rm /home/pi/CubeSatSim/camera_out.jpg.wav > /dev/null 2>&1", "r");
  pclose(file5);
	
    payload = OFF;
    fprintf(stderr,"Opening serial\n");
    if ((uart_fd = serialOpen("/dev/ttyAMA0", 115200)) >= 0) {  // was 9600
      fprintf(stderr,"Serial opened to Pico\n");	    
//      payload = ON;	
      payload = get_payload_serial(FALSE); 
      fprintf(stderr,"Get_payload_status: %d \n", payload);  // not debug	    
	    
    } else {
      fprintf(stderr, "Unable to open UART: %s\n -> Did you configure /boot/config.txt and /boot/cmdline.txt?\n", strerror(errno));
    }

	sensor_setup();

  if ((i2c_bus3 == OFF) || (sim_mode == TRUE)) {

    sim_mode = TRUE;
	    
    fprintf(stderr, "Simulated telemetry mode!\n");

    srand((unsigned int)time(0));

    axis[X] = rnd_float(-0.2, 0.2);
    if (axis[X] == 0)
      axis[X] = rnd_float(-0.2, 0.2);
    axis[Y] = rnd_float(-0.2, 0.2);
	float axis_z;
	axis_z  = sqrt(1 - axis[X] * axis[X] - axis[Y] * axis[Y]);  
    axis[Z] = (rnd_float(-0.2, 0.2) > 0) ? axis_z : -1.0 * axis_z;

    angle[X] = (float) atan(axis[Y] / axis[Z]);
    angle[Y] = (float) atan(axis[Z] / axis[X]);
    angle[Z] = (float) atan(axis[Y] / axis[X]);

    volts_max[X] = rnd_float(9.0, 12.0) * (float) sin(angle[Y]);
    volts_max[Y] = rnd_float(9.0, 12.0) * (float) cos(angle[X]);
    volts_max[Z] = rnd_float(9.0, 12.0) * (float) cos(angle[Y] - angle[X]);

    float amps_avg = rnd_float(150, 750);

    amps_max[X] = (amps_avg + rnd_float(-25.0, 25.0)) * (float) sin(angle[Y]);
    amps_max[Y] = (amps_avg + rnd_float(-25.0, 25.0)) * (float) cos(angle[X]);
    amps_max[Z] = (amps_avg + rnd_float(-25.0, 25.0)) * (float) cos(angle[Y] - angle[X]);

    batt = rnd_float(3.8, 4.1);
    speed = rnd_float(1.0, 2.5);
    eclipse = (rnd_float(-1, +4) > 0) ? 1.0 : 0.0;
	atmosphere = (rnd_float(-1, +4) > 0) ? 0.0 : 1.0;
	  
//    eclipse = 1;	  
    period = rnd_float(150, 300);
    tempS = rnd_float(20, 55);
    temp_max = rnd_float(50, 70);
    temp_min = rnd_float(10, 20);

//    #ifdef DEBUG_LOGGING
    for (int i = X; i <= Z; i++)
      printf("axis: %f angle: %f v: %f i: %f \n", axis[i], angle[i], volts_max[i], amps_max[i]);
    printf("batt: %f speed: %f eclipse_time: %f eclipse: %f period: %f temp: %f max: %f min: %f\n", batt, speed, eclipse_time, eclipse, period, tempS, temp_max, temp_min);
//    #endif

    time_start = (long int) millis();

    eclipse_time = (long int)(millis() / 1000.0);
    if (eclipse == 0.0)
      eclipse_time -= period / 2; // if starting in eclipse, shorten interval	
  }

  // tx_freq_hz -= tx_channel * 50000;

  if (transmit == FALSE) {

    fprintf(stderr, "\nNo CubeSatSim Low Pass Filter detected.  No transmissions after the CW ID.\n");
    fprintf(stderr, " See http://cubesatsim.org/wiki for info about building a CubeSatSim\n\n");
  }

   if (mode == FSK) {
      bitRate = 200;
      rsFrames = 1;
      payloads = 1;
      rsFrameLen = 64;
      headerLen = 6;
      dataLen = 58;
      syncBits = 10;
      syncWord = 0b0011111010;
      parityLen = 32;
      amplitude = 32767 / 3;
      samples = S_RATE / bitRate;
      bufLen = (frameCnt * (syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))) * samples);

      samplePeriod =  (int) (((float)((syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen)))) / (float) bitRate) * 1000 - 500);
      sleepTime = 0.1f;
	   
      frameTime = ((float)((float)bufLen / (samples * frameCnt * bitRate))) * 1000; // frame time in ms
	      
      printf("\n FSK Mode, %d bits per frame, %d bits per second, %d ms per frame, %d ms sample period\n",
        bufLen / (samples * frameCnt), bitRate, frameTime, samplePeriod);
	   
    } else if (mode == BPSK) {
      bitRate = 1200;
      rsFrames = 3;
      payloads = 6;
      rsFrameLen = 159;
      headerLen = 8;
      dataLen = 78;
      syncBits = 31;
      syncWord = 0b1000111110011010010000101011101;
      parityLen = 32;
      amplitude = 32767;
      samples = S_RATE / bitRate;
      bufLen = (frameCnt * (syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))) * samples);

         samplePeriod = ((float)((syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))))/(float)bitRate) * 1000 - 1800;
      //    samplePeriod = 3000;
      //    sleepTime = 3.0;
      //samplePeriod = 2200; // reduce dut to python and sensor querying delays
      sleepTime = 2.2f;
	   
      frameTime = ((float)((float)bufLen / (samples * frameCnt * bitRate))) * 1000; // frame time in ms

      printf("\n BPSK Mode, bufLen: %d,  %d bits per frame, %d bits per second, %d ms per frame %d ms sample period\n",
        bufLen, bufLen / (samples * frameCnt), bitRate, frameTime, samplePeriod);
	   
      sin_samples = S_RATE/freq_Hz;	 		
//      printf("Sin map: ");	 		
      for (int j = 0; j < sin_samples; j++) {	 		
        sin_map[j] = (short int)(amplitude * sin((float)(2 * M_PI * j / sin_samples)));	 		
//	printf(" %d", sin_map[j]);	 		
      }	 		
      printf("\n");
	   
    } else if (mode == FC) {  // for now copy BPSK settings
      bitRate = 1200;
//      rsFrames = 3;
//      payloads = 6;
//      rsFrameLen = 159;
      headerLen = 768; // 8;
      dataLen = 5200; // 78;
      syncBits = 32;  // 31;
      syncWord = 0x1acffc1d; // 0b1000111110011010010000101011101;
//      parityLen = 32;
      amplitude = 32767;
      samples = S_RATE / bitRate;
 //     bufLen = (frameCnt * (syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))) * samples);
      bufLen = (headerLen + syncBits + dataLen)/8;

 //        samplePeriod = ((float)((syncBits + 10 * (headerLen + rsFrames * (rsFrameLen + parityLen))))/(float)bitRate) * 1000 - 1800;
      samplePeriod = 5000;  
      //    samplePeriod = 3000;
      //    sleepTime = 3.0;
      //samplePeriod = 2200; // reduce dut to python and sensor querying delays
//      sleepTime = 2.2f;
	   
//      frameTime = ((float)((float)bufLen / (samples * frameCnt * bitRate))) * 1000; // frame time in ms
      frameTime = 5000;
	   
      printf("\n FC Mode, bufLen: %d,  %d bits per frame, %d bits per second, %d ms per frame %d ms sample period\n",
      bufLen, bufLen / samples, bitRate, frameTime, samplePeriod);
	   
      sin_samples = S_RATE/freq_Hz;	 		
      for (int j = 0; j < sin_samples; j++) {	 		
        sin_map[j] = (short int)(amplitude * sin((float)(2 * M_PI * j / sin_samples)));	 

      FILE * delete_image = popen("sudo rm /home/pi/CubeSatSim/image_file.bin", "r");  // delete any previous camera images
      pclose(delete_image); 	      
      }	 		
      printf("\n");
   }
	
  memset(voltage, 0, sizeof(voltage));
  memset(current, 0, sizeof(current));
  memset(sensor, 0, sizeof(sensor));
  memset(other, 0, sizeof(other));	
	
  if (((mode == FSK) || (mode == BPSK))) // && !sim_mode)
      get_tlm_fox();	// fill transmit buffer with reset count 0 packets that will be ignored
  else if (((mode == FC))) // && !sim_mode)
      get_tlm_fc();	// fill transmit buffer with reset count 0 packets that will be ignored

  if (firstTime == 0)	
  	firstTime = 1;
	  
//  if (!sim_mode)  // always read sensors, even in sim mode
  {
    strcpy(pythonStr, pythonCmd);
    strcat(pythonStr, busStr);
    strcat(pythonConfigStr, pythonStr);
    strcat(pythonConfigStr, " c");  

    fprintf(stderr, "pythonConfigStr: %s\n", pythonConfigStr);
	
    file1 = sopen(pythonConfigStr);  // python sensor polling function	  

    fgets(cmdbuffer, 1000, file1);
    fprintf(stderr, "pythonStr result: %s\n", cmdbuffer);
  }

  for (int i = 0; i < 9; i++) {
    voltage_min[i] = 1000.0;
    current_min[i] = 1000.0;
    voltage_max[i] = -1000.0;
    current_max[i] = -1000.0;
  }
  for (int i = 0; i < SENSOR_FIELDS; i++) {
    sensor_min[i] = 1000.0;
    sensor_max[i] = -1000.0;
 //   printf("Sensor min and max initialized!");
  }
  for (int i = 0; i < 3; i++) {
    other_min[i] = 1000.0;
    other_max[i] = -1000.0;
  }

  loopTime = millis();	
	
  while (loop-- != 0) {
    fflush(stdout);
    fflush(stderr);
//    frames_sent++;
//	if (!sim_mode)  {
    	sensor_payload[0] = '\0';
	    memset(sensor, 0, sizeof(sensor));
//	}
	  
    memset(voltage, 0, sizeof(voltage));
    memset(current, 0, sizeof(current));
    memset(other, 0, sizeof(other));	
	  
    FILE * uptime_file = fopen("/proc/uptime", "r");
    fscanf(uptime_file, "%f", & uptime_sec);

    uptime = (int) (uptime_sec + 0.5);
//    printf("Uptime sec: %f \n", uptime_sec);	  
//    #ifdef DEBUG_LOGGING
//    printf("INFO: Reset Count: %d Uptime since Reset: %ld \n", reset_count, uptime);
//    #endif
    fclose(uptime_file);

  if (fail_rnd_mode) {
//	if (loop % 10 == 0) { 	
	if ((loopTime - failTime) > fail_time * 1000)	{
//  	  failureMode = (int) rnd_float(1, FAIL_COUNT);
  	  failureMode = (int) rnd_float(1, 9);
	  printf("Sim Mode Random Failure Change\n");
	  FILE * failure_mode_file = fopen("/home/pi/CubeSatSim/failure_mode.txt", "w");
	  fprintf(failure_mode_file, "%d", failureMode);	
	  fclose(failure_mode_file);
	  failTime = loopTime;	
    }
  }
//  else
//  {
//	  failureMode = OFF;
	  FILE * failure_mode_file = fopen("/home/pi/CubeSatSim/failure_mode.txt", "r");
	  if (failure_mode_file != NULL) {	
	    char failure_string[10];	
	    if ( (fgets(failure_string, 10, failure_mode_file)) != NULL)  {
	     failureMode = atoi(failure_string); 
		 fclose(failure_mode_file);	
		 printf("Failure mode: %d\n", failureMode);	
		}
	  } else {
		failureMode = FAIL_NONE;
		printf("No simulated failure.\n");
	  }
//  }	  

   {
      int count1;
      char * token;
      fputc('\n', file1);
      fgets(cmdbuffer, 1000, file1);
//      fprintf(stderr, "Python read Result: %s\n", cmdbuffer);

//      serialPuts(uart_fd, cmdbuffer);   // write INA data to Pico over serial

      const char space[2] = " ";
      token = strtok(cmdbuffer, space);

      for (count1 = 0; count1 < 8; count1++) {
        if (token != NULL) {
          voltage[count1] = (float) atof(token);
          #ifdef DEBUG_LOGGING
//            printf("voltage: %f ", voltage[count1]);
          #endif
          token = strtok(NULL, space);
          if (token != NULL) {
            current[count1] = (float) atof(token);
            if ((current[count1] < 0) && (current[count1] > -0.5))
              current[count1] *= (-1.0f);
            #ifdef DEBUG_LOGGING
//              printf("current: %f\n", current[count1]);
            #endif
            token = strtok(NULL, space);
          }
        }
        if (voltage[map[BAT]] == 0.0)  // No BAT Board
			if (voltage[map[BAT2]] == 0.0) // No BAT2 Board
				batteryVoltage = 4.5;
			else {
				batteryVoltage = voltage[map[BAT2]];  // only BAT2 Board present
				if (sim_mode && !sim_config) {	// if Voltage sensor on Battery board is present, exit simulated telemetry mode
					sim_mode = FALSE; 
					fprintf(stderr, "Turning off sim_mode since battery sensor 2 is present\n");
				}
			}
		else  {
			batteryVoltage = voltage[map[BAT]];  // BAT Board present
			if (sim_mode && !sim_config) {	// if Voltage sensor on Battery board is present, exit simulated telemetry mode
				sim_mode = FALSE; 
				fprintf(stderr, "Turning off sim_mode since battery sensor is present\n");
			}
		}
        batteryCurrent = current[map[BAT]] + current[map[BAT2]];  // Sum BAT and BAT2 currents
	   
   }

	payload = get_payload_serial(FALSE);      
    printf("get_payload_status: %d \n", payload);  // not debug
	fflush(stdout); 
//	printf("String: %s\n", buffer2);       
	fflush(stdout);   
	strcpy(sensor_payload, buffer2);  
	   
     printf(" Response from STEM Payload: %s\n", sensor_payload);

	 char sensor_buffer[30];
	 int sensor_count;  
	 sensor_buffer[0] = 0;  
	 sensor_count = sensor_loop(sensor_buffer);
	 if (sensor_count > NEW_SENSOR_FIELDS_MAX)
		 sensor_count = NEW_SENSOR_FIELDS_MAX;
	 if (sensor_count > 0)  {
		char space[] = " ";
		strcat(sensor_payload, space);
	 	strcat(sensor_payload, sensor_buffer);  
	 	printf(" Payload after new sensor read: %s\n", sensor_payload);
	 }
        telem_file = fopen("/home/pi/CubeSatSim/telem.txt", "a");
//        printf("Writing payload string\n");
        time_t timeStamp;
        time(&timeStamp);   // get timestamp 
//      printf("Timestamp: %s\n", ctime(&timeStamp));
	    
        char timeStampNoNl[31], bat_string[31];    
        snprintf(timeStampNoNl, 30, "%.24s", ctime(&timeStamp)); 
//        printf("TimeStamp: %s\n", timeStampNoNl);
/*
	if (c2cStatus == DISABLED)      
		snprintf(bat_string, 30, "BAT %4.2f %5.1f", batteryVoltage, batteryCurrent);	
	else
		snprintf(bat_string, 30, "BAT %4.2f %5.1f C", batteryVoltage, batteryCurrent);	
	*/   
		snprintf(bat_string, 30, "BAT %4.2f %5.1f", batteryVoltage, batteryCurrent);	
	    if (c2cStatus != DISABLED)
			strcpy(bat_string," C");
	    if (sim_mode || (failureMode != FAIL_NONE))
			strcpy(bat_string," S");
        fprintf(telem_file, "%s %s %s\n", timeStampNoNl, bat_string, sensor_payload);	 // write telemetry string to telem.txt file    
        fclose(telem_file);

		if (failureMode == FAIL_PAYLOAD) {
			sensor_payload[0] = '\0';  // This will cause the payload to not be processed.
			printf("Simulated Payload Failure.\n");
		}

	    if (!sim_mode) {
        if ((sensor_payload[0] == 'O') && (sensor_payload[1] == 'K')) // only process if valid payload response
        {
//		  printf("Valid Payload!\n");  	
          int count1;
          char * token;
 
          const char space[2] = " ";
          token = strtok(sensor_payload, space);
//	  printf("token: %s\n", token);	
          for (count1 = 0; count1 < SENSOR_FIELDS; count1++) {
            if (token != NULL) {
              sensor[count1] = (float) atof(token);
			  strcpy(sensor_string[count1], token);	
//              #ifdef DEBUG_LOGGING
//                printf("sensor: %f ", sensor[count1]);  // print sensor data
//				printf("Sensor String %d is %s\n",count1, sensor_string[count1]);
//              #endif
              token = strtok(NULL, space);
            }
          }
	  

          printf("\n");
//	  if (sensor[GPS1] != 0) {     		
	  if ((sensor[GPS1] > -90.0) && (sensor[GPS1] < 90.0) && (sensor[GPS1] != 0.0))  { 
		if (sensor[GPS1] != latitude) {  
			latitude = sensor[GPS1];  
			printf("Latitude updated to %f \n", latitude); 
			newGpsTime = millis();  
	 	}
	  }
//	  if (sensor[GPS2] != 0)  {
	  if ((sensor[GPS2] > -180.0) && (sensor[GPS2] < 180.0) && (sensor[GPS2] != 0.0))  {   
		if (sensor[GPS2] != longitude) {  		  
			longitude = sensor[GPS2];  
			printf("Longitude updated to %f \n", longitude); 
			newGpsTime = millis();  
		}
	  }
    }
  	} 
//	else
//		; //payload = OFF;  // turn off since STEM Payload is not responding
      
      if ((millis() - newGpsTime) > 60000) {
		longitude += rnd_float(-0.05, 0.05) / 100.0;  // was .05
     		latitude += rnd_float(-0.05, 0.05) / 100.0;	      
//       		printf("GPS Location with Rnd: %f, %f \n", latitude, longitude);    
//	        printf("GPS Location with Rnd: APRS %07.2f, %08.2f \n", toAprsFormat(latitude), toAprsFormat(longitude));    
	      	newGpsTime = millis();  
      }

    if (sim_mode && (failureMode != FAIL_PAYLOAD)) {	   
		if (atmosphere == 0) {
			sensor[PRES] = 0;
			strcpy(sensor_string[PRES], "0.0");
			sensor[ALT] = 400;
			strcpy(sensor_string[ALT], "400");
			sensor[HUMI] = 0;
			strcpy(sensor_string[HUMI], "0.0");
			sensor[TEMP] = 0;	
			strcpy(sensor_string[TEMP], "0.0");
		} else {
			sensor[PRES] = 1015;
			strcpy(sensor_string[PRES], "1015");
			sensor[ALT] = 75;
			strcpy(sensor_string[ALT], "75");
			sensor[HUMI] = 48;
			strcpy(sensor_string[HUMI], "48");
			sensor[TEMP] = 27;
			strcpy(sensor_string[TEMP], "27.0");
		}
	   char sensor_number[20];	  
	   sensor[ACCEL_X] = axis[X];
	   sprintf(sensor_number, "%.2f", axis[X]);	  
	   strcpy(sensor_string[ACCEL_X], sensor_number);	  
	   sensor[ACCEL_Y] = axis[Y];
	   sprintf(sensor_number, "%.2f", axis[Y]);	  
	   strcpy(sensor_string[ACCEL_Y], sensor_number);	  	  
	   sensor[ACCEL_Z] = axis[Z];
	   sprintf(sensor_number, "%.2f", axis[Z]);	  
	   strcpy(sensor_string[ACCEL_Z], sensor_number);	  
		  
	   float spin;
	   spin = rnd_float(-30.0, 30.0);	  
	   sensor[GYRO_X] = axis[X] * spin;
	   sprintf(sensor_number, "%.2f", sensor[GYRO_X]);	  
	   strcpy(sensor_string[GYRO_X], sensor_number);	  	  
	   sensor[GYRO_Y] = axis[Y] * spin;
	   sprintf(sensor_number, "%.2f", sensor[GYRO_Y]);	  
	   strcpy(sensor_string[GYRO_Y], sensor_number);		  
	   sensor[GYRO_Z] = axis[Z] * spin;
	   sprintf(sensor_number, "%.2f", sensor[GYRO_Z]);	  
	   strcpy(sensor_string[GYRO_Z], sensor_number);	
	
	//   printf("sim sensor: %s\n", sensor_string[GYRO_Z]);	  
	   printf("sim sensor spin: %f value: %f length: %d string: %s\n", spin, sensor[GYRO_Z], strlen(sensor_string[GYRO_Z]), sensor_string[GYRO_Z]); 
   }	   

	  if (failureMode == FAIL_BME) {
			sensor[TEMP] = 0.0;
		    strcpy(sensor_string[TEMP], "0.0");
			sensor[PRES] = 0.0;		  
		    strcpy(sensor_string[PRES], "0.0");
		    sensor[HUMI] = 0.0;
		    strcpy(sensor_string[HUMI], "0.0");
		    sensor[ALT] = 0.0;	
		    strcpy(sensor_string[ALT], "0.0");				
		    printf("Simulated BME Failure!\n");
	  }

	  if (failureMode == FAIL_MPU) {
			sensor[ACCEL_X] = 0.0;
		    strcpy(sensor_string[ACCEL_X], "0.0");
			sensor[ACCEL_Y] = 0.0;
		    strcpy(sensor_string[ACCEL_Y], "0.0");		  
			sensor[ACCEL_Z] = 0.0;
		    strcpy(sensor_string[ACCEL_Z], "0.0");
			sensor[GYRO_X] = 0.0;
		    strcpy(sensor_string[GYRO_X], "0.0");
			sensor[GYRO_Y] = 0.0;
		    strcpy(sensor_string[GYRO_Y], "0.0");
			sensor[GYRO_Z] = 0.0;
		    strcpy(sensor_string[GYRO_Z], "0.0");		  
		    printf("Simulated MPU Failure!\n");
	  }

		if ((failureMode == FAIL_BME) || (failureMode == FAIL_MPU) || sim_mode) // recreaate sensor_payload string	
		{  
		  sensor_payload[0] = '\0';
		  strcpy(sensor_string[0], "OK");
		  strcpy(sensor_string[1], "BME280");
		  strcpy(sensor_string[6], "MPU6050");
			
          for (count1 = 0; count1 < SENSOR_FIELDS; count1++) {
			  strcat(sensor_payload, sensor_string[count1]);
			  strcat(sensor_payload, " ");
            }
		  printf("Updated Sensor String: %s\n", sensor_payload);	
		}
		else if (failureMode != FAIL_PAYLOAD) {
		  printf("Restoring sensor_payload\n");	
		  strcpy(sensor_payload, buffer2);  // restore sensor_payload after strtok operation
		}
	   
      if ((sensor_payload[0] == 'O') && (sensor_payload[1] == 'K')) {
//		printf("Valid Payload!!\n");  
        for (int count1 = 0; count1 < SENSOR_FIELDS; count1++) {
          if (sensor[count1] < sensor_min[count1])
            sensor_min[count1] = sensor[count1];
          if (sensor[count1] > sensor_max[count1])
            sensor_max[count1] = sensor[count1];
            //  printf("Smin %f Smax %f \n", sensor_min[count1], sensor_max[count1]);
        }
      }
	   
    if (sim_mode) { // simulated telemetry 

      double time = ((long int)millis() - time_start) / 1000.0;

      if ((time - eclipse_time) > period) {
        eclipse = (eclipse == 1) ? 0 : 1;
        eclipse_time = time;
        printf("\n\nSwitching eclipse mode! \n\n");
      }

      double Xi = eclipse * amps_max[0] * (float) sin(2.0 * 3.14 * time / (46.0 * speed)) + rnd_float(-2, 2);
      double Yi = eclipse * amps_max[1] * (float) sin((2.0 * 3.14 * time / (46.0 * speed)) + (3.14 / 2.0)) + rnd_float(-2, 2);
      double Zi = eclipse * amps_max[2] * (float) sin((2.0 * 3.14 * time / (46.0 * speed)) + 3.14 + angle[2]) + rnd_float(-2, 2);

      double Xv = eclipse * volts_max[0] * (float) sin(2.0 * 3.14 * time / (46.0 * speed)) + rnd_float(-0.2, 0.2);
      double Yv = eclipse * volts_max[1] * (float) sin((2.0 * 3.14 * time / (46.0 * speed)) + (3.14 / 2.0)) + rnd_float(-0.2, 0.2);
      double Zv = 2.0 * eclipse * volts_max[2] * (float) sin((2.0 * 3.14 * time / (46.0 * speed)) + 3.14 + angle[2]) + rnd_float(-0.2, 0.2);

       printf("Yi: %f Zi: %f %f %f Zv: %f \n", Yi, Zi, amps_max[2], angle[2], Zv);

      current[map[PLUS_X]] = (Xi >= 0) ? Xi : 0;
      current[map[MINUS_X]] = (Xi >= 0) ? 0 : ((-1.0f) * Xi);
      current[map[PLUS_Y]] = (Yi >= 0) ? Yi : 0;
      current[map[MINUS_Y]] = (Yi >= 0) ? 0 : ((-1.0f) * Yi);
      current[map[PLUS_Z]] = (Zi >= 0) ? Zi : 0;
      current[map[MINUS_Z]] = (Zi >= 0) ? 0 : ((-1.0f) * Zi);

      tempS += (eclipse > 0) ? ((temp_max - tempS) / 50.0f) : ((temp_min - tempS) / 50.0f);
      tempS += +rnd_float(-1.0, 1.0);
      //  IHUcpuTemp = (int)((tempS + rnd_float(-1.0, 1.0)) * 10 + 0.5);
      other[IHU_TEMP] = tempS;

      voltage[map[BAT2]] = 0.0; // rnd_float(5.0, 5.005);
      current[map[BAT2]] = 0.0; // rnd_float(158, 171);

      float charging = current[map[PLUS_X]] + current[map[MINUS_X]] + current[map[PLUS_Y]] + current[map[MINUS_Y]] + current[map[PLUS_Z]] + current[map[MINUS_Z]];
//      float charging = eclipse * (fabs(amps_max[0] * 0.707) + fabs(amps_max[1] * 0.707) + rnd_float(-4.0, 4.0));

//      current[map[BAT]] = ((current[map[BAT2]] * voltage[map[BAT2]]) / batt) - charging;
      current[map[BAT]] = rnd_float(320, 510) - charging;

        printf("charging: %f bat curr: %f bus curr: %f bat volt: %f bus volt: %f \n",charging, current[map[BAT]], current[map[BAT2]], batt, voltage[map[BAT2]]);

      batt -= (batt > 3.5) ? current[map[BAT]] / 300000 : current[map[BAT]] / 30000;
      if (batt < 3.6) {
        batt = 3.6;
        SafeMode = 1;
        printf("Safe Mode!\n");
      } else
        SafeMode= 0;

      if (batt > 4.1)
        batt = 4.1;

      voltage[map[BAT]] = batt + rnd_float(-0.01, 0.01);
		
      float Vm, Vp;
	  Vm = batt + 0.5;	
	  Vp = (Xv > 0) ? Xv : rnd_float(0.0, 0.1);
	  voltage[map[PLUS_X]] = (Vp >= Vm) ? (Vm + rnd_float(-0.1, 0.1)) : Vp;
      Vp = (Xv < 0) ? ((-1.0f) * Xv) : rnd_float(0.0, 0.1);
	  voltage[map[MINUS_X]] = (Vp >= Vm) ? (Vm + rnd_float(-0.1, 0.1)) : Vp;	
      Vp = (Yv > 0) ? Yv : rnd_float(0.0, 0.1);
	  voltage[map[PLUS_Y]] = (Vp >= Vm) ? (Vm + rnd_float(-0.1, 0.1)) : Vp;	
      Vp = (Yv < 0) ? ((-1.0f) * Yv) : rnd_float(0.0, 0.1);
	  voltage[map[MINUS_Y]] = (Vp >= Vm) ? (Vm + rnd_float(-0.1, 0.1)) : Vp;		
      Vp = (Zv > 0) ? Zv : rnd_float(0.0, 0.1);
	  voltage[map[PLUS_Z]] = (Vp >= Vm) ? (Vm + rnd_float(-0.1, 0.1)) : Vp;	
      Vp = (Zv < 0) ? ((-1.0f) * Zv) : rnd_float(0.0, 0.1);
	  voltage[map[MINUS_Z]] = (Vp >= Vm) ? (Vm + rnd_float(-0.1, 0.1)) : Vp;	

      printf("temp: %f Time: %f Eclipse: %d : %f %f | %f %f | %f %f\n",tempS, time, eclipse, voltage[map[PLUS_X]], voltage[map[MINUS_X]], voltage[map[PLUS_Y]], voltage[map[MINUS_Y]], current[map[PLUS_Z]], current[map[MINUS_Z]]);

      // end of simulated telemetry
	  }
	
      FILE * cpuTempSensor = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
      if (cpuTempSensor) {
   //     double cpuTemp;
        fscanf(cpuTempSensor, "%lf", & cpuTemp);
        cpuTemp /= 1000;

        #ifdef DEBUG_LOGGING
//        printf("CPU Temp Read: %6.1f\n", cpuTemp);
        #endif

        other[IHU_TEMP] = (double)cpuTemp;

      //  IHUcpuTemp = (int)((cpuTemp * 10.0) + 0.5);
      }
      fclose(cpuTempSensor);
    }   
	  
//    #ifdef DEBUG_LOGGING
    fprintf(stderr, "INFO: Battery voltage: %5.2f V  Threshold %5.2f V Current: %6.1f mA Threshold: %6.1f mA\n", batteryVoltage, voltageThreshold, batteryCurrent, currentThreshold);
//    #endif
	  
    if ((batteryCurrent > currentThreshold) && (batteryVoltage < (voltageThreshold + 0.15)) && !sim_mode && !hab_mode)
    {
	    fprintf(stderr,"Battery voltage low\n");
	    if (battery_saver_mode == OFF) {
		fprintf(stderr,"Switch to battery saver\n");    
	    	battery_saver(ON);
        	fprintf(stderr, "Safe Mode!\n");
		SafeMode = 1;    
		    
	    }
    } else if ((battery_saver_mode == ON) && (batteryCurrent < 0) && !sim_mode && !hab_mode)
    {
	    fprintf(stderr,"Battery is being charged - switch battery saver off\n");
	    if (battery_saver_mode == ON) {
	    	battery_saver(OFF);
        	fprintf(stderr, "Safe Mode off!\n");
		SafeMode = 0;  		    
	    }
    } 
    if ((batteryCurrent > currentThreshold) && (batteryVoltage < voltageThreshold) && !sim_mode && !hab_mode) // currentThreshold ensures that this won't happen when running on DC power.
    {
      fprintf(stderr, "Battery voltage too low: %f V - shutting down!\n", batteryVoltage);
      digitalWrite(txLed, txLedOff);
      digitalWrite(onLed, onLedOff);

      FILE * file6;	    
      file6 = popen("echo 'shutdown due to low battery voltage!' | wall", "r");
      pclose(file6);   
      	    
      sleep(1);
      digitalWrite(onLed, onLedOn);
      sleep(1);
      digitalWrite(onLed, onLedOff);
      sleep(1);
      digitalWrite(onLed, onLedOn);
      sleep(1);
      digitalWrite(onLed, onLedOff);

      file6 = popen("sudo shutdown -h now > /dev/null 2>&1", "r");
      pclose(file6);
      sleep(10);
    }
//#endif
	  
    FILE * fp = fopen("/home/pi/CubeSatSim/telem_string.txt", "w");
    if (fp != NULL)  {	  
//    	printf("Writing telem_string.txt\n");
	if (batteryVoltage != 4.5) {
/*		
		if (c2cStatus == DISABLED)
    			fprintf(fp, "BAT %4.2fV %4.0fmA\n", batteryVoltage, batteryCurrent);
		else
    			fprintf(fp, "BAT %4.2fV %4.0fmA C\n", batteryVoltage, batteryCurrent);	// show command and control is on		
		*/
		fprintf(fp, "BAT %4.2fV %4.0fmA", batteryVoltage, batteryCurrent);
		if (c2cStatus != DISABLED)
			fprintf(fp," C");
	    if (sim_mode || (failureMode != FAIL_NONE))
			fprintf(fp," S\n");
		else
			fprintf(fp,"\n");
	}
	else
    		fprintf(fp, "\n");	// don't show voltage and current if it isn't a sensor value
		
    	fclose(fp);	 
    } else 
	    printf("Error writing to telem_string.txt\n");
	    
/**/
    //  sleep(1);  // Delay 1 second
    ctr = 0;
    #ifdef DEBUG_LOGGING
//    fprintf(stderr, "INFO: Getting TLM Data\n");
    #endif

    FILE * command_file = fopen("/home/pi/CubeSatSim/command_control", "r");
    if (command_file == NULL) {
	    if (c2cStatus != DISABLED) {
	      fprintf(stderr,"Command and control is OFF\n");
	      c2cStatus = DISABLED;
	    }
    } else {
	    command_file = fopen("/home/pi/CubeSatSim/command_control_direwolf", "r");
	    if (command_file == NULL)  {
	      if (c2cStatus != CARRIER) {
		    fprintf(stderr,"Command and control Carrier (squelch) is ON\n");
		    c2cStatus = CARRIER;
	      }
	    } else {
		if (c2cStatus != DTMF_APRS) {
		    fprintf(stderr,"Command and control DTMF or APRS is ON\n");
		    c2cStatus = DTMF_APRS;
		}
	    }
    }	
  // printf("c2cStatus: %d \n", c2cStatus);	  
	  
    if ((mode == AFSK) || (mode == CW)) {
      get_tlm();
      sleep(25);
//      fprintf(stderr, "INFO: Sleeping for 25 sec\n");	 
	    
      int rand_sleep = (int)rnd_float(0.0, 5.0);	    
      sleep(rand_sleep);	    
//      fprintf(stderr, "INFO: Sleeping for extra %d sec\n", rand_sleep);	  
	    
    } else if ((mode == FSK) || (mode == BPSK)) {// FSK or BPSK
      get_tlm_fox();
    } else if ((mode == FC)) {
      get_tlm_fc();
    } else {  				// SSTV	    
//      fprintf(stderr, "Sleeping\n");
      sleep(30);	    
    }

    #ifdef DEBUG_LOGGING
//    fprintf(stderr, "INFO: Getting ready to send\n");
    #endif
  }

  if ((mode == BPSK) || (mode == FC)) {
//    digitalWrite(txLed, txLedOn);
    #ifdef DEBUG_LOGGING
//    printf("Tx LED On 1\n");
    #endif
//    printf("Sleeping to allow BPSK transmission to finish.\n");
    sleep((unsigned int)(loop_count * 5));
 //   printf("Done sleeping\n");
//    digitalWrite(txLed, txLedOff);
    #ifdef DEBUG_LOGGING
//    printf("Tx LED Off\n");
    #endif
  } else if (mode == FSK) {
//    printf("Sleeping to allow FSK transmission to finish.\n");
    sleep((unsigned int)loop_count);
//    printf("Done sleeping\n");
  }

  return 0;
}

//  Returns lower digit of a number which must be less than 99
//
int lower_digit(int number) {

  int digit = 0;
  if (number < 100)
    digit = number - ((int)(number / 10) * 10);
  else
    fprintf(stderr, "ERROR: Not a digit in lower_digit!\n");
  return digit;
}

// Returns upper digit of a number which must be less than 99
//
int upper_digit(int number) {

  int digit = 0;
  if (number < 100)

    digit = (int)(number / 10);
  else
    fprintf(stderr, "ERROR: Not a digit in upper_digit!\n");
  return digit;
}

void get_tlm(void) {

  FILE * txResult;

  for (int j = 0; j < frameCnt; j++) {
	  
    fflush(stdout);
    fflush(stderr);
	  
    int tlm[7][5];
    memset(tlm, 0, sizeof tlm);
	  
    tlm[1][A] = (int)(voltage[map[BAT2]] / 15.0 + 0.5) % 100; // Current of 5V supply to Pi
    tlm[1][B] = (int)(99.5 - current[map[PLUS_X]] / 10.0) % 100; // +X current [4]
    tlm[1][C] = (int)(99.5 - current[map[MINUS_X]] / 10.0) % 100; // X- current [10] 
    tlm[1][D] = (int)(99.5 - current[map[PLUS_Y]] / 10.0) % 100; // +Y current [7]

    tlm[2][A] = (int)(99.5 - current[map[MINUS_Y]] / 10.0) % 100; // -Y current [10] 
    tlm[2][B] = (int)(99.5 - current[map[PLUS_Z]] / 10.0) % 100; // +Z current [10] // was 70/2m transponder power, AO-7 didn't have a Z panel
    tlm[2][C] = (int)(99.5 - current[map[MINUS_Z]] / 10.0) % 100; // -Z current (was timestamp)
    tlm[2][D] = (int)(50.5 + current[map[BAT]] / 10.0) % 100; // NiMH Battery current

//    tlm[3][A] = abs((int)((voltage[map[BAT]] * 10.0) - 65.5) % 100);
    if (voltage[map[BAT]] > 4.6)	 
    	tlm[3][A] = (int)((voltage[map[BAT]] * 10.0) - 65.5) % 100;  // 7.0 - 10.0 V for old 9V battery
    else
    	tlm[3][A] = (int)((voltage[map[BAT]] * 10.0) + 44.5) % 100;  // 0 - 4.5 V for new 3 cell battery
	    
    tlm[3][B] = (int)(voltage[map[BAT2]] * 10.0) % 100; // 5V supply to Pi

    tlm[4][A] = (int)((95.8 - other[IHU_TEMP]) / 1.48 + 0.5) % 100;  // was [B] but didn't display in online TLM spreadsheet
		
    tlm[6][B] = 0;
    tlm[6][D] = 49 + rand() % 3;
/*
    #ifdef DEBUG_LOGGING
    // Display tlm
    int k, j;
    for (k = 1; k < 7; k++) {
      for (j = 1; j < 5; j++) {
        printf(" %2d ", tlm[k][j]);
      }
      printf("\n");
    }
    #endif
*/	  

    char str[1000];
    char tlm_str[1000];
    char header_str[] = "\x03\xf0"; // hi hi ";
    char header_str3[] = "echo '";
    char header_str2[] = "-11>APCSS:";
    char header_str2b[30]; // for APRS coordinates
    char header_lat[10];
    char header_long[10];
    char header_str4[] = "hi hi de ";
    char header_c2c[] = "  C";
//    char footer_str1[] = "\' > t.txt && echo \'";
    char footer_str1[] = "\' > t.txt";
//    char footer_str[] = "-11>APCSS:010101/hi hi ' >> t.txt && touch /home/pi/CubeSatSim/ready";  // transmit is done by transmit.py
    char footer_str[] = " && echo 'AMSAT-11>APCSS:010101/hi hi ' >> t.txt && touch /home/pi/CubeSatSim/ready";  // transmit is done by transmit.py
    char footer_str2[] = " && touch /home/pi/CubeSatSim/ready"; 
	char zero[] = "0.0";  
	  
   
      strcpy(str, header_str3);
//    }
      if (mode == AFSK) {
        strcat(str, call);
        strcat(str, header_str2);	    
      }	    
    
//      printf("Str: %s \n", str);
      if (mode != CW) {
         //	sprintf(header_str2b, "=%7.2f%c%c%c%08.2f%cShi hi ",4003.79,'N',0x5c,0x5c,07534.33,'W');  // add APRS lat and long
        if (latitude > 0)
          sprintf(header_lat, "%07.2f%c", toAprsFormat(latitude), 'N'); // lat
        else
          sprintf(header_lat, "%07.2f%c", toAprsFormat(latitude) * (-1.0), 'S'); // lat
        if (longitude > 0)
          sprintf(header_long, "%08.2f%c", toAprsFormat(longitude) , 'E'); // long
        else
          sprintf(header_long, "%08.2f%c",toAprsFormat( longitude) * (-1.0), 'W'); // long
	      
//#ifdef HAB
	if (hab_mode)	
		sprintf(header_str2b, "=%s%c%sOhi hi ", header_lat, 0x2f, header_long); // add APRS lat and long with Balloon HAB icon
//#else
	else
		sprintf(header_str2b, "=%s%c%c%sShi hi ", header_lat, 0x5c, 0x5c, header_long); // add APRS lat and long with Satellite icon	
//#endif		
          	           	    
        printf("\n\nString is %s \n\n", header_str2b);
        strcat(str, header_str2b);
      } else {  // CW mode
        strcat(str, header_str4);
	strcat(str, call);
	if (c2cStatus != DISABLED) {
		strcat(str, header_c2c);
	}

	sprintf(tlm_str, "%s' > cw0.txt", &str);   
	printf("CW string to execute: %s\n", &tlm_str);     
	FILE * cw_file = popen(tlm_str, "r");
        pclose(cw_file);      
	      
      }
//    }
	printf("Str: %s \n", str);
   if (mode == CW) {
    int channel;
    for (channel = 1; channel < 7; channel++) {
      sprintf(tlm_str, "echo -n ' %d%d%d %d%d%d %d%d%d %d%d%d ' > cw%1d.txt",
        channel, upper_digit(tlm[channel][1]), lower_digit(tlm[channel][1]),
        channel, upper_digit(tlm[channel][2]), lower_digit(tlm[channel][2]),
        channel, upper_digit(tlm[channel][3]), lower_digit(tlm[channel][3]),
        channel, upper_digit(tlm[channel][4]), lower_digit(tlm[channel][4]), channel);

	strcat(str, tlm_str);
	    
	printf("CW string to execute: %s\n", &tlm_str);
	    
	FILE * cw_file = popen(tlm_str, "r");
        pclose(cw_file);	     

    }
//    if (c2cStatus != DISABLED) {	   
//      FILE *file_append = sopen("echo 'C' >> cw6.txt");
//      fclose(file_append);
//    }
  } else {  // APRS
/*
      if (c2cStatus == 0)	   
        sprintf(tlm_str, "BAT %4.2f %5.1f ", batteryVoltage, batteryCurrent); 
      else
        sprintf(tlm_str, "BAT %4.2f %5.1f C ", batteryVoltage, batteryCurrent); 
*/
		snprintf(tlm_str, 30, "BAT %4.2f %5.1f ", batteryVoltage, batteryCurrent);	
		if (c2cStatus != DISABLED)
			strcpy(tlm_str,"C ");
		if (sim_mode || (failureMode != FAIL_NONE))
			strcpy(tlm_str,"S ");
	   
      	strcat(str, tlm_str);
  }  
	  
//    strcpy(sensor_payload, buffer2);      	  
    printf(" sensor_payload: %s\n", sensor_payload);
//    printf(" Str so far: %s\n", str);   
	  
    if (mode != CW) 
        strcat(str, sensor_payload); // append to telemetry string for transmission

    if (mode == CW) {

//      char cw_str2[1000];
//      char cw_header2[] = "echo '";
//      char cw_footer2[] = "' > id.txt && gen_packets -M 20 id.txt -o morse.wav -r 48000 > /dev/null 2>&1 && cat morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/transmit/transmit -i- -m RF -f 434.897e3";
      char cw_footer3[] = "' > cw.txt && touch /home/pi/CubeSatSim/cwready";  // transmit is done by transmit.py
      char cwready[] = "touch /home/pi/CubeSatSim/cwready";  // cw frame is complete. transmit is done by transmit.py
	    
      FILE * cw_file = popen(cwready, "r");
      pclose(cw_file);	    
	    
      while ((cw_file = fopen("/home/pi/CubeSatSim/cwready", "r")) != NULL) {  // wait for transmit  to be done
        fclose(cw_file); 
//	printf("Sleeping while waiting for transmit \n");
//	fflush(stdout);      
        sleep(5);	      
      }    
    } 
    else {  // APRS using transmit
	    
      strcat(str, footer_str1);
//      strcat(str, call);
      if (battery_saver_mode  == ON)	    
      	strcat(str, footer_str);  // add extra packet for transmit transmission
      else
      	strcat(str, footer_str2);
	    
      fprintf(stderr, "APRS String to execute: %s\n", str);
	    
      printf("\n\nTelemetry string is %s \n\n", str);	
	    
      if (transmit) {
        FILE * file2 = popen(str, "r");
        pclose(file2);
	      
	sleep(2);
	digitalWrite(txLed, txLedOff);
      
      } else {
        fprintf(stderr, "\nNo CubeSatSim Band Pass Filter detected.  No transmissions after the CW ID.\n");
        fprintf(stderr, " See http://cubesatsim.org/wiki for info about building a CubeSatSim\n\n");
      }

      sleep(3);
    }

  }

  return;
}

// generates telemetry which is decoded by AMSAT's FoxTelem: https://www.amsat.org/foxtelem-software-for-windows-mac-linux/
// for more info about how we use FoxTelem see https://www.g0kla.com/foxtelem/amsat_telemetry_designers_handbook.pdf

void get_tlm_fox() {  
  int i;

  long int sync = syncWord;
  int cam = ON;

  smaller = (int) (S_RATE / (2 * freq_Hz));

  short int b[dataLen];
  short int b_max[dataLen];
  short int b_min[dataLen];
	
  memset(b, 0, sizeof(b));
  memset(b_max, 0, sizeof(b_max));
  memset(b_min, 0, sizeof(b_min));
	
  short int h[headerLen];
  memset(h, 0, sizeof(h));

  memset(buffer, 0xa5, sizeof(buffer));

  short int rs_frame[rsFrames][223];
  unsigned char parities[rsFrames][parityLen], inputByte;

  int id, frm_type = 0x01, NormalModeFailure = 0;
  int PayloadFailure1 = 0, PayloadFailure2 = 0;
  int BAT2Voltage = 0, BAT2Current = 0, Resets = 0, Rssi = 2048;
  int batt_a_v = 0, batt_b_v = 0, batt_c_v = 0, battCurr = 0;
  int posXv = 0, negXv = 0, posYv = 0, negYv = 0, posZv = 0, negZv = 0;
  int posXi = 0, negXi = 0, posYi = 0, negYi = 0, posZi = 0, negZi = 0;
  int head_offset = 0;

  STEMBoardFailure = 1;

  short int buffer_test[bufLen];
  int buffSize;
  buffSize = (int) sizeof(buffer_test);
	
  if (failureMode == FAIL_NONE) 
	  printf("No Simulated Failure!\n");	
//  if (failureMode == -1) {
//	  failureMode = (int) rnd_float(1, FAIL_COUNT);
//	  printf("Random Failure\n");
//  }
  if (failureMode == FAIL_UNPLUG) {
	  voltage[map[PLUS_Y]] = rnd_float(0.8, 0.95);
	  current[map[PLUS_Y]] = 0.0;
	  printf("+Y Solar Unplugged Failure\n");
  }	
  if (failureMode == FAIL_SOLAR) {
	  voltage[map[PLUS_X]] = 0.0;
	  current[map[PLUS_X]] = 0.0;
	  printf("+X Solar Simulated Failure\n");
  }
  if (failureMode == FAIL_DEGRADE) {
	  voltage[map[MINUS_X]] = voltage[map[MINUS_X]] * 0.5;
	  current[map[MINUS_X]] = current[map[MINUS_X]] * 0.5;
	  printf("-X Solar Deg Simulated Failure\n");	  
  }
  if (failureMode == FAIL_SHORT) {
	  voltage[map[MINUS_Y]] = 0.0;
	  printf("-Y Solar SC Simulated Failure!\n");	  
  }
  if (failureMode == FAIL_I2C1)  {
	  voltage[map[PLUS_X]] = 0.0;
	  current[map[PLUS_X]] = 0.0;
	  voltage[map[PLUS_Y]] = 0.0;
	  current[map[PLUS_Y]] = 0.0;
	  voltage[map[BAT]] = 0.0;
	  current[map[BAT]] = 0.0;
	  voltage[map[BAT2]] = 0.0;
	  current[map[BAT2]] = 0.0;	  
	  printf("I2C Bus 1 Simulated Failure!\n");	
  } 
  if (failureMode == FAIL_I2C3)  {
	  voltage[map[MINUS_X]] = 0.0;
	  current[map[MINUS_X]] = 0.0;
	  voltage[map[MINUS_Y]] = 0.0;
	  current[map[MINUS_Y]] = 0.0;
	  voltage[map[MINUS_Z]] = 0.0;
	  current[map[MINUS_Z]] = 0.0;	  
	  voltage[map[PLUS_Z]] = 0.0;
	  current[map[PLUS_Z]] = 0.0;	  
	  printf("I2C Bus 3 Simulated Failure!\n");	
  }		
  if (failureMode == FAIL_PAYLOAD) {
	  payload = OFF;
	  printf("Payload Simulated Failure!\n");	  
  }
  if (failureMode == FAIL_CAMERA) {
	  cam = OFF;
	  printf("Camera Simulated Failure!\n");	  
  }

  if (mode == FSK)
    id = 7;
  else
    id = 0; // 99 in h[6]
	
  //  for (int frames = 0; frames < FRAME_CNT; frames++) 
  for (int frames = 0; frames < frameCnt; frames++) {
  
//    if (firstTime != ON) {
	if (TRUE) {	
      // delay for sample period

/**/
//      while ((millis() - sampleTime) < (unsigned int)samplePeriod)
     int startSleep = millis();	    
     if ((millis() - sampleTime) < ((unsigned int)frameTime - 750 + pi_zero_2_offset))  // was 250 100 500 for FSK
//        sleep(2.0); // 0.5);  // 25);  // initial period
        sleep(1.0); // 0.5);  // 25);  // initial period
     while ((millis() - sampleTime) < ((unsigned int)frameTime - 750 + pi_zero_2_offset))  // was 250 100
        sleep(0.1); // 25); // 0.5);  // 25);
//        sleep((unsigned int)sleepTime);
/**/
      printf("Start sleep %d Sleep period: %d  while period: %d\n", startSleep, millis() - startSleep, (unsigned int)frameTime - 750 + pi_zero_2_offset);
      fflush(stdout);
      
      sampleTime = (unsigned int) millis();
    } else
      printf("first or second time - no sleep\n");

    printf("++++ Loop time: %5.3f sec +++++\n", (millis() - loopTime)/1000.0);
    fflush(stdout);
    loopTime = millis();	  
	
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
      }
       for (int count1 = 0; count1 < 3; count1++) {
        if (other[count1] < other_min[count1])
          other_min[count1] = other[count1];
        if (other[count1] > other_max[count1])
          other_max[count1] = other[count1];

        //  printf("Other min %f max %f \n", other_min[count1], other_max[count1]);
      }
      	  if (mode == FSK)
	  {
	      if (loop % 32 == 0) {  // was 8
//		printf("Sending MIN frame \n");
		frm_type = 0x03;
		for (int count1 = 0; count1 < SENSOR_FIELDS; count1++) {
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
	      if ((loop + 16) % 32 == 0) {  // was 8
//		printf("Sending MAX frame \n");
		frm_type = 0x02;
		for (int count1 = 0; count1 < SENSOR_FIELDS; count1++) {
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
	  }
	  else
	  	frm_type = 0x02;  // BPSK always send MAX MIN frame
    } 	  
    sensor_payload[0] = 0;  // clear for next payload
	  
//   if (mode == FSK) {	// remove this 
//   }
    memset(rs_frame, 0, sizeof(rs_frame));
    memset(parities, 0, sizeof(parities));

    h[0] = (short int) ((h[0] & 0xf8) | (id & 0x07)); // 3 bits
     if (uptime != 0)	  // if uptime is 0, leave reset count at 0
    {
      h[0] = (short int) ((h[0] & 0x07) | ((reset_count & 0x1f) << 3));
      h[1] = (short int) ((reset_count >> 5) & 0xff);
      h[2] = (short int) ((h[2] & 0xf8) | ((reset_count >> 13) & 0x07));
    }
    h[2] = (short int) ((h[2] & 0x0e) | ((uptime & 0x1f) << 3));
    h[3] = (short int) ((uptime >> 5) & 0xff);
    h[4] = (short int) ((uptime >> 13) & 0xff);
    h[5] = (short int) ((h[5] & 0xf0) | ((uptime >> 21) & 0x0f));
    h[5] = (short int) ((h[5] & 0x0f) | (frm_type << 4));

    if (mode == BPSK)
      h[6] = 99;

    posXi = (int)(current[map[PLUS_X]] + 0.5) + 2048;
    posYi = (int)(current[map[PLUS_Y]] + 0.5) + 2048;
    posZi = (int)(current[map[PLUS_Z]] + 0.5) + 2048;
    negXi = (int)(current[map[MINUS_X]] + 0.5) + 2048;
    negYi = (int)(current[map[MINUS_Y]] + 0.5) + 2048;
    negZi = (int)(current[map[MINUS_Z]] + 0.5) + 2048;

    posXv = (int)(voltage[map[PLUS_X]] * 100);
    posYv = (int)(voltage[map[PLUS_Y]] * 100);
    posZv = (int)(voltage[map[PLUS_Z]] * 100);
    negXv = (int)(voltage[map[MINUS_X]] * 100);
    negYv = (int)(voltage[map[MINUS_Y]] * 100);
    negZv = (int)(voltage[map[MINUS_Z]] * 100);

    batt_c_v = (int)(voltage[map[BAT]] * 100);
    battCurr = (int)(current[map[BAT]] + 0.5) + 2048;
    BAT2Voltage = (int)(voltage[map[BAT2]] * 100);
    BAT2Current = (int)(current[map[BAT2]] + 0.5) + 2048;

    if (payload == ON)
      STEMBoardFailure = 0;

    // read payload sensor if available

//    encodeA(b, 0 + head_offset, batt_a_v);  // replaced by XS2 and XS3 below
//    encodeB(b, 1 + head_offset, batt_b_v);
    encodeA(b, 3 + head_offset, batt_c_v);

    encodeB(b, 4 + head_offset, (int)(sensor[ACCEL_X] * 100 + 0.5) + 2048); // Xaccel
    encodeA(b, 6 + head_offset, (int)(sensor[ACCEL_Y] * 100 + 0.5) + 2048); // Yaccel
    encodeB(b, 7 + head_offset, (int)(sensor[ACCEL_Z] * 100 + 0.5) + 2048); // Zaccel

    encodeA(b, 9 + head_offset, battCurr);

	encodeB(b, 10 + head_offset, (int)(sensor[TEMP] * 10 + 0.5)); // Temp	  

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
	    
      encodeA(b_max, 12 + head_offset, (int)(voltage_max[map[PLUS_X]] * 100));
      encodeB(b_max, 13 + head_offset, (int)(voltage_max[map[PLUS_Y]] * 100));
      encodeA(b_max, 15 + head_offset, (int)(voltage_max[map[PLUS_Z]] * 100));
      encodeB(b_max, 16 + head_offset, (int)(voltage_max[map[MINUS_X]] * 100));
      encodeA(b_max, 18 + head_offset, (int)(voltage_max[map[MINUS_Y]] * 100));
      encodeB(b_max, 19 + head_offset, (int)(voltage_max[map[MINUS_Z]] * 100));

      encodeA(b_max, 21 + head_offset, (int)(current_max[map[PLUS_X]] + 0.5) + 2048);
      encodeB(b_max, 22 + head_offset, (int)(current_max[map[PLUS_Y]] + 0.5) + 2048);
      encodeA(b_max, 24 + head_offset, (int)(current_max[map[PLUS_Z]] + 0.5) + 2048);
      encodeB(b_max, 25 + head_offset, (int)(current_max[map[MINUS_X]] + 0.5) + 2048);
      encodeA(b_max, 27 + head_offset, (int)(current_max[map[MINUS_Y]] + 0.5) + 2048);
      encodeB(b_max, 28 + head_offset, (int)(current_max[map[MINUS_Z]] + 0.5) + 2048);	    

      encodeA(b_max, 9 + head_offset, (int)(current_max[map[BAT]] + 0.5) + 2048);
      encodeA(b_max, 3 + head_offset, (int)(voltage_max[map[BAT]] * 100));
      encodeA(b_max, 30 + head_offset, (int)(voltage_max[map[BAT2]] * 100));
      encodeB(b_max, 46 + head_offset, (int)(current_max[map[BAT2]] + 0.5) + 2048);
	    
      encodeB(b_max, 37 + head_offset, (int)(other_max[RSSI] + 0.5) + 2048);	    
      encodeA(b_max, 39 + head_offset, (int)(other_max[IHU_TEMP] * 10 + 0.5));
      encodeB(b_max, 31 + head_offset, ((int)(other_max[SPIN] * 10)) + 2048);

	      if (sensor_min[TEMP] != 1000.0) // make sure values are valid
	      {	     

	      		  encodeB(b_max, 4 + head_offset, (int)(sensor_max[ACCEL_X] * 100 + 0.5) + 2048); // Xaccel
			      encodeA(b_max, 6 + head_offset, (int)(sensor_max[ACCEL_Y] * 100 + 0.5) + 2048); // Yaccel
			      encodeB(b_max, 7 + head_offset, (int)(sensor_max[ACCEL_Z] * 100 + 0.5) + 2048); // Zaccel	    
			      encodeB(b_max, 40 + head_offset, (int)(sensor_max[GYRO_X] + 0.5) + 2048);
			      encodeA(b_max, 42 + head_offset, (int)(sensor_max[GYRO_Y] + 0.5) + 2048);
			      encodeB(b_max, 43 + head_offset, (int)(sensor_max[GYRO_Z] + 0.5) + 2048);
			  
				  encodeA(b_max, 33 + head_offset, (int)(sensor_max[PRES] + 0.5)); // Pressure
			      encodeB(b_max, 34 + head_offset, (int)(sensor_max[ALT] * 10.0 + 0.5)); // Altitude
		//	      encodeB(b_max, 49 + head_offset, (int)(sensor_max[XS1] * 10 + 0.5) + 2048);
			      encodeB(b_max, 10 + head_offset, (int)(sensor_max[TEMP] * 10 + 0.5)); 	
			      encodeA(b_max, 45 + head_offset, (int)(sensor_max[HUMI] * 10 + 0.5));
		      encodeA(b_max, 48 + head_offset, (int)(sensor_max[DTEMP] * 10 + 0.5) + 2048);
				  
		      encodeB(b_max, 49 + head_offset, (int)(sensor_max[XS1]));
		      encodeA(b_max, 0 + head_offset, (int)(sensor_max[XS2]));
		      encodeB(b_max, 1 + head_offset, (int)(sensor_max[XS3]));
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
	//	      encodeB(b_max, 49 + head_offset, 2048);
	      }

      encodeA(b_min, 12 + head_offset, (int)(voltage_min[map[PLUS_X]] * 100));
      encodeB(b_min, 13 + head_offset, (int)(voltage_min[map[PLUS_Y]] * 100));
      encodeA(b_min, 15 + head_offset, (int)(voltage_min[map[PLUS_Z]] * 100));
      encodeB(b_min, 16 + head_offset, (int)(voltage_min[map[MINUS_X]] * 100));
      encodeA(b_min, 18 + head_offset, (int)(voltage_min[map[MINUS_Y]] * 100));
      encodeB(b_min, 19 + head_offset, (int)(voltage_min[map[MINUS_Z]] * 100));

      encodeA(b_min, 21 + head_offset, (int)(current_min[map[PLUS_X]] + 0.5) + 2048);
      encodeB(b_min, 22 + head_offset, (int)(current_min[map[PLUS_Y]] + 0.5) + 2048);
      encodeA(b_min, 24 + head_offset, (int)(current_min[map[PLUS_Z]] + 0.5) + 2048);
      encodeB(b_min, 25 + head_offset, (int)(current_min[map[MINUS_X]] + 0.5) + 2048);
      encodeA(b_min, 27 + head_offset, (int)(current_min[map[MINUS_Y]] + 0.5) + 2048);
      encodeB(b_min, 28 + head_offset, (int)(current_min[map[MINUS_Z]] + 0.5) + 2048);	
	    
      encodeA(b_min, 9 + head_offset, (int)(current_min[map[BAT]] + 0.5) + 2048);
      encodeA(b_min, 3 + head_offset, (int)(voltage_min[map[BAT]] * 100));
      encodeA(b_min, 30 + head_offset, (int)(voltage_min[map[BAT2]] * 100));
      encodeB(b_min, 46 + head_offset, (int)(current_min[map[BAT2]] + 0.5) + 2048);
	    
      encodeB(b_min, 31 + head_offset, ((int)(other_min[SPIN] * 10)) + 2048);
      encodeB(b_min, 37 + head_offset, (int)(other_min[RSSI] + 0.5) + 2048);	    
      encodeA(b_min, 39 + head_offset, (int)(other_min[IHU_TEMP] * 10 + 0.5));

	      if (sensor_min[TEMP] != 1000.0) // make sure values are valid
	      {	   
		      encodeB(b_min, 4 + head_offset, (int)(sensor_min[ACCEL_X] * 100 + 0.5) + 2048); // Xaccel
		      encodeA(b_min, 6 + head_offset, (int)(sensor_min[ACCEL_Y] * 100 + 0.5) + 2048); // Yaccel
		      encodeB(b_min, 7 + head_offset, (int)(sensor_min[ACCEL_Z] * 100 + 0.5) + 2048); // Zaccel	
		      encodeB(b_min, 40 + head_offset, (int)(sensor_min[GYRO_X] + 0.5) + 2048);
		      encodeA(b_min, 42 + head_offset, (int)(sensor_min[GYRO_Y] + 0.5) + 2048);
		      encodeB(b_min, 43 + head_offset, (int)(sensor_min[GYRO_Z] + 0.5) + 2048);
			  
		      encodeA(b_min, 33 + head_offset, (int)(sensor_min[PRES] + 0.5)); // Pressure
		      encodeB(b_min, 34 + head_offset, (int)(sensor_min[ALT] * 10.0 + 0.5)); // Altitude
		      encodeB(b_min, 10 + head_offset, (int)(sensor_min[TEMP] * 10 + 0.5)); 	    
		      encodeA(b_min, 45 + head_offset, (int)(sensor_min[HUMI] * 10 + 0.5));
		      encodeA(b_min, 48 + head_offset, (int)(sensor_min[DTEMP] * 10 + 0.5) + 2048);
	//	      encodeB(b_min, 49 + head_offset, (int)(sensor_min[XS1] * 10 + 0.5) + 2048);

	
		      encodeB(b_min, 49 + head_offset, (int)(sensor_min[XS1]));
		      encodeA(b_min, 0 + head_offset, (int)(sensor_min[XS2]));
		      encodeB(b_min, 1 + head_offset, (int)(sensor_min[XS3]));	      	      	      
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
	//	      encodeB(b_min, 49 + head_offset, 2048);
	    }	

    }    
    encodeA(b, 30 + head_offset, BAT2Voltage);

    encodeB(b, 31 + head_offset, ((int)(other[SPIN] * 10)) + 2048);

	  		encodeA(b, 33 + head_offset, (int)(sensor[PRES] + 0.5)); // Pressure
		    encodeB(b, 34 + head_offset, (int)(sensor[ALT] * 10.0 + 0.5)); // Altitude
		    encodeA(b, 45 + head_offset, (int)(sensor[HUMI] * 10 + 0.5)); // in place of sensor1
		    encodeA(b, 39 + head_offset, (int)(other[TEMP] * 10 + 0.5));
 
	    encodeA(b, 36 + head_offset, Resets);
	    encodeB(b, 37 + head_offset, (int)(other[RSSI] + 0.5) + 2048);
	
	
		    encodeB(b, 40 + head_offset, (int)(sensor[GYRO_X] + 0.5) + 2048);
		    encodeA(b, 42 + head_offset, (int)(sensor[GYRO_Y] + 0.5) + 2048);
		    encodeB(b, 43 + head_offset, (int)(sensor[GYRO_Z] + 0.5) + 2048);
		encodeA(b, 48 + head_offset, (int)(sensor[DTEMP] * 10 + 0.5) + 2048);
		encodeB(b, 49 + head_offset, (int)(sensor[XS1]));
    	encodeA(b, 0 + head_offset, (int)(sensor[XS2]));
    	encodeB(b, 1 + head_offset, (int)(sensor[XS3]));

    encodeB(b, 46 + head_offset, BAT2Current);
    encodeA(b, 39 + head_offset, (int)(other[IHU_TEMP] * 10 + 0.5));
	  
//    encodeB(b, 49 + head_offset, (int)(sensor[XS1] * 10 + 0.5) + 2048);
	  
    FILE * command_count_file = fopen("/home/pi/CubeSatSim/command_count.txt", "r");
    if (command_count_file != NULL) {	
      char count_string[10];	
      if ( (fgets(count_string, 10, command_count_file)) != NULL)
	   groundCommandCount = atoi(count_string); 
//      fclose(command_count_file);	    
    } else 
	    printf("Error opening command_count.txt!\n");
    fclose(command_count_file);
    
//    printf("Command count: %d\n", groundCommandCount);
	int simulated; 
	simulated = sim_mode;  
	if (failureMode != FAIL_NONE)  {
		simulated = TRUE;
//		printf("Showing Simulted in FoxTelem\n");
	}
	int i2c_1, i2c_3; 
	i2c_1 = i2c_bus1;  
	i2c_3 = i2c_bus3; 
//	printf("Bus1: %d Bus2: %d  \n", i2c_1, i2c_3);
	if (failureMode == FAIL_I2C1)  {
		i2c_1 = OFF;
//		printf("I2C Bus 1 Simulated Failure\n");
	} else 	if (failureMode == FAIL_I2C3)  {
		i2c_3 = OFF;
//		printf("I2C Bus 3 Simulated Failure\n");
	}		  
//  int status = STEMBoardFailure + SafeMode * 2 + sim_mode * 4 + PayloadFailure1 * 8 +    
//      (i2c_bus0 == OFF) * 16 + (i2c_bus1 == OFF) * 32 + (i2c_bus3 == OFF) * 64 + (camera == OFF) * 128 + groundCommandCount * 256;
    int status = STEMBoardFailure + SafeMode * 2 + simulated * 4 + PayloadFailure1 * 8 +
      (i2c_bus0 == OFF) * 16 + (i2c_1 == OFF) * 32 + (i2c_3 == OFF) * 64 + (cam == OFF) * 128 + groundCommandCount * 256;

    encodeA(b, 51 + head_offset, status);
    encodeB(b, 52 + head_offset, rxAntennaDeployed + txAntennaDeployed * 2 + c2cStatus * 4);
    if (mode == BPSK) {	  
      encodeA(b_max, 51 + head_offset, status);
      encodeA(b_min, 51 + head_offset, status);
      encodeB(b_max, 52 + head_offset, rxAntennaDeployed + txAntennaDeployed * 2 + c2cStatus * 4);
      encodeB(b_min, 52 + head_offset, rxAntennaDeployed + txAntennaDeployed * 2 + c2cStatus * 4);
    }
	  
    if (txAntennaDeployed == 0) {
      txAntennaDeployed = 1;
      printf("TX Antenna Deployed!\n");
    }
    if (rxAntennaDeployed == 0) {
      rxAntennaDeployed = 1;
      printf("RX Antenna Deployed!\n");
    }	  
    
    if (mode == BPSK) {  // wod field experiments
      unsigned long val = 0xffff;
      encodeA(b, 64 + head_offset, 0xff & val); 
      encodeA(b, 65 + head_offset, val >> 8); 	    
      encodeA(b, 63 + head_offset, 0x00); 
      encodeA(b, 62 + head_offset, 0x01);
      encodeB(b, 74 + head_offset, 0xfff); 
    }	  
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

    #ifdef DEBUG_LOGGING
    //	printf("\nAt end of data8 write, %d ctr1 values written\n\n", ctr1);
    /*
    	  printf("Parities ");
    		for (int m = 0; m < parityLen; m++) {
    		 	printf("%d ", parities[0][m]);
    		}
    		printf("\n");
    */
    #endif
	   
    int ctr2 = 0;
    memset(data10, 0, sizeof(data10));

    for (i = 0; i < dataLen * payloads + headerLen; i++) // 476 for BPSK
    {
      data10[ctr2] = (Encode_8b10b[rd][((int) data8[ctr2])] & 0x3ff);
      nrd = (Encode_8b10b[rd][((int) data8[ctr2])] >> 10) & 1;
      //		printf ("data10[%d] = encoded data8[%d] = %x \n",
      //		 	ctr2, ctr2, data10[ctr2]); 

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

          rd = nrd;
        }
      }
 //   }
    #ifdef DEBUG_LOGGING
    // 	printf("\nAt end of data10 write, %d ctr2 values written\n\n", ctr2);
    #endif

    int data;
    int val;
    //int offset = 0;

    #ifdef DEBUG_LOGGING
    //	printf("\nAt start of buffer loop, syncBits %d samples %d ctr %d\n", syncBits, samples, ctr);
    #endif

    for (i = 1; i <= syncBits * samples; i++) {
      write_wave(ctr, buffer);
      //		printf("%d ",ctr);
      if ((i % samples) == 0) {
        int bit = syncBits - i / samples + 1;
        val = sync;
        data = val & 1 << (bit - 1);
        //   	printf ("%d i: %d new frame %d sync bit %d = %d \n",
        //  		 ctr/, i, frames, bit, (data > 0) );
        if (mode == FSK) {
          phase = ((data != 0) * 2) - 1;
          //		printf("Sending a %d\n", phase);
        } else {
          if (data == 0) {
            phase *= -1;
            if ((ctr - smaller) > 0) {
              for (int j = 1; j <= smaller; j++)
                buffer[ctr - j] = buffer[ctr - j] * 0.4;
            }
            flip_ctr = ctr;
          }
        }
      }
    }
    #ifdef DEBUG_LOGGING
    //	printf("\n\nValue of ctr after header: %d Buffer Len: %d\n\n", ctr, buffSize);
    #endif
    for (i = 1; i <= (10 * (headerLen + dataLen * payloads + rsFrames * parityLen) * samples); i++) // 572   
    {
      write_wave(ctr, buffer);
      if ((i % samples) == 0) {
        int symbol = (int)((i - 1) / (samples * 10));
        int bit = 10 - (i - symbol * samples * 10) / samples + 1;
        val = data10[symbol];
        data = val & 1 << (bit - 1);
        //		printf ("%d i: %d new frame %d data10[%d] = %x bit %d = %d \n",
        //	    		 ctr/, i, frames, symbol, val, bit, (data > 0) );
        if (mode == FSK) {
          phase = ((data != 0) * 2) - 1;
          //			printf("Sending a %d\n", phase);
        } else {
          if (data == 0) {
            phase *= -1;
            if ((ctr - smaller) > 0) {
              for (int j = 1; j <= smaller; j++)
                buffer[ctr - j] = buffer[ctr - j] * 0.4;
            }
            flip_ctr = ctr;
          }
        }
      }
    }
  }
  #ifdef DEBUG_LOGGING
  //	printf("\nValue of ctr after looping: %d Buffer Len: %d\n", ctr, buffSize);
  //	printf("\ctr/samples = %d ctr/(samples*10) = %d\n\n", ctr/samples, ctr/(samples*10));
  #endif

  //int error = 0;
  // int count;
  //  for (count = 0; count < dataLen; count++) {
  //      printf("%02X", b[count]);
  //  }
  //  printf("\n");

  // socket write

  socket_send(ctr);
/*	
  if (!socket_open && transmit) {
    printf("Opening socket!\n");
 //   struct sockaddr_in address;
 //   int valread;
    struct sockaddr_in serv_addr;
    //    char *hello = "Hello from client"; 
    //    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("\n Socket creation error \n");
      error = 1;
    }

    memset( & serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if (inet_pton(AF_INET, "127.0.0.1", & serv_addr.sin_addr) <= 0) {
      printf("\nInvalid address/ Address not supported \n");
      error = 1;
    }

    if (connect(sock, (struct sockaddr * ) & serv_addr, sizeof(serv_addr)) < 0) {
      printf("\nConnection Failed \n");
      printf("Error: %s\n", strerror(errno));
      error = 1;
//  	FILE * transmit_restartf2 = popen("sudo systemctl restart transmit", "r");
//  	pclose(transmit_restartf2);	      
//        sleep(10);  // was 5 // sleep if socket connection refused

    // try again
      error = 0;
      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        error = 1;
      }

      memset( & serv_addr, '0', sizeof(serv_addr));

      serv_addr.sin_family = AF_INET;
      serv_addr.sin_port = htons(PORT);

      // Convert IPv4 and IPv6 addresses from text to binary form 
      if (inet_pton(AF_INET, "127.0.0.1", & serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        error = 1;
      }

      if (connect(sock, (struct sockaddr * ) & serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        printf("Error: %s\n", strerror(errno));
        error = 1;
//  	  FILE * transmit_restartf = popen("sudo systemctl restart transmit", "r");
//  	  pclose(transmit_restartf);	      
//          sleep(10);  // was 5 // sleep if socket connection refused
      }	    
    }
    if (error == 1) {
      printf("Socket error count: %d\n", error_count);  	    
//    ; //transmitStatus = -1;
      if (error_count++ > 5) { 
	  printf("Restarting transmit\n");    
  	  FILE * transmit_restartf = popen("sudo systemctl restart transmit", "r");
  	  pclose(transmit_restartf);	      
          sleep(10);  // was 5 // sleep if socket connection refused
      }	    
    }
    else {
      socket_open = 1;
      error_count = 0;	    
    }
  }

  if (!error && transmit) {
    //	digitalWrite (0, LOW);
 //   printf("Sending %d buffer bytes over socket after %d ms!\n", ctr, (long unsigned int)millis() - start);
    start = millis();
    int sock_ret = send(sock, buffer, (unsigned int)(ctr * 2 + 2), 0);
//    printf("socket send 1 %d ms bytes: %d \n\n", (unsigned int)millis() - start, sock_ret);
    fflush(stdout);	  
    
    if (sock_ret < (ctr * 2 + 2)) {
  //    printf("Not resending\n");
      sleep(0.5);
      sock_ret = send(sock, &buffer[sock_ret], (unsigned int)(ctr * 2 + 2 - sock_ret), 0);
//      printf("socket send 2 %d ms bytes: %d \n\n", millis() - start, sock_ret);
    }
*/	  
	  
    loop_count++;	  
    if ((firstTime == 1) || (((loop_count % 180) == 0) && (mode == FSK)) || (((loop_count % 80) == 0) && (mode == BPSK))) // do first time and was every 180 samples
    {
      int max;
      if (mode == FSK)
	      if (sim_mode)
 		max = 6;
              else if (firstTime == 1)
	      	max = 4;  // 5; // was 6
              else
		max = 3;
      else  
 	      if (firstTime == 1)
	      	max = 5;  // 5; // was 6
              else
		max = 4;  
	    
      for (int times = 0; times < max; times++) 	    
      {
///	      start = millis();  // send frame until buffer fills
	      socket_send(ctr);
///	      sock_ret = send(sock, buffer, (unsigned int)(ctr * 2 + 2), 0);
//	      printf("socket send %d in %d ms bytes: %d \n\n",times + 2, (unsigned int)millis() - start, sock_ret);
	      
///	      if ((millis() - start) > 500) {
///		      printf("Buffer over filled!\n");
///		      break;
///	      }

///	      if (sock_ret < (ctr * 2 + 2)) {
	  //    printf("Not resending\n");
///		sleep(0.5);
///		sock_ret = send(sock, &buffer[sock_ret], (unsigned int)(ctr * 2 + 2 - sock_ret), 0);
///		printf("socket resend %d in %d ms bytes: %d \n\n",times, millis() - start, sock_ret);
///	      }
      }
      sampleTime = (unsigned int) millis(); // resetting time for sleeping
 //     fflush(stdout);
//      if (firstTime == 1)
//	      max -= 1;
    }

///    if (sock_ret == -1) {
///      printf("Error: %s \n", strerror(errno));
///      socket_open = 0;
      //transmitStatus = -1;
///    }
///  }
  if (!transmit) {
    fprintf(stderr, "\nNo CubeSatSim Band Pass Filter detected.  No transmissions after the CW ID.\n");
    fprintf(stderr, " See http://cubesatsim.org/wiki for info about building a CubeSatSim\n\n");
  }

///  if (socket_open == 1)	
///    firstTime = 0;
//  else if (frames_sent > 0) //5)
//    firstTime = 0;

  return;
}

// code by https://stackoverflow.com/questions/25161377/open-a-cmd-program-with-full-functionality-i-o/25177958#25177958

FILE *sopen(const char *program)
    {
        int fds[2];
        pid_t pid;

        if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) < 0)
            return NULL;

        switch(pid=vfork()) {
        case -1:    /* Error */
            close(fds[0]);
            close(fds[1]);
            return NULL;
        case 0:     /* child */
            close(fds[0]);
            dup2(fds[1], 0);
            dup2(fds[1], 1);
            close(fds[1]);
            execl("/bin/sh", "sh", "-c", program, NULL);
            _exit(127);
        }
        /* parent */
        close(fds[1]);
        return fdopen(fds[0], "r+");
    }

 /*
 * TelemEncoding.h
 *
 *  Created on: Feb 3, 2014
 *      Author: fox
 */

/* 
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define false 0
#define true 1
*/
void write_wave(int i, short int *buffer)
{
		if (mode == FSK)
		{
			if ((ctr - flip_ctr) < smaller)
				buffer[ctr++] = (short int)(0.1 * phase * (ctr - flip_ctr) / smaller);
			else
				buffer[ctr++] = (short int)(0.25 * amplitude * phase);
		}
		else
		{
			if ((ctr - flip_ctr) < smaller) {
//  		 		buffer[ctr++] = (short int)(amplitude * 0.4 * phase * sin((float)(2*M_PI*i*freq_Hz/S_RATE)));	
  		 		buffer[ctr++] = (short int)(phase * sin_map[ctr % sin_samples] / 2);
//				if (ctr < 1000) printf("*");
			}
		else
//  		 		buffer[ctr++] = (short int)(amplitude * 0.4 * phase * sin((float)(2*M_PI*i*freq_Hz/S_RATE)));
  		 		buffer[ctr++] = (short int)(phase * sin_map[ctr % sin_samples]); 
		} 			
//		if (ctr < 1000) printf("%d %d %d \n", ctr, i, buffer[ctr - 1]);

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

int test_i2c_bus(int bus)
{
	int output = bus; // return bus number if OK, otherwise return -1
	char busDev[20] = "/dev/i2c-";
	char busS[5];
	snprintf(busS, 5, "%d", bus);  
	strcat (busDev, busS);	
	printf("I2C Bus Tested: %s \n", busDev);
	
	if (access(busDev, W_OK | R_OK) >= 0)  {   // Test if I2C Bus is present
//	  	printf("bus is present\n\n");	    
    	  	char result[128];		
    	  	const char command_start[] = "timeout 2 i2cdetect -y ";  // was  5 10
		char command[50];
		strcpy (command, command_start);
    	 	strcat (command, busS);
//     	 	printf("Command: %s \n", command);
    	 	FILE *i2cdetect = popen(command, "r");
	
    	 	while (fgets(result, 128, i2cdetect) != NULL) {
    	 		;
//       	 	printf("result: %s", result);
    	 	}	
    	 	int error = pclose(i2cdetect)/256;
//      	 	printf("%s error: %d \n", &command, error);
    	 	if (error != 0) 
    	 	{	
    	 		printf("ERROR: %s bus has a problem \n  Check I2C wiring and pullup resistors \n", busDev);
			if (bus == 3)
				printf("-> If this is a CubeSatSim Lite, then this error is normal!\n");
			output = -1;
    		}								
	} else
	{
    	 	printf("ERROR: %s bus has a problem \n  Check software to see if I2C enabled \n", busDev);
		output = -1; 
	}
	return(output);	// return bus number or -1 if there is a problem with the bus
}

float toAprsFormat(float input) {
// converts decimal coordinate (latitude or longitude) to APRS DDMM.MM format	
    int dd = (int) input;
    int mm1 = (int)((input - dd) * 60.0);
    int mm2 = (int)((input - dd - (float)mm1/60.0) * 60.0 * 60.0);
    float output = dd * 100 + mm1 + (float)mm2 * 0.01;
    return(output);	
}

int get_payload_serial(int debug_camera)  {

  index1 = 0;
  flag_count = 0;
  start_flag_detected = FALSE;
  start_flag_complete = FALSE;
  end_flag_detected = FALSE;
  jpeg_start = 0;
	
  serialFlush (uart_fd);  // flush serial buffer so latest payload is read	
// #ifdef GET_IMAGE_DEBUG
 if (debug_camera)
  printf("Received from Payload:\n");
 // #endif
  finished = FALSE;

  unsigned long time_start = millis();	    
  while ((!finished) && ((millis() - time_start) < CAMERA_TIMEOUT)) {
	  
    if (serialDataAvail(uart_fd)) {
	      char octet = (char) serialGetchar(uart_fd);
              printf("%c", octet);
              fflush(stdout);	  

     if (start_flag_complete) {
//       printf("Start flag complete detected\n");
       buffer2[index1++] = octet;
       if (octet == end_flag[flag_count]) {  // looking for end flag
//         if (end_flag_detected) {
            flag_count++;
#ifdef GET_IMAGE_DEBUG  
//       if (debug_camera)  
            printf("Found part of end flag!\n");
#endif
            if (flag_count >= strlen(end_flag)) {  // complete image           
              index1 -= strlen(end_flag);
	      buffer2[index1++] = 0;
	      printf(" Payload length: %d \n",index1); 	    

//              write_jpg();
	      finished = TRUE;	    
              index1 = 0;           
              start_flag_complete = FALSE;
              start_flag_detected = FALSE; // get ready for next image 
              end_flag_detected = FALSE;
              flag_count = 0; 
//              delay(6000);
            }
         } else {
	   if (flag_count > 1)    
             printf("Resetting. Not end flag.\n");    	       
           flag_count = 0;
	       
         }
 ///        buffer2[index1++] = octet;
           
//#ifdef GET_IMAGE_DEBUG 
    if (debug_camera) {   
           char hexValue[5];
           if (octet != 0x66) {
             sprintf(hexValue, "%02X", octet);
//             printf(hexValue);
           } else {
//             Serial.println("\n********************************************* Got a 66!");
             printf("66");
           } 
//             Serial.write(octet);
    }
//#endif             
           if (index1 > 1000) {
             index1 = 0; 
	     printf("Resetting index!\n");	   
	   }
//         }
    } else if (octet == start_flag[flag_count]) {  // looking for start flag
          start_flag_detected = TRUE;
          flag_count++;
#ifdef GET_IMAGE_DEBUG  
          printf("Found part of start flag! \n");
#endif  
          if (flag_count >= strlen(start_flag)) {
            flag_count = 0;
            start_flag_complete = TRUE;
#ifdef GET_IMAGE_DEBUG  
            printf("Found all of start flag!\n");        
#endif  
          }
      } else {  // not the flag, keep looking
          start_flag_detected = FALSE;
          flag_count = 0;
#ifdef GET_IMAGE_DEBUG  
          printf("Resetting. Not start flag.\n");        
#endif  
       } 
    }
//    Serial.println("writing to Serial2");
  }
  if (debug_camera)                 
      printf("\nComplete\n");
  fflush(stdout);	
  return(finished);
}

void program_radio() {
// if (sr_frs_present) {	
  printf("Programming FM module!\n");	

  pinMode(28, OUTPUT);	
  pinMode(29, OUTPUT);
  digitalWrite(29, HIGH);  // enable SR_FRS
  digitalWrite(28, HIGH);  // stop transmit	
	
if ((uart_fd = serialOpen("/dev/ttyAMA0", 9600)) >= 0) {  // was 9600
  printf("serial opened 9600\n");  
  for (int i = 0; i < 5; i++) {
     sleep(1); // delay(500);
//#ifdef APRS_VHF
//     char vhf_string[] = "AT+DMOSETGROUP=0,144.3900,144.3900,0,3,0,0\r\n";	
//     serialPrintf(uart_fd, vhf_string);	
//     mySerial.println("AT+DMOSETGROUP=0,144.3900,144.3900,0,3,0,0\r");    // can change to 144.39 for standard APRS	  
//    mySerial.println("AT+DMOSETGROUP=0,145.0000,145.0000,0,3,0,0\r");    // can change to 145 for testing ASPRS	  
//#else
     char uhf_string[] = "AT+DMOSETGROUP=0,435.0000,434.9000,0,3,0,0\r\n";
     char uhf_string1a[] = "AT+DMOSETGROUP=0,";	// changed frequency to verify
     char comma[] = ",";
     char uhf_string1b[] = ",0,";	// changed frequency to verify
     char uhf_string1[] = "AT+DMOSETGROUP=0,435.0000,434.9000,0,";	// changed frequency to verify
     char uhf_string2[] = ",0,0\r\n";	
     char sq_string[2];
     sq_string[0] = '0' + squelch;
     sq_string[1] = 0;
	  
//     serialPrintf(uart_fd, uhf_string);	
/**/	  
	serialPrintf(uart_fd, uhf_string1a);	
	serialPrintf(uart_fd, rx);	
	serialPrintf(uart_fd, comma);	
	serialPrintf(uart_fd, tx);	
	serialPrintf(uart_fd, uhf_string1b);	
	serialPrintf(uart_fd, sq_string);	
	serialPrintf(uart_fd, uhf_string2);	
/**/	  
	  
//     mySerial.println("AT+DMOSETGROUP=0,435.1000,434.9900,0,3,0,0\r");   // squelch set to 3
//#endif	  
   sleep(1);
   char mic_string[] = "AT+DMOSETMIC=8,0\r\n";  
   serialPrintf(uart_fd, mic_string);
//   mySerial.println("AT+DMOSETMIC=8,0\r");  // was 8
	
  }
 }
//#ifdef APRS_VHF	  	
// printf("Programming FM tx 144.39, rx on 144.39 MHz\n");
//#else
 printf("Programming FM tx 434.9, rx on 435.0 MHz\n");
//#endif	
//  digitalWrite(PTT_PIN, LOW);  // transmit carrier for 0.5 sec
//  sleep(0.5);
//  digitalWrite(PTT_PIN, HIGH);	
  digitalWrite(29, LOW);  // disable SR_FRS	
  pinMode(28, INPUT);
  pinMode(29, INPUT);

  serialClose(uart_fd);	
}

int battery_saver_check() {
	FILE *file = fopen("/home/pi/CubeSatSim/battery_saver", "r");
	if (file == NULL) {
//		fprintf(stderr,"Battery saver mode is OFF!\n");
		return(OFF);
	} 
	fclose(file);
//        fprintf(stderr, "Safe Mode!\n");
//	fprintf(stderr,"Battery saver mode is ON!\n");
	return(ON);
}

void battery_saver(int setting) {
if (setting == ON) {
	if (battery_saver_check() == OFF) {
		FILE *command = popen("touch /home/pi/CubeSatSim/battery_saver", "r");
		pclose(command);
		fprintf(stderr,"Turning Safe Mode ON\n"); 
		fprintf(stderr,"Turning Battery saver mode ON\n"); 
		battery_saver_mode = ON;
		if ((mode == AFSK) || (mode == SSTV) || (mode == CW)) {
			command = popen("echo 'reboot due to turning ON Safe Mode!' | wall", "r");
			pclose(command);
			command = popen("sudo reboot now", "r");
		  	pclose(command);
			sleep(60);
			return;  
		}
//		} else
//			fprintf(stderr, "Nothing to do for battery_saver\n");
	}  
  } else if (setting == OFF) {
	if (battery_saver_check() == ON) {
		FILE *command = popen("rm /home/pi/CubeSatSim/battery_saver", "r");
		pclose(command);
		fprintf(stderr,"Turning Battery saver mode OFF\n"); 
		battery_saver_mode = OFF;
		if ((mode == AFSK) || (mode == SSTV) || (mode == CW)) {
			command = popen("echo 'reboot due to turning OFF Safe Mode!' | wall", "r");
			pclose(command);
			command = popen("sudo reboot now", "r");
		  	pclose(command);
			sleep(60);
			return; 
		}
//		} else
//			fprintf(stderr, "Nothing to do for battery_saver\n");
	}  
  } else {
	  fprintf(stderr,"battery_saver function error");
	  return;
  }
  return;
}

void get_tlm_fc() {  // FUNcube Mode telemetry generation

//# define FC_EM
//#define JY_1
#define FC_SIM	
	
	/* create data, stream, and waveform buffers */

  	unsigned char source_bytes[256];
	int byte_count = 256;

	/* write telemetry into data buffer */

//	printf("\nBLOCKSIZE = %d\n", BLOCKSIZE);
//	printf("\nSYMPBLOCK = %d\n", SYMPBLOCK);

	memset(source_bytes, 0x00, sizeof(source_bytes));
#ifdef FC_EM	
	source_bytes[0] = 0b00000001 ;	// Sat Id is FUNcube-EM
#endif
#ifdef JY_1	
//	source_bytes[0] = 0b11000001 ;    // Sat Id is extended, Frame 2 (RT2 + WO2)
	source_bytes[0] = 0xE0 | 0x20 | 0x00; // 1;    // Sat Id is extended, Frame 34 (RT2 + IMG2)

	source_bytes[0] = source_bytes[0] | ( 0x01 & (uint8_t)(sequence % 2));  // alternate last bit for RT1, RT2.

	//	source_bytes[1] = 0x08 ; // extended Nayify - works per code
	source_bytes[1] = 0x10 ; // extended JY-1 - works, no documentation
	
	int extended = 1;
#endif
#ifdef FC_SIM
//	source_bytes[0] = 0b11000001 ;    // Sat Id is extended, Frame 2 (RT2 + WO2)
	source_bytes[0] = 0xE0 | 0x20 | 0x00; // 1;    // Sat Id is extended, Frame 34 (RT2 + IMG2)

	source_bytes[0] = source_bytes[0] | ( 0x01 & (uint8_t)(sequence % 2));  // alternate last bit for RT1, RT2.

	//	source_bytes[1] = 0x08 ; // extended Nayify - works per code
	source_bytes[1] = 0xfb ; // funcube sim sat id per AMSAT-UK allocation
	int extended = 1;
#endif	
#if defined(FC_SIM) || defined(JY_1)	
//	if (sequence > 10) {
		if (image_file == NULL)  {
			image_file = fopen("/home/pi/CubeSatSim/image_file.bin", "r");
			image_id++;
			printf("Opening file image_file.bin for image_id: %d\n", image_id);
		}
//	}
	int pos = FC_PAYLOAD + extended;	  
	int value;
	if (image_file != NULL) {
		printf("Writing image data to payload\n");
		while ((pos < 256) && ((value = getc(image_file)) != EOF)) {
			source_bytes[pos++] = value;
//			printf("%2x ", value);
		}
		if (value == EOF) {
			image_file = NULL;
			printf("End of file reached! Delete image_file.bin");
			FILE * delete_image = popen("sudo rm /home/pi/CubeSatSim/image_file.bin", "r");
  			pclose(delete_image); 
		}
	}
	
#endif	

//	printf("Volts: %f %f %f %f \n", voltage[map[BAT]], voltage[map[PLUS_X]] , voltage[map[PLUS_Y]], voltage[map[PLUS_Z]]); 
//	printf("AmpsPlus: %f %f %f %f \n", current[map[BAT]], current[map[PLUS_X]] , current[map[PLUS_Y]], current[map[PLUS_Z]]); 
//	printf("AmpsMinus: %f %f %f %f \n", current[map[BAT2]], current[map[MINUS_X]] , current[map[MINUS_Y]], current[map[MINUS_Z]]); 

	float xmax = (voltage[map[PLUS_X]] > voltage[map[MINUS_X]]) ? voltage[map[PLUS_X]] : voltage[map[MINUS_X]];
	float ymax = (voltage[map[PLUS_Y]] > voltage[map[MINUS_Y]]) ? voltage[map[PLUS_Y]] : voltage[map[MINUS_Y]];
	float zmax = (voltage[map[PLUS_Z]] > voltage[map[MINUS_Z]]) ? voltage[map[PLUS_Z]] : voltage[map[MINUS_Z]];

//	printf("Vmax: %f %f %f \n", xmax, ymax, zmax);
	
	uint16_t x = (uint16_t)(xmax * 1000) & 0x3fff;  // 14 bits
	uint16_t y = (uint16_t)(ymax * 1000) & 0x3fff;
	uint16_t z = (uint16_t)(zmax * 1000) & 0x3fff;
	uint16_t b = (uint16_t)(voltage[map[BAT]] * 1000) & 0x3fff;

	uint16_t ix = (uint16_t)((current[map[PLUS_X]] + current[map[MINUS_X]])) & 0x3ff;  // 10 bits
	uint16_t iy = (uint16_t)((current[map[PLUS_Y]] + current[map[MINUS_Y]])) & 0x3ff;
	uint16_t iz = (uint16_t)((current[map[PLUS_Z]] + current[map[MINUS_Z]])) & 0x3ff;

	uint16_t ic = 0; 
	uint16_t ib = 0;
	
	if (current[map[BAT]] < 0 ) 
		ic = (uint16_t)(current[map[BAT]] * (-1)) & 0x3ff;  // charging current
	else 
		ib = (uint16_t)(current[map[BAT]]) & 0x3ff;  // supplying current

//	printf("X %x Y %x Z %x B %x\n", x, y, z, b);
//	printf("iX %x iY %x iZ %x iB %x iC\n", ix, iy, iz, ib, ic);
	
#if defined(FC_SIM) || defined(JY_1)
	source_bytes[extended + FC_EPS + 0] = 0xff & (x >> 6);  // Vx
	source_bytes[extended + FC_EPS + 1] = 0xfc & (x << 2);
        source_bytes[extended + FC_EPS + 1] = source_bytes[extended + FC_EPS + 1] | (0x03 & (y >> 12));
	source_bytes[extended + FC_EPS + 2] = 0xff & (y >> 2);  // Vy
	source_bytes[extended + FC_EPS + 3] = 0xf0 & (y << 4);

	source_bytes[extended + FC_EPS + 3] = source_bytes[extended + FC_EPS + 3] | (0x0f & (z >> 10));
	source_bytes[extended + FC_EPS + 4] = 0xff & (z >> 2);  // Vz
	source_bytes[extended + FC_EPS + 5] = 0xc0 & (z << 6);
	
	source_bytes[extended + FC_EPS + 5] = source_bytes[extended + FC_EPS + 5] | (0x3f & (b >> 8));  
	source_bytes[extended + FC_EPS + 6] = 0xff & (b >> 0);  // Vb

	source_bytes[extended + FC_EPS + 7] = 0xff & (ix >> 2);   // ix
	source_bytes[extended + FC_EPS + 8] = 0xc0 & (iy << 6);   // iy

	source_bytes[extended + FC_EPS + 8] = source_bytes[extended + FC_EPS + 8] | (0x3f & (iy >> 4));  
	source_bytes[extended + FC_EPS + 9] = 0xf0 & (iy << 4);	

	source_bytes[extended + FC_EPS + 9] = source_bytes[extended + FC_EPS + 9] | (0x0f & (iz >> 6));  
	source_bytes[extended + FC_EPS + 10] = 0x3f & (iz << 2);  // iz

	source_bytes[extended + FC_EPS + 10] = source_bytes[extended + FC_EPS + 10] | (0x03 & (ic >> 8));  
	source_bytes[extended + FC_EPS + 11] = 0xff & (ic << 0);  // ic battery charging curent	

	source_bytes[extended + FC_EPS + 12] = 0xff & (ib >> 2);  // ib battery discharging current
	source_bytes[extended + FC_EPS + 13] = 0xc0 & (ib << 6);  

	source_bytes[extended + FC_EPS + 13] = source_bytes[extended + FC_EPS + 13] | 0x3f & (((unsigned long int)reset_count) >> 2);
	source_bytes[extended + FC_EPS + 14] = 0xff & (((unsigned long int)reset_count) << 6); // reset count

	uint8_t temp = (int)(other[IHU_TEMP] + 0.5);

	source_bytes[extended + FC_EPS + 17] = source_bytes[extended + FC_EPS + 17] | 0x3f & (temp >> 2);  // cpu temp
	source_bytes[extended + FC_EPS + 18] = 0xff & (temp << 6);

	source_bytes[extended + 48] = 0x0c;  // Antenna 1 and 2 deployed
	
	source_bytes[extended + 49] = 0xff & ((unsigned long int)sequence >> 16);  // sequence number
	source_bytes[extended + 50] = 0xff & ((unsigned long int)sequence >> 8);
	source_bytes[extended + 51] = 0xff & (unsigned long int)sequence++;

	uint16_t groundCommandCount = 0;
	FILE * command_count_file = fopen("/home/pi/CubeSatSim/command_count.txt", "r");
	if (command_count_file != NULL) {	
		char count_string[10];	
		if ( (fgets(count_string, 10, command_count_file)) != NULL)
			groundCommandCount = (uint16_t) atoi(count_string);     
	} else 
		printf("Error opening command_count.txt!\n");
	fclose(command_count_file);

//	source_bytes[extended + 52] = 0xfc & (groundCommandCount << 2);  // command doesn't work

	source_bytes[extended + 53] = 0x0f;  // SW valid 
	source_bytes[extended + 54] = 0xe0;  // SW valid

	if ((ix + iy + iz) < 4)
		source_bytes[extended + 54] = source_bytes[extended + 54] | 0x10;  // eclipse
	if (SafeMode == 1)
		source_bytes[extended + 54] = source_bytes[extended + 54] | 0x08;  // safe mode
#endif
	
#ifdef FC_EM
	source_bytes[FC_EPS + 0] = 0xff & (((unsigned int)((voltage[map[PLUS_X]] + voltage[map[MINUS_X]]) * 1000) >> 8));  // mV
	source_bytes[FC_EPS + 1] = 0xff & ((unsigned int)((voltage[map[PLUS_X]] + voltage[map[MINUS_X]]) * 1000));
	source_bytes[FC_EPS + 2] = 0xff & (((unsigned int)((voltage[map[PLUS_Y]] + voltage[map[MINUS_Y]]) * 1000) >> 8));  // mV
	source_bytes[FC_EPS + 3] = 0xff & ((unsigned int)((voltage[map[PLUS_Y]] + voltage[map[MINUS_Y]]) * 1000));
	source_bytes[FC_EPS + 4] = 0xff & (((unsigned int)((voltage[map[PLUS_Z]] + voltage[map[MINUS_Z]]) * 1000) >> 8));  // mV
	source_bytes[FC_EPS + 5] = 0xff & ((unsigned int)((voltage[map[PLUS_Z]] + voltage[map[MINUS_Z]]) * 1000));
	unsigned int total_solar_current = (unsigned int) (current[map[PLUS_X]] + current[map[MINUS_X]] 
				+ current[map[PLUS_Y]] + current[map[MINUS_Y]] 
				+ current[map[PLUS_Z]] + current[map[MINUS_Z]]);
	source_bytes[FC_EPS + 6] = 0xff & total_solar_current >> 8;
	source_bytes[FC_EPS + 7] = 0xff & total_solar_current;		
	source_bytes[FC_EPS + 8] = 0xff & (((unsigned int)(voltage[map[BAT]] * 1000) >> 8));  // mV
	source_bytes[FC_EPS + 9] = 0xff & ((unsigned int)(voltage[map[BAT]] * 1000));
	source_bytes[FC_EPS + 10] = 0xff & (((unsigned int)(current[map[BAT]] * 1) >> 8));  // mA
	source_bytes[FC_EPS + 11] = 0xff & ((unsigned int)(current[map[BAT]] * 1));
	source_bytes[FC_EPS + 12] = 0xff & (((unsigned long int)reset_count >> 8));
	source_bytes[FC_EPS + 13] = 0xff & ((unsigned long int)reset_count);
	
	source_bytes[FC_SW + 0] = 0xff & ((unsigned long int)sequence >> 16); // Sequence number
	source_bytes[FC_SW + 1] = 0xff & ((unsigned long int)sequence >> 8);
	source_bytes[FC_SW + 2] = 0xff & (unsigned long int)sequence++;
		
#endif

/**/
	printf("\nsource_bytes\n");
	for (int i=0; i<256; i++)
		printf("%x ", source_bytes[i]);
	printf("\n\n");
/**/

	/* convert data buffer into stream buffer */

	const unsigned char* encoded_bytes = encode(source_bytes, byte_count);
/*
	printf("\nencoded_bytes\n");
	for (int i=0; i<5200; i++)
		printf("%d", encoded_bytes[i]);
	printf("\n\n");
*/	
	/* convert to waveform buffer */

    int data;
    int val;
    int i;	
    ctr = 0;
    int symbol = 0;
    smaller = (int) (S_RATE / (2 * freq_Hz));
//    printf("\n\nsmaller = %d \n\n",smaller);	
	
    for (i = 1; i <= headerLen * samples; i++) {
      write_wave(ctr, buffer);	
      if ((i % samples) == 0) {
	phase *= -1;
        if ((ctr - smaller) > 0) {
		int j;
        	for (j = 1; j <= smaller; j++) {
                	buffer[ctr - j] = buffer[ctr - j] * 0.5;
//			if (ctr < 1000) printf("# %d %d\n", ctr - j, buffer[ctr - j]);
		}
        }
        flip_ctr = ctr;
      }
    }
	
    for (i = 1; i <= syncBits * samples; i++) {
      write_wave(ctr, buffer);
      //		printf("%d ",ctr);
      if ((i % samples) == 0) {
        int bit = syncBits - i / samples + 1;
        val = syncWord;
        data = val & 1 << (bit - 1);
//           	printf ("--- %d i: %d sync bit %d = %d \n",
//          		 ctr, i, bit, (data > 0) );        
        if (data == 0) {
            phase *= -1;
            if ((ctr - smaller) > 0) {
	      int j;
              for (j = 1; j <= smaller; j++)
                buffer[ctr - j] = buffer[ctr - j] * 0.5;
            }
            flip_ctr = ctr;
        }
      }
    }
	
    for (i = 1; i <= (dataLen * samples); i++) // 5200   
    {
      write_wave(ctr, buffer);
      if ((i % samples) == 0) {
        symbol = i / samples - 1;
//	if (i < 100) printf("symbol = %d\n",symbol);      
	data = encoded_bytes[symbol];      
	if (data == 0) {
	   phase *= -1;
	   if ((ctr - smaller) > 0) {
	     int j;   
	     for (j = 1; j <= smaller; j++) {
		buffer[ctr - j] = buffer[ctr - j] * 0.5;
//		if (ctr < 1000) printf("# %d %d\n", ctr - j, buffer[ctr - j]);
	     }
           }
	   flip_ctr = ctr;
	}
      }
    }
// printf("symbol = %d\n",symbol);
// printf("\nctr = %d\n\n", ctr);

//  socket_send((((headerLen + syncBits + dataLen) * samples) * 2) + 2);
  socket_send(ctr);
	
  if (!transmit) {
    fprintf(stderr, "\nNo CubeSatSim Band Pass Filter detected.  No transmissions after the CW ID.\n");
    fprintf(stderr, " See http://cubesatsim.org/wiki for info about building a CubeSatSim\n\n");
  }

  int startSleep = millis();	    
  if ((millis() - sampleTime) < ((unsigned int)frameTime)) // - 750 + pi_zero_2_offset))  
        sleep(1.0); 
  while ((millis() - sampleTime) < ((unsigned int)frameTime)) // - 750 + pi_zero_2_offset))  
        sleep(0.1); 
  printf("Start sleep %d Sleep period: %d  while period: %d\n", startSleep, millis() - startSleep, millis() - sampleTime);
  sampleTime = (unsigned int) millis(); // resetting time for sleeping
  fflush(stdout);
}

void socket_send(int length) {

  printf("Socket_send!\n");	
  int error = 0;

  if (!socket_open && transmit) { // open socket if not open
    printf("Opening socket!\n");
 //   struct sockaddr_in address;
 //   int valread;
    struct sockaddr_in serv_addr;
    //    char *hello = "Hello from client"; 
    //    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("\n Socket creation error \n");
      error = 1;
    }

    memset( & serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if (inet_pton(AF_INET, "127.0.0.1", & serv_addr.sin_addr) <= 0) {
      printf("\nInvalid address/ Address not supported \n");
      error = 1;
    }

    if (connect(sock, (struct sockaddr * ) & serv_addr, sizeof(serv_addr)) < 0) {
      printf("\nConnection Failed \n");
      printf("Error: %s\n", strerror(errno));
      error = 1;

    // try again
      error = 0;
      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        error = 1;
      }

      memset( & serv_addr, '0', sizeof(serv_addr));

      serv_addr.sin_family = AF_INET;
      serv_addr.sin_port = htons(PORT);

      // Convert IPv4 and IPv6 addresses from text to binary form 
      if (inet_pton(AF_INET, "127.0.0.1", & serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        error = 1;
      }

      if (connect(sock, (struct sockaddr * ) & serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        printf("Error: %s\n", strerror(errno));
        error = 1;
      }	    
    }
    if (error == 1) {
      printf("Socket error count: %d\n", error_count);  	    
//    ; //transmitStatus = -1;
      if (error_count++ > 5) { 
	  printf("Restarting transmit\n");    
  	  FILE * transmit_restartf = popen("sudo systemctl restart transmit", "r");
  	  pclose(transmit_restartf);	      
          sleep(10);  // was 5 // sleep if socket connection refused
      }	    
    }
    else {
      socket_open = 1;
      error_count = 0;	    
    }
  }

/* write waveform buffer over socket */

//  int length = (((headerLen + syncBits + dataLen) * samples) * 2) + 2;	// ctr * 2 + 2 like bpsk due to 2 bytes per sample.
  length = length * 2 + 2;  // convert from samples to bytes
//  printf("length in bytes: %d\n", length);	

  if (!error && transmit) {
 //   printf("Sending %d buffer bytes over socket after %d ms!\n", ctr, (long unsigned int)millis() - start);
    start = millis();
    int sock_ret = send(sock, buffer, length, 0);
    printf("socket send 1 %d ms bytes: %d \n\n", (unsigned int)millis() - start, sock_ret);
    fflush(stdout);	  
    
    if (sock_ret < length) {
  //    printf("Not resending\n");
      sleep(0.5);
      sock_ret = send(sock, &buffer[sock_ret], length - sock_ret, 0);
//      printf("socket send 2 %d ms bytes: %d \n\n", millis() - start, sock_ret);
    }
	  
//    loop_count++;	  

    if (sock_ret == -1) {
      printf("Error: %s \n", strerror(errno));
      socket_open = 0;
    }
  }

/*	
  if (!transmit) {
    fprintf(stderr, "\nNo CubeSatSim Band Pass Filter detected.  No transmissions after the CW ID.\n");
    fprintf(stderr, " See http://cubesatsim.org/wiki for info about building a CubeSatSim\n\n");
  }

  int startSleep = millis();	    
  if ((millis() - sampleTime) < ((unsigned int)frameTime)) // - 750 + pi_zero_2_offset))  
        sleep(1.0); 
  while ((millis() - sampleTime) < ((unsigned int)frameTime)) // - 750 + pi_zero_2_offset))  
        sleep(0.1); 
  printf("Start sleep %d Sleep period: %d  while period: %d\n", startSleep, millis() - startSleep, millis() - sampleTime);
  sampleTime = (unsigned int) millis(); // resetting time for sleeping
  fflush(stdout);
	*/
	
  if (socket_open == 1)	
    firstTime = 0;
}
