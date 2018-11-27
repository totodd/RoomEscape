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
#define ICPIN_11 12
#define ICPIN_12 13

#define MODE_SWITCH A2
#define rst A1
#define output A0

#define SEQUENCE_MODE 1
#define ALLIN_MODE 2

int ICpins[] = {ICPIN_1, ICPIN_2, ICPIN_3, ICPIN_4, ICPIN_5, ICPIN_6, ICPIN_7, ICPIN_8, ICPIN_9, ICPIN_10, ICPIN_11, ICPIN_12};

Adafruit_SSD1306 display;
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


#define MAX_NR_OF_SLAVES    12

int count;
int tapCount;
int slaveArray[MAX_NR_OF_SLAVES];
int targetSequence[MAX_NR_OF_SLAVES];
int temp[MAX_NR_OF_SLAVES];
int countAddr = 1;
int savedAddr = 0;
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


  pinMode(rst, INPUT);
  pinMode(output, OUTPUT);
  pinMode(MODE_SWITCH, INPUT);

  for (int i = 0; i < MAX_NR_OF_SLAVES; i++) {
    pinMode(ICpins[i], INPUT);
  }


  for (int i = 0; i < count; i++) {
    targetSequence[i] = i;
  }
  copy(targetSequence,temp, MAX_NR_OF_SLAVES);
}

int reading[MAX_NR_OF_SLAVES];
int sequence[MAX_NR_OF_SLAVES];

String mode = "seq";


//------------------loop ----------------
void loop() {
  boolean buttonPressed = digitalRead(rst);

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






//--------------functions ------------

void resetProgram(){
  digitalWrite(output, LOW);
  setup();
}

void resetSequnce() {
  tapCount = 0;
  copy(targetSequence,temp, MAX_NR_OF_SLAVES) ;
}

void copy(int* src, int* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

boolean checkSequence(int i, int tapCount) {
  return (targetSequence[tapCount] == i);
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
  if(tapCount == count) passAction();
  Serial.printf("tapCount = %d \n", tapCount );
  Serial.printf("count = %d \n", count );
  Serial.print("targetSequence");
  for(int i = 0; i < MAX_NR_OF_SLAVES; i++)
  {
    Serial.print(targetSequence[i]);
  }
  for(int i = 0; i < MAX_NR_OF_SLAVES; i++)
  {
    Serial.print(temp[i]);
  }

  for (int i = 0; i < (count - tapCount); i++) {
    int cardNumber = temp[i];
    int input = digitalRead(ICpins[cardNumber]);
    if (input == 1) {
      if(cardNumber == targetSequence[tapCount]){
        removePin(i, temp);
        tapCount++;
      }else{
        resetSequnce();
      }
      // if (checkSequence(i, tapCount)) {
      //   if (tapCount == (count)) return passAction();
      //   else {
      //     sequence[tapCount] = i;
      //     tapCount++;
      //   }
      // }
      // else resetSequnce();
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
