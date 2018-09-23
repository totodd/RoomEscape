/**
  *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          10                      D10             
 * SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 */

/*  Wiring up the RFID Readers ***
 *  RFID readers based on the Mifare RC522 like this one:  http://amzn.to/2gwB81z
 *  get wired up like this:
 *
 *  RFID pin    Arduino pin (above)
 *  _________   ________
 *  SDA          SDA - each RFID board needs its OWN pin on the arduino
 *  SCK          SCK - all RFID boards connect to this one pin
 *  MOSI         MOSI - all RFID boards connect to this one pin
 *  MISO         MISO - all RFID boards connect to this one pin
 *  IRQ          not used
 *  GND          GND - all RFID connect to GND
 *  RST          RST - all RFID boards connect to this one pin
 *  3.3V         3v3 - all RFID connect to 3.3v for power supply
 *
 */

/* SPI pin of each reader
 * SPI SS 1    SDA(SS)      2
 * SPI SS 2    SDA(SS)      3
 * SPI SS 3    SDA(SS)      4
 * SPI SS 4    SDA(SS)      5
 * SPI SS 5    SDA(SS)      6
 * SPI SS 6    SDA(SS)      7
 * SPI SS 7    SDA(SS)      8
 * output                   A2
*/


#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         10          // Configurable, see typical pin layout above

//each SS_x_PIN variable indicates the unique SS pin for another RFID reader
#define SS_1_PIN        2         // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 2
#define SS_2_PIN        3          // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 1
#define SS_3_PIN        4 
#define SS_4_PIN        5 
#define SS_5_PIN        6 
#define SS_6_PIN        7 
#define SS_7_PIN        8 

#define switch1 A0
#define switch2 A1


//must have one SS_x_PIN for each reader connected
#define MAX_NR_OF_READERS   7
int NR_OF_READERS = 0;
int cardCount = 0;
int NoCardCnt = 10;
int cardSetCount = 0;

int currentPlaceSequence[MAX_NR_OF_READERS];
int targetPlaceSequence[MAX_NR_OF_READERS];


//reset button
#define rst 9

//sequence setting button
#define setButton A3
int lastButtonState = LOW;   // the previous reading from the input pin
int buttonState;             // the current reading from the input pin

byte ssPins[7] = {SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN, SS_5_PIN, SS_6_PIN, SS_7_PIN};

String targetCardSequence[MAX_NR_OF_READERS];
String currentCardSequence[MAX_NR_OF_READERS];
MFRC522 mfrc522[MAX_NR_OF_READERS];   // Create MFRC522 instance.
String read_rfid;


boolean settingMode;
boolean randPlaceMode; // true if random place, false if sequence place
boolean currentMode;

#define output A2


/**
 * Initialize.
 */
void setup() {


  pinMode(rst, INPUT);
  pinMode(output, OUTPUT);
  pinMode(setButton, INPUT);

  digitalWrite(output, LOW);
  //this is the pin the relay is on, change as needed for your code
  int readerNumber = 0;
 

  randPlaceMode = true;
  settingMode = false;

  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();        // Init SPI bus

  // get amount of connected readers
  for (uint8_t reader = 0; reader < MAX_NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    int ver = mfrc522[reader].PCD_ReadRegister(mfrc522[reader].VersionReg);
    Serial.println(ver);
    if (ver != 0) {
      readerNumber++;
    } 
  }

  Serial.println(readerNumber);
  NR_OF_READERS = readerNumber;
  // default place sequence: 1,2,3,4,5...
  for(int i = 0; i < NR_OF_READERS; i++){
    targetPlaceSequence[i] = i + 1;
  }

  if(digitalRead(switch1)){
    randPlaceMode = true;
  }
  if(digitalRead(switch2)){
    randPlaceMode = false;
  }
      Serial.print("Random Place Mode?: ");
    Serial.println(randPlaceMode);
}

/**
 * Main loop.
 */
void loop() {

  int buttonReading = digitalRead(rst);
  if(buttonReading){
    reset();
  }

  int setButtonReading = digitalRead(setButton);
  debounce(setButtonReading);
  // if(setButtonReading){
  //   Serial.println("set pressed");
  // }
  if(setButtonReading){
    if(settingMode){

    }else{
      cardSetCount = 0;
      memset(targetCardSequence, 0, sizeof(targetCardSequence));
    }
    settingMode = !settingMode;
    Serial.print("Setting Mode?: ");
    Serial.println(settingMode);
  }




  // // reset if no card loading...
  // if (NoCardCnt > 150){
  //   reset();
  // }  

  // TO DO: set randPlaceMode
  currentMode = randPlaceMode;

  if(digitalRead(switch1)){
    randPlaceMode = true;
  }
  if(digitalRead(switch2)){
    randPlaceMode = false;
  }

  if(currentMode != randPlaceMode){
    Serial.print("Random Place Mode?: ");
    Serial.println(randPlaceMode);
  }



  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Look for new cards

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);

      if(settingMode){
        
        if(!checkInArray(read_rfid,targetCardSequence)){
          targetCardSequence[cardSetCount] = read_rfid;
          cardSetCount++;
        }
        Serial.println(cardSetCount);

      }else{
        cardCount++;

        currentPlaceSequence[cardCount-1] = reader+1;
        currentCardSequence[cardCount-1] = read_rfid;
        showcurrentPlaceSequence();
        Serial.println(cardCount);

      }

      // Serial.print(F("Reader: "));
      // Serial.println(reader);
      // // Show some details of the PICC (that is: the tag/card)
      // Serial.print(F("RFID:  "));
      // Serial.println(read_rfid);

      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();



    }
    else{
      NoCardCnt = NoCardCnt + 1;
      delay(50);
    }
  
  }

  if(cardCount == NR_OF_READERS){

      showcurrentPlaceSequence();
      if(randPlaceMode){
        if(checkStrSequence(targetCardSequence, currentCardSequence)){
          passAction();
        }else{
          failAction();
        }
      }else{
        if(checkSequence(targetPlaceSequence, currentPlaceSequence)){
          passAction();
        }else{
          failAction();
        }
      }
    } 
  lastButtonState = buttonReading;

}



void dump_byte_array(byte *buffer, byte bufferSize) {
  read_rfid = "";
  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid  + String(buffer[i], HEX);
  }
}



void reset(){
  Serial.println("***********reset************");
  digitalWrite(output, LOW);

  cardCount = 0;
  NoCardCnt = 0;
  for (int i = 0; i < NR_OF_READERS; i++)
  {
    currentPlaceSequence[i] = 0;
  }
}

boolean checkSequence(int target[], int current[]){
    if (sizeof(target) != sizeof(current))
    {
      return false;
    }else{
      for (int i = 0; i < sizeof(current); i++) {
        if (target[i]!=current[i])
        {
          return false;
        }
      }
      return true;
    }
}

boolean checkStrSequence(String target[], String current[]){
    if (sizeof(target) != sizeof(current))
    {
      return false;
    }else{
      for (int i = 0; i < sizeof(current); i++) {
        if (target[i]!=current[i])
        {
          return false;
        }
      }
      return true;
    }
}

void showcurrentPlaceSequence(){
  for (int i = 0; i < NR_OF_READERS; i++){
        Serial.print(currentPlaceSequence[i]);
      }
  Serial.println();
}

void passAction(){
  Serial.println("pass");
  digitalWrite(output, HIGH);
  // delay(2000); // delay 2 sec
  // reset();
}

void failAction(){
  Serial.println("fail");
  for (int i = 0; i < 3; ++i)
  {
    digitalWrite(output, HIGH);
    delay(300);
    digitalWrite(output, LOW);
    delay(300);
  }
  reset();

}

void debounce(int reading){
  unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
  unsigned long debounceDelay = 5;    // the debounce time; increase if the output flickers

    // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;


    }
  }

}

boolean checkInArray(String comp, String array[]){
  boolean y_valid = false;
  for (int x = 0; x < sizeof(array); x++) 
  {
    if (comp == array[x])
    {
       y_valid = true;
       break;
    } 
  }
  return y_valid;
}
