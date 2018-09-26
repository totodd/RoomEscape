/*
   This file is used for 10SwitchController, includs pin setting and pin match, when pin matched, external signal will write to HIGH
*/
#include <EEPROM.h>
int addr = 0;


// Declearation of 10 switchs
int switchs[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
int switchCount = 10;

// Current pin input
int currentPin[10];
// Saved pre-setting pin
int presetPin[10];
// Used to send external signal
int externalSignal = A0;
// Used to save new pin
int pinSetButton = A1;
// Used to test whether current pin and pre-setting pin matched
int pinMatchButton = A2;
// LED to show the action state, blink with 1s, means pin-setting succeed, blink quickly means pin incorrect
int stateDisplay = A3;
// Pin can only be set when this trigger is high
int pinSetTrigger = A4;

void setup() {
  Serial.begin(9600);
  for (int thisSwitch = 0; thisSwitch < switchCount; thisSwitch++) {
    pinMode(switchs[thisSwitch], OUTPUT);
  }
  pinMode(externalSignal, OUTPUT);
  pinMode(pinSetButton, INPUT);
  pinMode(pinMatchButton, INPUT);
  pinMode(stateDisplay, OUTPUT);
  pinMode(pinSetTrigger, INPUT);
}

// function to compare two arrays.
boolean array_cmp(int *a, int *b, int len_a, int len_b) {
  int n;

  // if their lengths are different, return false
  if (len_a != len_b) return false;

  // test each element to be the same. if not, return false
  for (n = 0; n < len_a; n++) if (a[n] != b[n]) return false;

  //ok, if we have not returned yet, they are equal :)
  return true;
}



// To check Pin is correct
void pinMatchCheck() {
  if (array_cmp(currentPin, presetPin, 10, 10) == true) {
    Serial.println("Pin Matched");
    digitalWrite(externalSignal, HIGH);
    delay(10000);
    digitalWrite(externalSignal, LOW);
  }
  else {
    digitalWrite(externalSignal, LOW);
    digitalWrite(stateDisplay, HIGH);
    delay(100);
    digitalWrite(stateDisplay, LOW);
    delay(100);
    digitalWrite(stateDisplay, HIGH);
    delay(100);
    digitalWrite(stateDisplay, LOW);
    delay(100);
    digitalWrite(stateDisplay, HIGH);
    delay(100);
    digitalWrite(stateDisplay, LOW);
  }

}


// This function is used to store customized pin, when it called, it will store the current pin into preset pin
void setPin() {
  for (int thisSwitch = 0; thisSwitch < switchCount; thisSwitch++) {
    presetPin[thisSwitch] = digitalRead(switchs[thisSwitch]);
    EEPROM.write(addr, currentPin[thisSwitch]);
    addr = addr + 1;
    delay(100);
  }
  digitalWrite(stateDisplay, HIGH);
  delay(1000);
  digitalWrite(stateDisplay, LOW);
  delay(1000);
  digitalWrite(stateDisplay, HIGH);
  delay(1000);
  digitalWrite(stateDisplay, LOW);
  delay(1000);
  digitalWrite(stateDisplay, HIGH);
  delay(1000);
  digitalWrite(stateDisplay, LOW);
}






void loop() {
  /*
     Show message for debug
  */
  // current pin display;
  Serial.println("The current pin is ");
  for (int thisSwitch = 0; thisSwitch < switchCount; thisSwitch++) {
    currentPin[thisSwitch] = digitalRead(switchs[thisSwitch]);
    Serial.print(currentPin[thisSwitch]);
  }
  Serial.println();

  // pre setting pin display
  Serial.println("The presetting pin is "); 
  for (int thisaddr = 0; thisaddr < 10; thisaddr++) {
    presetPin[thisaddr] = EEPROM.read(thisaddr);
    Serial.print(EEPROM.read(thisaddr));
    
  }
  Serial.println();
  delay(500);

  // When pinSetTrigger is high, in the pin-setting mode, when button pressed, the new pin will be saved
  if (digitalRead(pinSetButton) == HIGH && digitalRead(pinSetTrigger) == HIGH) {
    setPin();
  }
  // When this button is pressed, check whether pre-setting pin matched to the current pin.
  if (digitalRead(pinMatchButton) == HIGH) {
    pinMatchCheck();
  }
}

