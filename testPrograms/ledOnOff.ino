#define LED 2
#define BUTTON 11
int lastState = LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, lastState);
  if (digitalRead(BUTTON) == lastState){
    lastState = !lastState;
    if (lastState == HIGH){
      Serial.println("On");
    } else Serial.println("Off");
  }
}
