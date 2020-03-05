  int counter = 0;
  
  void setup() {
  Serial1.begin(9600);
  pinMode(PC13, OUTPUT);
    digitalWrite(PC13, LOW);   // turn the LED on 
    delay(50);              // wait for a second
    digitalWrite(PC13, HIGH);    // turn the LED off
   
}

void loop() {

  if (Serial1.available() > 0) {
    digitalWrite(PC13, LOW);   // turn the LED on
    delay(50);              // wait for a second
    digitalWrite(PC13, HIGH);    // turn the LED off
    char result = Serial1.read();
//    Serial1.println(result);
    Serial1.println("OK");
//  Serial1.println(counter++);    
  }
  
  delay(100);
}
