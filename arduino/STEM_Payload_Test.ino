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
  
  Added CubeSatSim Payload tests for STM32 and Pro Micro by Alan Johnston, KU2Y
  
 */

int sensorValue = 0;
int greenLED = 9;
int blueLED = 8;
// Calibration data for diode temperature sensor
float T1 = 25; // Temperature data point 1
float R1 = 373; // Reading data point 1
float T2 = 17; // Temperature data point 2
float R2 = 405; // Reading data point 2

// the setup function runs once when you press reset or power the board
void setup() {

#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)  
  // initialize digital pin PB1 as an output.
  pinMode(PB9, OUTPUT);
  pinMode(PB8, OUTPUT);
  pinMode(PA0, INPUT_ANALOG);
  pinMode(PA1, INPUT_ANALOG);
  pinMode(PA2, INPUT_ANALOG);

  T2 = 25; // Temperature data point 1
  R2 = 2111; // Reading data point 1
  T1 = 23; // Temperature data point 2
  R1 = 2097; // Reading data point 2
#endif

#if defined __AVR_ATmega32U4__
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED,OUTPUT);
#endif

  Serial.begin(9600);
 }

// the loop function runs over and over again forever
void loop() {
  ledWrite(greenLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  ledWrite(blueLED, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  ledWrite(greenLED, LOW);    // turn the LED off by making the voltage LOW
  ledWrite(blueLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000); // wait for a second

  sensorValue = readAnalog(0);
//  Serial.println(sensorValue);
  sensorValue = readAnalog(1);
  
  float temp = T1 + (sensorValue - R1) *(T2 - T1)/(R2 - R1);
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  
  sensorValue = readAnalog(2);
//  Serial.println(sensorValue);  
}

void ledWrite(int ledPin, bool state)
{
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)
  if (ledPin == greenLED)
    digitalWrite(PB9, state);
  else if (ledPin == blueLED)
    digitalWrite(PB8, state);    
#endif

#if defined __AVR_ATmega32U4__
  digitalWrite(ledPin, state);   
#endif  
}

int readAnalog(int pin)
{
  int value = 0;
#if defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4)
  if (pin == 0)
    value = analogRead(PA0);
  else if (pin == 1)
    value = analogRead(PA1);    
  else if (pin == 2)
    value = analogRead(PA2);   
#endif

#if defined __AVR_ATmega32U4__
  value = analogRead(pin);   
#endif  

  return(value);
}
