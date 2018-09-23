/**
  *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
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


#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above

//each SS_x_PIN variable indicates the unique SS pin for another RFID reader
#define SS_1_PIN        10         // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 2
#define SS_2_PIN        8          // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 1
#define SS_3_PIN        7          // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 1


//must have one SS_x_PIN for each reader connected
#define NR_OF_READERS   2

//indicater of light
#define indicater1 2
#define indicater2 3
#define output 4

//reset button
#define rst 5
int lastButtonState = LOW;   // the previous reading from the input pin
int buttonState;             // the current reading from the input pin

byte ssPins[] = {SS_1_PIN, SS_2_PIN, SS_3_PIN};

MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.
String read_rfid;



//this is the pin the relay is on, change as needed for your code
int NoCardCnt = 0;
int currentSequence[NR_OF_READERS];
int cardCount = 0;

// *************customized***************

int targetSequence[] = {1,2};
int indicaters[] = {indicater1, indicater2};

String cardstr[] = {"a68e5e8e", "46ec294"};
// String card1str = "a68e5e8e";
// String card2str = "46ec294";


/**
 * Initialize.
 */
void setup() {
  pinMode(indicater1, OUTPUT);
  pinMode(indicater2, OUTPUT);
  pinMode(output, OUTPUT);

  pinMode(rst, INPUT);

  
  digitalWrite(indicater1, LOW);
  digitalWrite(indicater2, LOW);
  digitalWrite(output, LOW);


  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();        // Init SPI bus

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }

}

/**
 * Main loop.
 */
void loop() {

  int buttonReading = digitalRead(rst);
//    Serial.println(buttonReading);

  debounce(buttonReading);
  
//  Serial.println(buttonState);

  if(buttonState){
    reset();
  }

  if (NoCardCnt > 150){
    reset();
  }  
   
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Look for new cards

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {

      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);

      // currentSequence[cardCount-1] = reader+1;
      for (int i=0; i<NR_OF_READERS; i++) {
         if (read_rfid == cardstr[i]) {
           cardCount = cardCount + 1;

           break;
         }
      }

      showCurrentSequence();
      
      digitalWrite(indicaters[reader], HIGH);

  
      Serial.print(F("Reader: "));
      Serial.println(reader);
      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F("RFID:  "));
      Serial.println(read_rfid);
      

      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();

    }
    else{
      NoCardCnt = NoCardCnt + 1;
      delay(50);
    }


    if(cardCount == NR_OF_READERS){
      showCurrentSequence();
      // if(checkSequence(targetSequence, currentSequence)){
        passAction();
      // }
      // else{
      //   failAction();
      // }
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
  digitalWrite(indicater1, LOW);
  digitalWrite(indicater2, LOW);
  digitalWrite(output, LOW);
  cardCount = 0;
  NoCardCnt = 0;
  for (int i = 0; i < NR_OF_READERS; i++)
  {
    currentSequence[i] = 0;
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

void showCurrentSequence(){
  for (int i = 0; i < NR_OF_READERS; i++){
        Serial.print(currentSequence[i]);
      }
  Serial.println();

}

void passAction(){
  reset();
  Serial.println("pass");
  digitalWrite(output, HIGH);
  delay(2000); // delay 2 sec
  reset();
}

void failAction(){
  reset();
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
  unsigned long debounceDelay = 20;    // the debounce time; increase if the output flickers

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