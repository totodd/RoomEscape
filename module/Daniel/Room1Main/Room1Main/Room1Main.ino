// Relay defination
int illumination[5] = {39, 38, 37, 36, 35}; // Relay 1, 2, 3, 4, 5 HIGH means lights on, illumination[4]---Relay 5 controls all illumination (1-8)
int alarm = 34 ;                        // Relay 6   HIGH means illumination on
int lighting = 33;                      // Relay 7   HIGH means ligthing on
int door[7] = {32, 31, 30, 29, 28, 27, 26} // Relay 8, 9, 10, 11, 12, 13, 14  HIGH means door open

              // Constant number defination
              int doorCount = 7;
int illuminationCount = 5;

// Slave controller definaiton (INPUT)
int blueWordSoundTriger = ;
int ElectricityCuteOffTriger = ;
int ExternalillunationTriger = {};



void setup() {
  for (thisDoor = 0; thisDoor < doorCount; thisDoor ++) {
    pinMode(door[thisDoor], OUTPUT);
  }
  for (thisillumination = 0; thisillumination < illuminationCount; thisillumination ++) {
    pinMode(illumination[thisillumination], OUTPUT);
  }
  pinMode(lighting, OUTPUT);
  pinMode(alarm, OUTPUT);

  for (thisexternalillumination = 0; thisexternalillumination < 4; thisexternalillumination ++) {
    pinMode(ExternalillunationTriger[thisexternalillumination], INPUT);
  }
}

void loop() {
  if (digitalRead(ElectricityCuteOffTriger) == HIGH) {
    electricityCutOff();
  }
  for (thisexternalillumination = 0; thisexternalillumination < 4; thisexternalillumination ++) {
    illumination[thisexternalillumination] = digitalRead(ExternalillunationTriger[thisexternalillumination]);
  }

}


// Open all five doors
void all5DoorOpen() {
  for (thisDoor = 0; thisDoor < 5; thisDoor ++) {
    digitalWrite(door[thisDoor], HIGH);
  }
}
// Close all five doors
void all5DoorClose() {
  for (thisDoor = 0; thisDoor < 5; thisDoor ++) {
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
  for (thisdoor = 0; thisdoor < 3; thisdoor ++) {
    digitalWrite(door[thisdoor], HIGH);
  }
}

// Electricity on
void electricityOn() {
  digitalWrite(illumination[4], HIGH);
  for (thisdoor = 0; thisdoor < 3; thisdoor ++) {
    digitalWrite(door[thisdoor], LOW);
  }
}
