#include <Arduino.h>

const float max_volts = 3.3;
const float max_analog_steps = 4095;
  
void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Setup initializing");

  pinMode(25, INPUT);
  pinMode(34, OUTPUT);
  digitalWrite(34, HIGH);
}

void loop() {
  digitalWrite(34, HIGH);
  int a0_read = analogRead(25);
  

  
  Serial.print(a0_read * (max_volts / max_analog_steps));
  
  Serial.println("");

  delay(200);
}
