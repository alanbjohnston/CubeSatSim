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
  
  // query INA219 sensors and Payload sensors
  
  // encode as digits (APRS or CW mode) or binary (DUV FSK)
  
  // send telemetry
  
  // delay some time
  
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
