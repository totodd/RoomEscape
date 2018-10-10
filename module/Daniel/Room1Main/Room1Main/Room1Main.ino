// Relay defination
int illumination[5] = {22, 23, 24, 25, 26}; // Relay 1, 2, 3, 4, 5 LOW means lights on, illumination[4]---Relay 5 controls all illumination (1-8)
int alarm = 27 ;                        // Relay 6   LOW means alarm on
int door[7] = { 28, 29, 30, 31, 32, 33, 34} ; // Relay 8, 9, 10, 11, 12, 13, 14  LOW means door open


// Constant number defination
int doorCount = 7;
int illuminationCount = 5;

// Slave controller definaiton (INPUT)
int blueWordSoundTriger = 2;
int ElectricityCuteOffTriger = 10 ;
int ElectricityReset = 11;
int ExternalillunationTriger[4] = {4, 5, 6, 7};

// Wireless Controller
int wController[8] = {4, 2, 5, 3, 8, 9, 6, 7};


// State Controll
bool isPowerGenerated;

void setup() {
  Serial.begin(9600);
  // OUTPUT

  // Door define, initially, the door are all closed, when write to HIGH, realy stop works, door open
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
  pinMode(blueWordSoundTriger, INPUT);
  pinMode(ElectricityCuteOffTriger, INPUT);
  pinMode(ElectricityReset,INPUT);

  // State Control
  isPowerGenerated = false;
}

void loop() {

//Serial.println(digitalRead(ElectricityReset));
  // Door controller
  for (int thiswController = 0; thiswController < 5; thiswController ++) {
    int controlSignal = digitalRead(wController[thiswController]);
    if (controlSignal == HIGH) {
      openSpecificDoor(thiswController);
    }
  }

  // After first lighting, electricity cut off
  if (digitalRead(ElectricityCuteOffTriger) == HIGH) {
    electricityCutOff();
    Serial.println("electricity off");
  }

  // After Electricity generation, electricity reset
  if (digitalRead(ElectricityReset) == HIGH && isPowerGenerated == false){
    electricityOn();
    Serial.println("electricity ON");
    isPowerGenerated = true;
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
  delay(3000);
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
}
