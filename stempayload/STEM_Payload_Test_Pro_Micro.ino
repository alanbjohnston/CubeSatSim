/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc
  This example code is in the public domain.
  modified 8 May 2014
  by Scott Fitzgerald
  
  Modified by Roger Clark. www.rogerclark.net for Maple mini 25th April 2015 , where the LED is on PC13
  
  Added CubeSatSim Payload tests by Alan Johnston, KU2Y
  
 */

int sensorValue = 0;
int D9 = 9;
int D8 = 8;
// Calibration data for diode temperature sensor
float T1 = 25; // Temperature data point 1
float R1 = 373; // Reading data point 1
float T2 = 17; // Temperature data point 2
float R2 = 405; // Reading data point 2

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin PB1 as an output.
  pinMode(D9, OUTPUT);
  pinMode(D8, OUTPUT);

  Serial.begin(9600);
 }

// the loop function runs over and over again forever
void loop() {
  digitalWrite(D9, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(D8, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(D9, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(D8, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000); // wait for a second

  sensorValue = analogRead(A0);
//  Serial.println(sensorValue);
  sensorValue = analogRead(A1);
  
  float temp = T1 + (sensorValue - R1) *(T2 - T1)/(R2 - R1);
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  
  sensorValue = analogRead(A2);
//  Serial.println(sensorValue);  
}
