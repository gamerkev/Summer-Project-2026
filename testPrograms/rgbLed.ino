#define RED 2
#define GREEN 3
#define BLUE 4
#define INPUT 39
int colour = 0;
int lastState = HIGH;

void setup() {
  // put your setup code here, to run once:
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(INPUT, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(INPUT) == LOW && lastState == HIGH){
    lastState = !lastState;
    colour = (colour + 1) % 4;
    Serial.println(colour);
    changeColour(colour);
  } else if (digitalRead(INPUT) == HIGH && lastState == LOW){
    lastState = !lastState;
  }
}

void changeColour(int colour){
  if (colour == 0){
    digitalWrite(RED, 0);
    digitalWrite(GREEN, 0);
    digitalWrite(BLUE, 0);
    Serial.println("Off");
  }
  if (colour == 1){
    digitalWrite(RED, 255);
    digitalWrite(GREEN, 0);
    digitalWrite(BLUE, 0);
    Serial.println("Red");
  }
  if (colour == 2){
    digitalWrite(RED, 0);
    digitalWrite(GREEN, 255);
    digitalWrite(BLUE, 0);
    Serial.println("Green");
  }
  if (colour == 3){
    digitalWrite(RED, 0);
    digitalWrite(GREEN, 0);
    digitalWrite(BLUE, 255);
    Serial.println("Blue");
  }
}