// Relay defination
int illumination = ;
int alarm = ;
int lighting = ;
int door[] = {}

// Constant number defination
int doorCount = 7;

// Slave controller definaiton


void setup() {
  for (thisDoor = 0; thisDoor < 7; thisDoor ++) {
    pinMode(door[thisDoor], OUTPUT);
  }
  pinMode(lighting, OUTPUT);
  pinMode(alarm, OUTPUT);
  pinMode(illumination, OUTPUT);
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
void openSpecificDoor(int a){
  digitalWrite(door[a],HIGH);
}

// Close some specific door
void closeSpecificDoor(int a){
  digitalWrite(door[a],LOW);
}
