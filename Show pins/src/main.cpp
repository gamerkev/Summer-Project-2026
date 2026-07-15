#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(5000);
  Serial.print("SCK: "); Serial.println(SCK);
  Serial.print("MISO: "); Serial.println(MISO);
  Serial.print("MOSI: "); Serial.println(MOSI);
  Serial.print("SS: "); Serial.println(SS);
}

void loop() {
  // put your main code here, to run repeatedly:
}
