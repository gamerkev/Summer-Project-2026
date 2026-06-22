#define bLife1 1  //For the lights that represent blue's lives
#define bLife2 2
#define bLife3 3

#define rLife1 4  //For the lights that represent red's lives
#define rLife2 5
#define rLife3 6

#define bPos1 7 //For the lights that represent blue's position
#define bPos2 8
#define bPos3 9
#define bPos4 10
#define bPos5 11

#define rPos1 12  //For the lights that represent red's position
#define rPos2 13
#define rPos3 14
#define rPos4 15
#define rPos5 16

#define yPos1 35  //For the lights that represent the bullets in the middle of the players
#define yPos2 36
#define yPos3 37
#define yPos4 33
#define yPos5 34

#define bShoot 17 //For blue's controls
#define bLeft 18
#define bRight 21

#define rShoot 38 //For red's controls
#define rLeft 39
#define rRight 40

int lastbShoot = LOW; //These variables are used to make sure that each event triggers only once and only when the button is pushed down
int lastbLeft = LOW;
int lastbRight = LOW;
int lastrShoot = LOW;
int lastrLeft = LOW;
int lastrRight = LOW;

class Player {  //A player class to make the code more readable
  private:
    int lives;  //How many lives the player is on
    int position; //The position will be added to either player's leftmost position to give the LED that should light up
    int lastLife; //The position of the life that will be the last one remaining lit up when the player is on 1 life

  public:
    Player(int newLastLife){
      lives = 3;  //Initialise the full 3 lives
      position = 2; //Place the player in the middle of their grid
      lastLife = newLastLife; //The pin that the last life is connected to
    }
    void loseLife(){  //Used when a player is shot
      lives = lives-1;
    }
    int getLives(){
      return lives;
    }
    int getPos(){
      return position;
    }
    int getLastLife(){  //Location of the player's last life, used in the shoot function
      return lastLife;
    }
    int moveLeft(){
      position = (position + 4) % 5;  //Move to the left, wraps around
      return position;
    }
    int moveRight(){
      position = (position + 1) % 5;  //Move to the right, wraps around
      return position;
    }
    void shoot(Player &victim){
      int shotPosition = position;  //Store the shooter's initial position
      digitalWrite(yPos1 + shotPosition, HIGH); //Light up the bullet light corresponding to the shooter's position
      delay(300); //Give the victim time to react
      if (victim.getPos() == shotPosition){ //If the victim is still in the bullet position
        victim.loseLife();    //Remove one of the lives of the victim
        if (victim.getLives() == 2){  //If the victim is now on 2 lives, switch off one of their life lights
          digitalWrite(victim.getLastLife() + 2, LOW);
          digitalWrite(yPos1 + shotPosition, LOW);  //Switch off the bullet light
        } else if (victim.getLives() == 1){ //If the victim is now on their last life, switch off another one of their lights
          digitalWrite(victim.getLastLife() + 1, LOW);
          digitalWrite(yPos1 + shotPosition, LOW);  //Switch off the bullet light
        } else{
          digitalWrite(victim.getLastLife(), LOW);  //Otherwise the victim now has 0 lives, switch off their last life, the bullet light never turns off in this case
        }
      } else{
        digitalWrite(yPos1 + shotPosition, LOW);  //Switch off the bullet light after the delay if the victim is not hit
      }
    }
};

Player blue(bLife1);  //Create the two players
Player red(rLife1);

void setup() {
  // put your setup code here, to run once:

  pinMode(bLife1, OUTPUT);  //Set all of the pin modes
  pinMode(bLife2, OUTPUT);
  pinMode(bLife3, OUTPUT);

  pinMode(rLife1, OUTPUT);
  pinMode(rLife2, OUTPUT);
  pinMode(rLife3, OUTPUT);

  pinMode(bPos1, OUTPUT);
  pinMode(bPos2, OUTPUT);
  pinMode(bPos3, OUTPUT);
  pinMode(bPos4, OUTPUT);
  pinMode(bPos5, OUTPUT);

  pinMode(rPos1, OUTPUT);
  pinMode(rPos2, OUTPUT);
  pinMode(rPos3, OUTPUT);
  pinMode(rPos4, OUTPUT);
  pinMode(rPos5, OUTPUT);

  pinMode(yPos1, OUTPUT);
  pinMode(yPos2, OUTPUT);
  pinMode(yPos3, OUTPUT);
  pinMode(yPos4, OUTPUT);
  pinMode(yPos5, OUTPUT);

  pinMode(bShoot, INPUT_PULLUP);
  pinMode(bLeft, INPUT_PULLUP);
  pinMode(bRight, INPUT_PULLUP);

  pinMode(rShoot, INPUT_PULLUP);
  pinMode(rLeft, INPUT_PULLUP);
  pinMode(rRight, INPUT_PULLUP);

  digitalWrite(bLife1, HIGH); //Give both players all 3 lives
  digitalWrite(bLife2, HIGH);
  digitalWrite(bLife3, HIGH);
  digitalWrite(rLife1, HIGH);
  digitalWrite(rLife2, HIGH);
  digitalWrite(rLife3, HIGH);

  digitalWrite(bPos3, HIGH);  //Place both players in the middle of their rows
  digitalWrite(rPos3, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(bShoot) == LOW && lastbShoot == HIGH){  //The "if" statements look complicated but it's just my way of ensuring that only the button pushdown edge is detected
    lastbShoot = !lastbShoot;                             //Same with the flipping of the variables
    // shoot(blue, red);
    blue.shoot(red);
  } else if (digitalRead(bShoot) == HIGH && lastbShoot == LOW){
    lastbShoot = !lastbShoot;
  }
  if (digitalRead(rShoot) == LOW && lastrShoot == HIGH){
    lastrShoot = !lastrShoot;
    // shoot(red, blue);
    red.shoot(blue);
  } else if (digitalRead(rShoot) == HIGH && lastrShoot == LOW){
    lastrShoot = !lastrShoot;
  }
  if (digitalRead(bLeft) == LOW && lastbLeft == HIGH){
    lastbLeft = !lastbLeft;
    digitalWrite(bPos1 + blue.getPos(), LOW);
    blue.moveLeft();
    digitalWrite(bPos1 + blue.getPos(), HIGH);
  } else if (digitalRead(bLeft) == HIGH && lastbLeft == LOW){
    lastbLeft = !lastbLeft;
  }
  if (digitalRead(rLeft) == LOW && lastrLeft == HIGH){
    lastrLeft = !lastrLeft;
    digitalWrite(rPos1 + red.getPos(), LOW);
    red.moveLeft();
    digitalWrite(rPos1 + red.getPos(), HIGH);
  } else if (digitalRead(rLeft) == HIGH && lastrLeft == LOW){
    lastrLeft = !lastrLeft;
  }
  if (digitalRead(bRight) == LOW && lastbRight == HIGH){
    lastbRight = !lastbRight;
    digitalWrite(bPos1 + blue.getPos(), LOW);
    blue.moveRight();
    digitalWrite(bPos1 + blue.getPos(), HIGH);
  } else if (digitalRead(bRight) == HIGH && lastbRight == LOW){
    lastbRight = !lastbRight;
  }
  if (digitalRead(rRight) == LOW && lastrRight == HIGH){
    lastrRight = !lastrRight;
    digitalWrite(rPos1 + red.getPos(), LOW);
    Serial.printf("Red was at %d, now at ", red.getPos());
    red.moveRight();
    Serial.printf("%d\n", red.getPos());
    digitalWrite(rPos1 + red.getPos(), HIGH);
  } else if (digitalRead(rRight) == HIGH && lastrRight == LOW){
    lastrRight = !lastrRight;
  }
  if (blue.getLives() == 0){
    digitalWrite(blue.getPos(), LOW);   //Swtich off the loser's lights
    digitalWrite(rPos1, HIGH);  //Light up all of the winner's lights
    digitalWrite(rPos2, HIGH);
    digitalWrite(rPos3, HIGH);
    digitalWrite(rPos4, HIGH);
    digitalWrite(rPos5, HIGH);
    Serial.println("Red wins");
    while(1){}  //Stop the game
  }
  if (red.getLives() == 0){
    digitalWrite(rPos1 + red.getPos(), LOW);
    digitalWrite(bPos1, HIGH);
    digitalWrite(bPos2, HIGH);
    digitalWrite(bPos3, HIGH);
    digitalWrite(bPos4, HIGH);
    digitalWrite(bPos5, HIGH);
    Serial.println("Blue wins");
    while(1){}
  }
}

// void shoot(Player &shooter, Player &victim){
//   int shotPosition = shooter.getPos();
//   digitalWrite(yPos1 + shotPosition, HIGH);
//   delay(300);
//   if (victim.getPos() == shotPosition){
//     victim.loseLife();
//     if (victim.getLives() == 2){
//       digitalWrite(victim.getLastLife() + 2, LOW);
//       digitalWrite(yPos1 + shotPosition, LOW);
//     } else if (victim.getLives() == 1){
//       digitalWrite(victim.getLastLife() + 1, LOW);
//       digitalWrite(yPos1 + shotPosition, LOW);
//     } else{
//       digitalWrite(victim.getLastLife(), LOW);
//     }
//   } else{
//     digitalWrite(yPos1 + shotPosition, LOW);
//   }
// }