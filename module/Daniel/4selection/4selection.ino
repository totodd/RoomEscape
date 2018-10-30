/*
   This file is used for 10SwitchController, includs pin setting and pin match, when pin matched, external signal will write to HIGH
*/
#include <EEPROM.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


Adafruit_SSD1306 display;



#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif




// Declearation of 10 switchs
int switchs[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
int LED[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37};
int switchCount = 16;

// Current pin input
int currentPin[16];
// Saved pre-setting pin
int presetPin[16];
// Used to send external signal
int externalSignal = A15;
// used to detect whether it is in pin set mode
int pinSetFlag = 0;
// pin store address
int addr = 0;

// Pin can only be set when this trigger is high
int pinSetTrigger = A2;


void initDisplay() {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  display.clearDisplay();
  display.setRotation(0);
  display.dim(0);
}


void setup() {
  Serial.begin(9600);
  for (int thisSwitch = 0; thisSwitch < switchCount; thisSwitch++) {
    pinMode(switchs[thisSwitch], INPUT_PULLUP);
    pinMode(LED[thisSwitch], OUTPUT);
  }
  pinMode(externalSignal, OUTPUT);
  pinMode(pinSetTrigger, INPUT_PULLUP);
  initDisplay();
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

void showTextNumber(String s, int num) {
  // display.setFont(&FreeMonoBold12pt7b);  // Set a custom font
  char string[10];

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setTextWrap(true);
  dtostrf(num, 3, 0, string);

  display.setCursor(0, 0);
  display.clearDisplay();
  display.print(s);
  display.println(string);
  display.display();
  // delay(1);
}


void showText(String s) {
  // display.setFont(&FreeMonoBold12pt7b);  // Set a custom font
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setTextWrap(true);

  display.setCursor(0, 0);
  display.clearDisplay();
  display.println(s);
  display.display();
  // delay(1);
}
void showTextPlayMode(String s) {
  // display.setFont(&FreeMonoBold12pt7b);  // Set a custom font
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setTextWrap(true);

  display.setCursor(0, 0);
  display.clearDisplay();

  //display.println("play mode");
  display.println(s);

  display.display();
  // delay(1);
}

// To check Pin is correct
void pinMatchCheck() {
  if (array_cmp(currentPin, presetPin, 16, 16) == true) {
    Serial.println("Pin Matched");
    digitalWrite(externalSignal, HIGH);
    delay(10000);
    digitalWrite(externalSignal, LOW);
  }
  else {
    digitalWrite(externalSignal, LOW);
  }

}


// This function is used to store customized pin, when it called, it will store the current pin into preset pin
void setPin() {
  for (int thisSwitch = 0; thisSwitch < switchCount; thisSwitch++) {
    presetPin[thisSwitch] = digitalRead(switchs[thisSwitch]);
    EEPROM.put(thisSwitch, currentPin[thisSwitch]);
  }
  showText("Succeed");
  delay(1000);
}







void PinConfiguration() {
  /*
    Show message for debug
  */
  // current pin display;
  //Serial.println("The current pin is ");
  for (int thisSwitch = 0; thisSwitch < switchCount; thisSwitch++) {
    currentPin[thisSwitch] = digitalRead(switchs[thisSwitch]);
    Serial.print(currentPin[thisSwitch]);
  }
  Serial.println();


  // pre setting pin display
  //Serial.println("The presetting pin is ");
  for (int thisaddr = 0; thisaddr < 16; thisaddr++) {
    presetPin[thisaddr] = EEPROM.read(thisaddr);
    Serial.print(EEPROM.read(thisaddr));

  }
  Serial.println();
  //9wetadelay(500);
}


void loop() {

  PinConfiguration();
  indicator();
  if (digitalRead(pinSetTrigger) == HIGH) {
    Serial.println("pin set mode");
    //showText("Pin Set");
    pinSetFlag = 1;
  }
  else {
    if (pinSetFlag == 1) {
      setPin();
      delay(3000);
      pinSetFlag = 0;
    }
    Serial.println("Play mode");
    showTextPlayMode(String(currentPin[0]) + String(currentPin[1]) + String(currentPin[2]) + String(currentPin[3]) + String(currentPin[4]) + String(currentPin[5]) + String(currentPin[6]) + String(currentPin[7]) + String(currentPin[8]) + String(currentPin[9]) + String(currentPin[10]) + String(currentPin[11] + String(currentPin[12]) + String(currentPin[13]) + String(currentPin[14]) + String(currentPin[15])));
    pinMatchCheck();

  }


}

void indicator() {
  for (int thisSwitch = 0; thisSwitch < switchCount; thisSwitch++) {
    if (digitalRead(switchs[thisSwitch]) == HIGH) {
      digitalWrite(LED[thisSwitch], LOW);
    }
    else {
      digitalWrite(LED[thisSwitch], HIGH);
    }
  }
}
