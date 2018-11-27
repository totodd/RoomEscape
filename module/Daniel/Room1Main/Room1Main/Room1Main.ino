/*
   Part A: This is the variable defination part, which decleared all variables
*/

// Relay defination
int illumination[5] = {22, 23, 24, 25, 26}; // Relay 1, 2, 3, 4, 5 LOW means lights on, illumination[4]---Relay 5 controls all illumination (1-8)
int alarm = 27 ;                        // Relay 6   LOW means alarm on
int door[7] = { 28, 29, 30, 31, 32, 33, 34} ; // Relay 7, 8, 9, 10, 11, 12, 13  LOW means door open


// Constant number defination
int doorCount = 7;
int illuminationCount = 5;
int secondMP3_blueword_Count = 0;         // used to record the control button pressed, 1 means second MP3 start play the
// pre-setting sound, even number means start blueword mode, odd means exist the mode


// Slave controller definaiton (INPUT & OUTPUT)
// INPUT
int ElectricityCuteOffTriger = 12 ;       // When this triger is high, cut off electricity
int ElectricityReset = 13;                // When this triger is high, recover the electricity
int ExternalillunationTriger[4] = {14, 15, 16, 17};    // This part code seems useless, may delete later

// OUTPUT
int blueWordSoundTriger = 11;             // Write this triger to high if blueword sound is required
int pinPadTriger = 18;                 // Write this triger to high if the second MP3 triger is required

// Wireless Controller
int wController[8] = {4, 2, 5, 3, 9, 7, 10, 8};


// State Controll
bool isPowerGenerated;


/*
   Part B: This is the set up part, whcih shows all possible settings.
*/
void setup() {
  Serial.begin(9600);

  // Door define, initially, the door are all closed, when write to HIGH, relay stop working, door open
  for (int thisDoor = 0; thisDoor < doorCount; thisDoor ++) {
    pinMode(door[thisDoor], OUTPUT);
    digitalWrite(door[thisDoor], LOW);
  }

  // Illumination define, initially, the illuminations are light up, when write to HIGH, relay stop, lights off.
  for (int thisillumination = 0; thisillumination < illuminationCount; thisillumination ++) {
    pinMode(illumination[thisillumination], OUTPUT);
    digitalWrite(illumination[thisillumination], LOW);
  }

  // Alarm define, initially, the alarm turns off, when wirte to LOW, relay work, alarm sounds up.
  pinMode(alarm, OUTPUT);
  digitalWrite(alarm, HIGH);

  // INPUT
  for (int thisexternalillumination = 0; thisexternalillumination < 4; thisexternalillumination ++) {
    pinMode(ExternalillunationTriger[thisexternalillumination], INPUT);
  }
  for (int thiswController = 0; thiswController < 8; thiswController ++) {
    pinMode(wController[thiswController], INPUT);
  }
  pinMode(ElectricityCuteOffTriger, INPUT);
  pinMode(ElectricityReset, INPUT);


  // OUTPUT
  pinMode(blueWordSoundTriger, OUTPUT);
  pinMode(pinPadTriger, OUTPUT);

  // State Control
  isPowerGenerated = false;
}

/*
   Part C: This is the main logic of this program
*/
void loop() {

  // Initially, the room is in dark
  digitalWrite(illumination[4], HIGH);
  // Door controller (The first 5 doors)
  for (int thiswController = 0; thiswController < 5; thiswController ++) {
    int controlSignal = digitalRead(wController[thiswController]);
    if (controlSignal == HIGH) {
      openSpecificDoor(thiswController);
    }
  }


  // The second MP3 controller & illumination system controller & blueword mode controller
  /*
     Two functions includes, The first is, when the button is the first time been pressed, it will
     control the entire illumination system, if it is the second time been pressed, then the second
     MP3 player to play a music. When it is not the first time been pressed, if
     the count is even number, BLUE WORD MODE will on, else, this mode will off.
  */
  int secondMP3_blueword_Control =  digitalRead(wController[6]);
  if (secondMP3_blueword_Control == HIGH) {
    secondMP3_blueword_Count++;
  }
  if (secondMP3_blueword_Count == 1) {
    digitalWrite(illumination[4], LOW);
    Serial.println("illumination system on");
  }
  else if (secondMP3_blueword_Count == 2) {
    Serial.println("Second MP3 player play music");
    digitalWrite(blueWordSoundTriger, HIGH);
  }
  else {
    if (secondMP3_blueword_Count % 2 == 1) {
      Serial.println("Enter BlueWord mode");
      blueWordON();
    }
    else {
      Serial.println("Exist BlueWord mode");
      blueWordOFF();
    }
  }

  // The rest door controller, the rest two doors will open 5 seconds.
  int restdoor_Control =  digitalRead(wController[7]);
  if (restdoor_Control == HIGH) {
    restDoorOpen();
    delay(5000);
  }

  // After first lighting, electricity cut off
  if (digitalRead(ElectricityCuteOffTriger) == HIGH) {
    electricityCutOff();
  }
  Serial.println(digitalRead(ElectricityReset));

  // After Electricity generation, electricity reset
  if (digitalRead(ElectricityReset) == HIGH && isPowerGenerated == false) {
    electricityOn();
    Serial.println("electricity ON");
    isPowerGenerated = true;
  }

  // PinPad is actived
  if (digitalRead(pinPadTriger) == HIGH){
    all5DoorOpen();
    restDoorOpen();
    electricityCutOff();
    digitalWrite(alarm, LOW);
  }
}

/*
   Part D: This part is the function used in the main program.
*/
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

// Open rest doors
void restDoorOpen() {
  for (int thisDoor = 5; thisDoor < 7; thisDoor ++) {
    digitalWrite(door[thisDoor], HIGH);
  }
}

// Open some specific door
void openSpecificDoor(int a) {
  digitalWrite(door[a], HIGH);
  delay(3000);
  digitalWrite(door[a], LOW);
}

// blueWordOn
void blueWordON() {
  digitalWrite(illumination[4], HIGH);
  digitalWrite(alarm, LOW);
  digitalWrite(blueWordSoundTriger, HIGH);
}

// blueWordOn
void blueWordOFF() {
  digitalWrite(illumination[4], LOW);
  digitalWrite(alarm, HIGH);
  digitalWrite(blueWordSoundTriger, LOW);
}

// Electricity cut off
void electricityCutOff() {
  digitalWrite(illumination[4], HIGH);
  for (int thisdoor = 0; thisdoor < 3; thisdoor ++) {
    digitalWrite(door[thisdoor], HIGH);
  }
  Serial.println("Electricity off");
}

// Electricity on
void electricityOn() {
  digitalWrite(illumination[4], LOW);
  for (int thisdoor = 0; thisdoor < 3; thisdoor ++) {
    digitalWrite(door[thisdoor], LOW);
  }
  Serial.println("Electricity reset");
}
