// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>


#define ICPIN_1 2
#define ICPIN_2 3
#define ICPIN_3 4
#define ICPIN_4 5
#define ICPIN_5 6
#define ICPIN_6 7
#define ICPIN_7 8
#define ICPIN_8 9
#define ICPIN_9 10
#define ICPIN_10 11
// #define ICPIN_11 12
// #define ICPIN_12 13

#define MODE_SWITCH A2
#define BUTTON A1
#define output A0
#define ENABLE_PIN A3
#define IC_ENABLE 12
#define IC_RST 13

#define SEQUENCE_MODE 1
#define ALLIN_MODE 2

int ICpins[] = {ICPIN_1, ICPIN_2, ICPIN_3, ICPIN_4, ICPIN_5, ICPIN_6, ICPIN_7, ICPIN_8, ICPIN_9, ICPIN_10};

Adafruit_SSD1306 display;
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


#define MAX_NR_OF_SLAVES    12

int count;
int tapCount;
int slaveArray[MAX_NR_OF_SLAVES];
int countAddr = 1;
int savedAddr = 0;

int targetSeq[MAX_NR_OF_SLAVES];

bool sequenceResult = true;

int pinIndex = 0;
int seqCount = 0;
int cardCount = 0;
int toCheckPins[MAX_NR_OF_SLAVES];
int originPins[MAX_NR_OF_SLAVES];
int pinsStatus[MAX_NR_OF_SLAVES];

bool enable;

void setup() {
  Serial.begin (9600);
  tapCount = 0;

  initDisplay();
  // scanSlaves();

  if(EEPROM.read(savedAddr) == 1){
    count = EEPROM.read(countAddr);
  }else{
    count = 1;
  }

  pinMode(ENABLE_PIN, INPUT_PULLUP);
  pinMode(BUTTON, INPUT);
  pinMode(output, OUTPUT);
  pinMode(MODE_SWITCH, INPUT);
  pinMode(IC_ENABLE, OUTPUT);
  pinMode(IC_RST, OUTPUT);
  digitalWrite(output, LOW);
  digitalWrite(IC_RST, LOW);
  // digitalWrite(IC_RST, HIGH); // no reset


  for (int i = 0; i < MAX_NR_OF_SLAVES; i++) {
    pinMode(ICpins[i], INPUT);
  }


  for (int index = 0; index < count; index++) {
    /* code */
    targetSeq[index] = index; //target seq = 0 1 2 3 4
    toCheckPins[index] = index + 2; // pin set 2 3 4 5 6
    originPins[index] = index + 2;
  }
}

int reading[MAX_NR_OF_SLAVES];
int sequence[MAX_NR_OF_SLAVES];

String mode = "seq";

//------------------loop ----------------
void loop() {

  digitalWrite(IC_RST, HIGH);

  enable = digitalRead(ENABLE_PIN);
  boolean buttonPressed = digitalRead(BUTTON);
  // digitalWrite(IC_RST, HIGH); // no reset
  if(!enable) {
    Serial.println("disable");
    digitalWrite(IC_ENABLE, LOW);
  }
  if(enable) {
    digitalWrite(IC_ENABLE, HIGH);
    if(buttonPressed){
      if(count < 12)count++;
      else count = 1;
      EEPROM.write(countAddr, count);
      EEPROM.write(savedAddr, 1);
    }


    if(digitalRead(MODE_SWITCH)) mode = "seq";
    else mode = "all";

    if (mode == "seq") sequenceModeProcess();
    else allinModeProcess();

    showTextNumber("Dev #: ", count, "Mode: ", mode);


    delay(50);
  }
}






//--------------functions ------------

void resetProgram(){
  digitalWrite(output, LOW);
  setup();
}


void copy(int* src, int* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}


boolean check_condition(int *arr) {
  for (int n = 0; n < count; n++) {

    if (arr[n] != 1) {
      return false;
    }
  }
  return true;
}

void allinModeProcess() {
  for (int i = 0; i < count; i++) {
    int input = digitalRead(ICpins[i]);
    reading[i] = input;
  }

  if (check_condition(reading)) {
    passAction();
  }
}

void removePin(int i, int array[MAX_NR_OF_SLAVES]){
  int tmp[MAX_NR_OF_SLAVES];
  int cnt = 0;
  for(int ind = 0; ind < (count - tapCount); ind++){
    if(i != array[ind]){
      tmp[cnt] = array[ind];
      cnt++;
    }
  }
  copy(tmp, array, MAX_NR_OF_SLAVES);
}

void sequenceModeProcess() {
  // digitalWrite(IC_RST, LOW);
  Serial.print("seqCount: ");
  Serial.print(seqCount);
  Serial.print("   toCheckPins: ");
  for (int i = 0; i < count; i++) {
    /* code */
    Serial.print(toCheckPins[i]);
  }
  // Serial.println();

  Serial.print("   pinsStatus: ");
  for (int i = 0; i < count; i++) {
    /* code */
    Serial.print(pinsStatus[i]);
  }
  // Serial.println();

  Serial.print("   pin 2 reading: ");
  Serial.print(digitalRead(toCheckPins[0]));
  Serial.print("   cardCount ");
  Serial.println(cardCount);
  int unchecked = 0;
  int checked = 1;

  if(cardCount >= count) {
    if(sequenceResult) {
      Serial.println("right seq");
      passAction();
    }
    else{ // wrong sequence
      Serial.println("wrong seq");

      if(checkEmpty(toCheckPins)){
        delay(1500);
        if(checkEmpty(toCheckPins)){ // if cards removed
          rstReaders();
          sequenceResult = true;

          seqCount = 0; // reset checking sequence
          cardCount = 0;
          for (int i = 0; i < count; i++) {
            /* code */
            pinsStatus[i] = 0;
          }
        }
      }
    }
  }
  for (int pinIndex = 0; pinIndex < count; pinIndex++) {
    if(pinsStatus[pinIndex]== unchecked && digitalRead(toCheckPins[pinIndex]) == 1){ // if get reading at pinindex
      cardCount++;
      pinsStatus[pinIndex] = checked;

      if(targetSeq[seqCount] == (toCheckPins[pinIndex]-2)){ // if pin at pinindex is in sequence
        seqCount++; // move on checking sequence
      }else{ // if pin is in wrong sequence
        sequenceResult = false;
        Serial.println("already wrong");
        // break;

      }
    }
  }

}

void passAction() {
  digitalWrite(output, HIGH);
}

void scanSlaves() {
  Serial.println ();
  Serial.println ("I2C scanner. Scanning ...");
  Wire.begin();
  showText("Scanning..");

  for (byte i = 1; i < MAX_NR_OF_SLAVES; i++)
  {

    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      slaveArray[count] = i;
      count++;

    } // end of good response
    delay (5);  // give devices time to recover
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
}

void initDisplay() {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  display.clearDisplay();
  display.setRotation(0);
  display.dim(0);
}


void showTextNumber(String s, int num, String s2, String s3) {
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
  display.print(s2);
  display.println(s3);
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

void rstReaders(){
//  nRST = LOW;
  Serial.println("Set to LOW");
  digitalWrite(IC_RST, LOW);

  delay(100);
}

bool checkEmpty(int *toCheckPins){
  for (size_t i = 0; i < count; i++) {
    if(digitalRead(toCheckPins[i])) return false;
  }
  return true;
}

