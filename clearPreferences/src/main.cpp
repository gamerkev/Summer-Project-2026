#include <Arduino.h>
#include <Preferences.h>

Preferences preferences;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(2000);
  preferences.begin("netCreds", false);
  preferences.clear();
  preferences.end();
  Serial.println(preferences.freeEntries());
}

void loop() {
  // put your main code here, to run repeatedly:
}
