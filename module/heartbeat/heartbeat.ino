// 继电器控制？？？？？？？？
// 接收busy口 high = working
// 每个音频1对output口

#define HUMANSENSOR 12
#define DISPLAY_PIN A0
#define DISPLAY_SHOW A1
#define MUSICPIN1 6
#define MUSICPIN2 7
#define MUSICPIN3 8
#define MUSICBUSY 9
#define ENABLE_PIN1 10
#define ENABLE_PIN2 11
#define DOOR_PIN 5
#define CARD1 4
#define CARD2 3
int musicPins[] = {MUSICPIN1, MUSICPIN2, MUSICPIN3};
int enablePins[] = {ENABLE_PIN1, ENABLE_PIN2};
int musicPlayed[] = {0,0,0};
bool disableRFID = true;
bool displayShow = false;

int gameState = 0;
bool displayed = false;

void setup(){
  Serial.begin(9600);
  delay(10000); //5 min (10s for test)
  pinMode(HUMANSENSOR, INPUT);
  pinMode(MUSICPIN1, OUTPUT);
  pinMode(MUSICPIN2, OUTPUT);
  pinMode(MUSICPIN3, OUTPUT);
  pinMode(MUSICBUSY, INPUT_PULLUP);
  pinMode(ENABLE_PIN1, OUTPUT);
  pinMode(ENABLE_PIN2, OUTPUT);
  pinMode(DOOR_PIN, OUTPUT);
  pinMode(CARD1, INPUT);
  pinMode(CARD2, INPUT);
  pinMode(DISPLAY_PIN, OUTPUT);
  pinMode(DISPLAY_SHOW, OUTPUT);

  digitalWrite(DISPLAY_PIN, LOW);
  digitalWrite(MUSICPIN1, LOW);
  digitalWrite(MUSICPIN2, LOW);
  digitalWrite(MUSICPIN3, LOW);

}

void loop(){
  Serial.print("state is "); Serial.print(gameState);
  Serial.print("music status "); Serial.print(musicPlayed[0]);Serial.print(musicPlayed[1]);Serial.print(musicPlayed[2]);
  Serial.print("  play# is "); Serial.println(musicPlayed[1]);

  // Serial.println("card 1 is" + digitalRead(CARD1));
  // Serial.println("card 2 is" + digitalRead(CARD2));
  // Serial.println("music busy is" + digitalRead(MUSICBUSY));
  RFIDDisable(1);
  RFIDDisable(2);

  switch (gameState) {
    case 0: // initial state
      if(digitalRead(HUMANSENSOR)) gameState = 1;
      digitalWrite(DISPLAY_SHOW, LOW);

      break;
    case 1: // player entrance, A4 working
      delay(1000); //10s
      gameState = 2;
            digitalWrite(DISPLAY_SHOW, HIGH);

      break;
    case 2: // after 10s dalay
      if(!displayed) displayStart();
      if(musicPlayed[0] == 0) playMusic(1);
      if(!digitalRead(MUSICBUSY)&& musicPlayed[0]>0) gameState = 3;
      break;
    case 3: // RFID A working
      RFIDEnable(1);
      if(card1Action() == true) gameState = 4;
      break;
    case 4: // RFID BC working
      RFIDDisable(1);
      RFIDEnable(2);
      if(card2Action() == true) {
        if(musicPlayed[2] == 0) playMusic(3);
        delay(1000);
      }
      if(musicPlayed[2]>0 && !digitalRead(MUSICBUSY))
        gameState = 5;
      break;
    case 5: //
      openDoor();
      break;
  }
}
void displayStart(){
  digitalWrite(DISPLAY_PIN, LOW);
  delay(10);
  digitalWrite(DISPLAY_PIN, HIGH);
  displayed = true;
}

void RFIDEnable(int i){
  digitalWrite(enablePins[i-1], HIGH);
  // Serial.print(enablePins[i-1]); Serial.print(" "); Serial.println(LOW);
}

void RFIDDisable(int i){
  digitalWrite(enablePins[i-1], LOW);
  // Serial.print(enablePins[i-1]); Serial.print(" "); Serial.println(HIGH);

}

void playMusic(int i){
  digitalWrite(musicPins[i-1], HIGH);
 delay(1000);
 digitalWrite(musicPins[i-1], LOW);
  musicPlayed[i-1]++;
}

bool card1Action(){
  bool cardAttached = false;
  Serial.print("cardattached ");Serial.println(cardAttached);
  if(digitalRead(CARD1)&&!cardAttached && !digitalRead(MUSICBUSY)){
    cardAttached = true;
    playMusic(2);
    delay(2000); //delay 2s
  }

  if(!digitalRead(CARD1) && cardAttached){
    cardAttached = false;
  }
  if(musicPlayed[1] >= 3) return true;
  return false;
}

bool card2Action(){
  return (digitalRead(CARD2));
}

void openDoor(){
  digitalWrite(DOOR_PIN, HIGH);
}

