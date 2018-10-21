// Variables for wireless controller
int WController[3] = {2,3,4,5};


// Variables for wireless controller
int TelephoneSound = 13;
int door1 = 12;
int door2 = 11;




void setup() {

  // //////////////////Define Input////////////////
  
  for (int i = 0; i<3;i++){
    pinMode(WController[i],INPUT);
  }



  // //////////////////Define Output///////////////
  pinMode(TelephoneSound,OUTPUT);
  pinMode(door1,OUTPUT);
  pinMode(door2,OUTPUT);
  

}

void loop() {
  WirelessControll();
}

// Wireless Controll door and telephone
void WirelessControll(){

  // Controll the telephone sound (telephone MP3)
  if (digitalRead(WController[0]) == HIGH){
    digitalWrite(TelephoneSound,HIGH);
    Serial.println("Phone Ring");
  }

  // Controll the first door
  if (digitalRead(WController[1]) == HIGH){
    digitalWrite(door1,HIGH);
    Serial.println("door 1 open");
  }

  // Controll the second door
  if (digitalRead(WController[2]) == HIGH){
    digitalWrite(door2,HIGH);
    Serial.println("door 2 open");
  }

  // Close all door
  if (digitalRead(Controller[3]) == HIGH){
    digitalWrite(door1,LOW);
    digitalWrite(door2,LOW);
    Serial.println("all door clsed");
  }
}
