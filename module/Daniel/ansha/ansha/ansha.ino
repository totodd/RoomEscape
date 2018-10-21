// Variables for wireless controller
int WController[4] = {4,2,5,3};
int check[4];

// Variables for wireless controller
int TelephoneSound = 13;
int door1 = 12;
int door2 = 11;


// Time Variable defination;
int TelephoneSoundStartTime;
int door1StartTime;
int door2StartTime;
int TelephoneSoundDuration;
int door1Durtation;
int door2Durtation;

// Variables for A7
int MSensor1 = 10;
int MSensor2 = 9;
int ICSensor = 8;
int Power1 = 7;
int Power2 = 6;
int IndoorMP3 = A0;
int doorcheck= A2;
bool MSensorTriger = false;
bool isVideo6 = false;




void setup() {
  Serial.begin(9600);

  // //////////////////Define Input////////////////
  
  for (int i = 0; i<3;i++){
    pinMode(WController[i],INPUT);
  }
  pinMode(MSensor1,INPUT_PULLUP);
  pinMode(MSensor2,INPUT_PULLUP);



  // //////////////////Define Output///////////////
  pinMode(TelephoneSound,OUTPUT);
  pinMode(door1,OUTPUT);
  pinMode(door2,OUTPUT);
  pinMode(doorcheck,INPUT_PULLUP);
  

}

void loop() {
    // Inspect the state of door and telephone
  /*for (int i =0;i<3;i++){
    Serial.print(check[i]);
  }
  Serial.println();*/
  Serial.println(digitalRead(doorcheck));
  

  // Controll the telephone and door
  WirelessControllerParent(5000);
  // A7
  A7PowerOFF();


}



// Wireless Controll door and telephone open
void WirelessControll(){

  // Controll the telephone sound (telephone MP3)
  if (digitalRead(WController[0]) == HIGH){
    TelephoneSoundStartTime = millis();
    digitalWrite(TelephoneSound,HIGH);
    Serial.println("Phone Ring");
    check[0] = 1;
  }

  // Controll the first door
  if (digitalRead(WController[1]) == HIGH){
    door1StartTime = millis();
    digitalWrite(door1,HIGH);
    Serial.println("door 1 open");
    check[1] = 1;
  }

  // Controll the second door
  if (digitalRead(WController[2]) == HIGH){
    door2StartTime = millis();
    digitalWrite(door2,HIGH);
    Serial.println("door 2 open");
    check[2] = 1;
  }

  // Close all door
  if (digitalRead(WController[3]) == HIGH){
    digitalWrite(door1,LOW);
    digitalWrite(door2,LOW);
    Serial.println("all door clsed");
    check[1] = 0;
    check[2] = 0;
    check[3] = 0;
  }
}

// parent of wirelessController which can controll the duration of door open;
void WirelessControllerParent(int doorDurationTime){
  WirelessControll();
  TelephoneSoundDuration = millis()-TelephoneSoundStartTime;
  door1Durtation = millis()-door1StartTime;
  door2Durtation = millis()-door2StartTime;
  if ((TelephoneSoundDuration) > 1000){
    digitalWrite(TelephoneSound,LOW);
    check[0] = 0;
  }
  if (door1Durtation > doorDurationTime){
    digitalWrite(door1,LOW);
    check[1] = 0;
  }
  if (door2Durtation > doorDurationTime){
    digitalWrite(door2,LOW);
    check[2] = 0;
  }
}


// function A7PowerOFF
void A7PowerOFF(){

  // When the first mSensor is detected, turn off power 1 and play sound
  if (digitalRead(MSensor1) == LOW){
    digitalWrite(Power1,LOW);
    digitalWrite(IndoorMP3,HIGH);
    Serial.println("mSensor 1 detected");
  }

  // When IC sensor is detected and door 1 is closed, turn on pwer 2
  if (digitalRead(ICSensor) == HIGH && digitalRead(doorcheck) == LOW){
        digitalWrite(Power1,HIGH);
  }

  // When video 6 is finished, enable the second mSensor and turn off power 2;
  if (isVideo6){
    digitalWrite(Power2,LOW);
    MSensorTriger = true;
  }

  // When the second mSensor is detected, turn open the first door.
  if (digitalRead(MSensor2) == HIGH && MSensorTriger){
    digitalWrite(door1,HIGH);
    check[1] = 1;
  }
  
}
