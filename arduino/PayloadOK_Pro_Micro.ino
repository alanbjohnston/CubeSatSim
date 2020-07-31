int counter = 0;
int RXLED = 17;  // The RX LED has a defined Arduino pin

void setup() {
  Serial1.begin(9600);

  digitalWrite(RXLED, LOW);   // set the RX LED ON
  TXLED0; //TX LED is not tied to a normally controlled pin so a macro is needed, turn LED OFF
  delay(50);              // wait for a second
  digitalWrite(RXLED, HIGH);   // set the RX LED ON
  TXLED0; //TX LED is not tied to a normally controlled pin so a macro is needed, turn LED OFF

}

void loop() {

  if (Serial.available() > 0) {
    digitalWrite(RXLED, LOW);   // set the RX LED ON
    TXLED0; //TX LED is not tied to a normally controlled pin so a macro is needed, turn LED OFF
    delay(50);              // wait for a second
    digitalWrite(RXLED, HIGH);   // set the RX LED ON
    TXLED0; //TX LED is not tied to a normally controlled pin so a macro is needed, turn LED OFF
    char result = Serial1.read();
    //    Serial1.println(result);
    Serial1.println("OK");
    //  Serial1.println(counter++);
  }

  delay(100);
}
