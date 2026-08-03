#include <Arduino.h>

String in;

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("Ready to take input");
}

void loop() {
  while (Serial.available()){
    in = Serial.readString();
    Serial.println(in);
  }
}