// Relay defination
int illumination[5] = {38, 39, 36, 37, 34}; // Relay 1, 2, 3, 4, 5 HIGH means lights on, illumination[4]---Relay 5 controls all illumination (1-8)
int alarm = 35 ;                        // Relay 6   HIGH means alarm on
int door[7] = {34, 33, 32, 31, 30, 29, 28} ;// Relay 8, 9, 10, 11, 12, 13, 14  HIGH means door open


// Constant number defination
int doorCount = 7;
int illuminationCount = 5;

// Slave controller definaiton (INPUT)
int blueWordSoundTriger = 2;
int ElectricityCuteOffTriger = 3 ;
int ExternalillunationTriger[4] = {4,5,6,7};
int TongYaoTriger = 8;
int TongYaoPlay = 9;


void setup() {
  Serial.begin(9600);
  // OUTPUT
  for (int thisDoor = 0; thisDoor < doorCount; thisDoor ++) {
    pinMode(door[thisDoor], OUTPUT);
  }
  for (int thisillumination = 0; thisillumination < illuminationCount; thisillumination ++) {
    pinMode(illumination[thisillumination], OUTPUT);
  }
  pinMode(TongYaoPlay, OUTPUT);
  pinMode(alarm, OUTPUT);

  // INPUT
  for (int thisexternalillumination = 0; thisexternalillumination < 4; thisexternalillumination ++) {
    pinMode(ExternalillunationTriger[thisexternalillumination], INPUT);
  }
  pinMode(blueWordSoundTriger,INPUT);
  pinMode(ElectricityCuteOffTriger,INPUT);    
  pinMode(TongYaoTriger,INPUT_PULLUP);
}

void loop() {
  if (digitalRead(TongYaoTriger) == HIGH){
    digitalWrite(TongYaoPlay,HIGH); 
  }
  if (digitalRead(ElectricityCuteOffTriger) == HIGH) {
    electricityCutOff();
    Serial.println("Begin to cutoff power"); 
    delay(5000); //need to be replaced with power generator
    electricityOn();
  }

  for (int thisexternalillumination = 0; thisexternalillumination < 4; thisexternalillumination ++) {
    if( digitalRead(ExternalillunationTriger[thisexternalillumination]) == HIGH){
      illumination[thisexternalillumination] =HIGH;
    }
  }

}


// Open all five doors
void all5DoorOpen() {
  for (int thisDoor = 0; thisDoor < 5; thisDoor ++) {
    digitalWrite(door[thisDoor], HIGH);
  }
}
// Close all five doors
void all5DoorClose() {
  for (int thisDoor = 0; thisDoor < 5; thisDoor ++) {
    digitalWrite(door[thisDoor], LOW);
  }
}

// Open some specific door
void openSpecificDoor(int a) {
  digitalWrite(door[a], HIGH);
}

// Close some specific door
void closeSpecificDoor(int a) {
  digitalWrite(door[a], LOW);
}

// blueWordOn
void blueWordON() {
  digitalWrite(illumination[4], LOW);
  digitalWrite(alarm, HIGH);
  digitalWrite(blueWordSoundTriger, HIGH);
}

// blueWordOn
void blueWordOFF() {
  digitalWrite(illumination[4], HIGH);
  digitalWrite(alarm, LOW);
  digitalWrite(blueWordSoundTriger, LOW);
}

// Electricity cut off
void electricityCutOff() {
  digitalWrite(illumination[4], LOW);
  for (int thisdoor = 0; thisdoor < 3; thisdoor ++) {
    digitalWrite(door[thisdoor], HIGH);
  }
}

// Electricity on
void electricityOn() {
  digitalWrite(illumination[4], HIGH);
  for (int thisdoor = 0; thisdoor < 3; thisdoor ++) {
    digitalWrite(door[thisdoor], LOW);
  }
}
