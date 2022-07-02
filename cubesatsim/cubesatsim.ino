/*
 *  Transmits CubeSat Telemetry at 434.9MHz in AFSK, FSK, or CW format
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// This code is an Arduino sketch for the Raspberry Pi Pico
// based on the Raspberry Pi Code

#include "cubesatsim.h"

void setup() {

  Serial.begin(9600);
  
  Serial.println("Pico v0.1 starting...\n\n");


// set all Pico GPIO pins to input
  
// detect Pi Zero using 3.3V
  
// if Pi is present, run Payload OK software
  
// otherwise, run CubeSatSim Pico code
  
  Serial.println("\n\nCubeSatSim Pico v0.1 starting...\n\n");
  
  mode = FSK;
  frameCnt = 1;
  
  Serial.println("v1 Present with UHF BPF\n");
  txLed = 2;
  txLedOn = HIGH;
  txLedOff = LOW;
  vB5 = TRUE;
  onLed = 27;
  onLedOn = HIGH;
  onLedOff = LOW;
  transmit = TRUE;  
  
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

//    printf("\n FSK Mode, %d bits per frame, %d bits per second, %d ms per frame, %d ms sample period\n",
//      bufLen / (samples * frameCnt), bitRate, frameTime, samplePeriod);
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

//    printf("\n BPSK Mode, bufLen: %d,  %d bits per frame, %d bits per second, %d ms per frame %d ms sample period\n",
//      bufLen, bufLen / (samples * frameCnt), bitRate, frameTime, samplePeriod);

    sin_samples = S_RATE/freq_Hz;	 		
  //      printf("Sin map: ");	 		
    for (int j = 0; j < sin_samples; j++) {	 		
      sin_map[j] = (short int)(amplitude * sin((float)(2 * M_PI * j / sin_samples)));	 		
  //	printf(" %d", sin_map[j]);	 		
//   }	 		
      printf("\n");
   }
  
// program Transceiver board  
  
}
}

void loop() {
  
  loop_count++;
	
  // query INA219 sensors and Payload sensors
  
  // encode as digits (APRS or CW mode) or binary (DUV FSK)
  
  // send telemetry
  
  // delay some time
  
}

void get_tlm_fox() {
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
  memset(buffer, 0xa5, sizeof(buffer));
  short int rs_frame[rsFrames][223];
  unsigned char parities[rsFrames][parityLen], inputByte;
  int id, frm_type = 0x01, NormalModeFailure = 0, groundCommandCount = 0;
  int PayloadFailure1 = 0, PayloadFailure2 = 0;
  int PSUVoltage = 0, PSUCurrent = 0, Resets = 0, Rssi = 2048;
  int batt_a_v = 0, batt_b_v = 0, batt_c_v = 0, battCurr = 0;
  int posXv = 0, negXv = 0, posYv = 0, negYv = 0, posZv = 0, negZv = 0;
  int posXi = 0, negXi = 0, posYi = 0, negYi = 0, posZi = 0, negZi = 0;
  int head_offset = 0;
  short int buffer_test[bufLen];
  int buffSize;
  buffSize = (int) sizeof(buffer_test);
	
  if (mode == FSK)
    id = 7;
  else
    id = 0; // 99 in h[6]
	
  //  for (int frames = 0; frames < FRAME_CNT; frames++) 
  for (int frames = 0; frames < frameCnt; frames++) {
  
    if (firstTime != ON) {
      // delay for sample period
/**/
//      while ((millis() - sampleTime) < (unsigned int)samplePeriod)
     int startSleep = millis();	    
     if ((millis() - sampleTime) < ((unsigned int)frameTime - 250))  // was 250 100 500 for FSK
        sleep(2.0); // 0.5);  // 25);  // initial period
     while ((millis() - sampleTime) < ((unsigned int)frameTime - 250))  // was 250 100
        sleep(0.1); // 25); // 0.5);  // 25);
//        sleep((unsigned int)sleepTime);
/**/
      printf("Sleep period: %d\n", millis() - startSleep);
      fflush(stdout);
      
      sampleTime = (unsigned int) millis();
    } else
      printf("first time - no sleep\n");
	
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
	      if (loop_count % 32 == 0) {  // was 8  /// was loop now loop_count
		printf("Sending MIN frame \n");
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
		printf("Sending MAX frame \n");
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
    posXi = (int)(current[mapping[PLUS_X]] + 0.5) + 2048;
    posYi = (int)(current[mapping[PLUS_Y]] + 0.5) + 2048;
    posZi = (int)(current[mapping[PLUS_Z]] + 0.5) + 2048;
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
    PSUVoltage = (int)(voltage[map[BUS]] * 100);
    PSUCurrent = (int)(current[map[BUS]] + 0.5) + 2048;
    if (payload == ON)
      STEMBoardFailure = 0;
    // read payload sensor if available
    encodeA(b, 0 + head_offset, batt_a_v);
    encodeB(b, 1 + head_offset, batt_b_v);
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
      encodeA(b_max, 30 + head_offset, (int)(voltage_max[map[BUS]] * 100));
      encodeB(b_max, 46 + head_offset, (int)(current_max[map[BUS]] + 0.5) + 2048);
	    
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
      encodeA(b_min, 30 + head_offset, (int)(voltage_min[map[BUS]] * 100));
      encodeB(b_min, 46 + head_offset, (int)(current_min[map[BUS]] + 0.5) + 2048);
	    
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
    int status = STEMBoardFailure + SafeMode * 2 + sim_mode * 4 + PayloadFailure1 * 8 +
      (i2c_bus0 == OFF) * 16 + (i2c_bus1 == OFF) * 32 + (i2c_bus3 == OFF) * 64 + (camera == OFF) * 128 + groundCommandCount * 256;
    encodeA(b, 51 + head_offset, status);
    encodeB(b, 52 + head_offset, rxAntennaDeployed + txAntennaDeployed * 2);
    if (txAntennaDeployed == 0) {
      txAntennaDeployed = 1;
      printf("TX Antenna Deployed!\n");
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
///    #ifdef DEBUG_LOGGING
    //	printf("\n\nValue of ctr after header: %d Buffer Len: %d\n\n", ctr, buffSize);
///    #endif
    for (i = 1; i <= (10 * (headerLen + dataLen * payloads + rsFrames * parityLen) * samples); i++) // 572   
    {
      write_wave(ctr, buffer);
      if ((i % samples) == 0) {
        int symbol = (int)((i - 1) / (samples * 10));
        int bit = 10 - (i - symbol * samples * 10) / samples + 1;
        val = data10[symbol];
        data = val & 1 << (bit - 1);
        //		printf ("%d i: %d new frame %d data10[%d] = %x bit %d = %d \n",
        //	    		 ctr/SAMPLES, i, frames, symbol, val, bit, (data > 0) );
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
}

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
		if ((ctr - flip_ctr) < smaller)
	//  		 		buffer[ctr++] = (short int)(amplitude * 0.4 * phase * sin((float)(2*M_PI*i*freq_Hz/S_RATE)));	 	  		 	buffer[ctr++] = (short int)(amplitude * 0.4 * phase * sin((float)(2*M_PI*i*freq_Hz/S_RATE)));	
			buffer[ctr++] = (short int)(phase * sin_map[ctr % sin_samples] / 2);
	else
	//  		 		buffer[ctr++] = (short int)(amplitude * 0.4 * phase * sin((float)(2*M_PI*i*freq_Hz/S_RATE)));	 	 		 	buffer[ctr++] = (short int)(amplitude * phase * sin((float)(2*M_PI*i*freq_Hz/S_RATE)));	
			buffer[ctr++] = (short int)(phase * sin_map[ctr % sin_samples]); 		 } 			
	//		printf("%d %d \n", i, buffer[ctr - 1]);

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

void sleep(int time) {

  int startSleep = millis();	    
  while ((millis() - startSleep) < time)
    delay(100);		
}
