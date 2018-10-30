
#include <Keypad.h>
#include <DFPlayer_Mini_Mp3.h>

#include <SoftwareSerial.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','3'},
  {'4','5','6','6'},
  {'7','8','9','9'},
  {'*','0','#','#'}
};
byte rowPins[ROWS] = { 8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 11, 10,9  }; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  mp3_set_serial (Serial);    //set Serial for DFPlayer-mini mp3 module
  mp3_set_volume (30);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  mp3_play(1);
  delay(1000);
  if (customKey){
    Serial.println(customKey);
  }
}
