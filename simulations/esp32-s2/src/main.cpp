#include <Arduino.h>

// put function declarations here:
#define LED 2
#define BUTT 38
// bool on;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(BUTT, INPUT);
}

int lastState = HIGH;
void loop() {
  // put your main code here, to run repeatedly:
  while(true){
    int value = digitalRead(BUTT);
  if (lastState!=value){
    lastState=value;
  }
  if (value == HIGH)
  {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }
  }
  
}

// put function definitions here:
