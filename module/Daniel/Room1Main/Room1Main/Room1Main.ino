// Relay defination
int illumination[5] = {39, 38, 37, 36, 35}; // Relay 1, 2, 3, 4, 5
int alarm = 34 ;                        // Relay 6
int lighting = 33;                      // Relay 7
int door[7] = {32, 31, 30, 29, 28, 27, 26} // Relay 8, 9, 10, 11, 12, 13, 14

              // Constant number defination
              int doorCount = 7;
int illuminationCount = 5;

// Slave controller definaiton
int blueWordSoundTriger = ;

void setup() {
  for (thisDoor = 0; thisDoor < doorCount; thisDoor ++) {
    pinMode(door[thisDoor], OUTPUT);
  }
  for (thisillumination = 0; thisillumination < illuminationCount; thisillumination ++) {
    pinMode(illumination[thisillumination], OUTPUT);
  }
  pinMode(lighting, OUTPUT);
  pinMode(alarm, OUTPUT);
}

void loop() {


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
  for (thisillumination = 0; thisillumination < illuminationCount; thisillumination ++) {
    digitalWrite(illumination[thisillumination], LOW);
  }
  digitalWrite(alarm, HIGH);
  digitalWrite(blueWordSoundTriger, HIGH);
}

// blueWordOn
void blueWordOFF() {
  for (thisillumination = 0; thisillumination < illuminationCount; thisillumination ++) {
    digitalWrite(illumination[thisillumination], HIGH);
  }
  digitalWrite(alarm, LOW);
  digitalWrite(blueWordSoundTriger, LOW);
}
