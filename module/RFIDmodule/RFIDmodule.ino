/**
  *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9                      D9             
 * SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 */



#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above

//each SS_x_PIN variable indicates the unique SS pin for another RFID reader
#define SS_PIN        2         // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 2
#define modeSwitch 4

#define MAX_NR_OF_CARDS  12

#define output 6
#define outputIndicator 7
 byte count = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

boolean cardPresentState = false;
boolean settingMode = false;
String read_rfid;
String cardStored[MAX_NR_OF_CARDS];
int storedCount = 0;
int scanCount = 0;

unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 5000;  
/**
 * Initialize.
 */
void setup() {

  delay(1000);
  pinMode(output, OUTPUT);
  pinMode(outputIndicator, OUTPUT); 

  pinMode(modeSwitch, INPUT_PULLUP);
  digitalWrite(output, LOW);  
  digitalWrite(outputIndicator, LOW);  
  autoSetAddr();


  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  mfrc522.PCD_DumpVersionToSerial();

}

/**
 * Main loop.
 */
boolean buttonActive = false;
boolean longPressActive = false;
long buttonTimer = 0;
long longPressTime = 10;
void loop() {
  if(digitalRead(modeSwitch) == 0){
    if(buttonActive == false){
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
      longPressActive = true;
      settingMode = !settingMode;
    }
  }else{
    buttonActive = false;
    longPressActive = false;
  }
// Serial.println(cardStored[0]);
 
  if(settingMode){
    blinkLED(true);
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      cardPresentState = true;
      dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
      if(!inArray(read_rfid, cardStored)){
        cardStored[storedCount] = read_rfid;
        storedCount++;
      if(settingMode){
        blinkLED(1);
        if(!inArray(read_rfid, cardStored)){
          cardStored[storedCount] = read_rfid;
          storedCount++;
        }
      }else{
        blinkLED(0);
        if(!inArray(read_rfid, cardStored)) waitAction();
        else{
          if(read_rfid == cardStored[scanCount]){
            scanCount++;
            if(scanCount == storedCount) passAction();
            else return;
          }else{
            scanCount = 0;
          }
        }
      }
    }  else if (!mfrc522.PICC_ReadCardSerial()){
    cardPresentState = false;
    waitAction();
  }
  }else{
    blinkLED(false);
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      cardPresentState = true;
      dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
      if(!inArray(read_rfid, cardStored)) waitAction();
      else{
        if(read_rfid == cardStored[scanCount]){
          scanCount++;
          if(scanCount == storedCount) passAction();
          else return;
        }else{
          scanCount = 0;
        }
      }
    }  else if (!mfrc522.PICC_ReadCardSerial()){
    cardPresentState = false;
    waitAction();
  }
  }

  // Serial.println(cardPresentState);
  delay(100);

}
}

void passAction(){
  digitalWrite(output,HIGH);
  digitalWrite(outputIndicator, HIGH);
  delay(3000);
  scanCount = 0;
}

void waitAction(){
  digitalWrite(output,LOW);
  digitalWrite(outputIndicator, LOW);

}

void blinkLED(boolean state){
  int ledState = LOW;
  if(state){
      unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(outputIndicator, ledState);
  }
  }
  else{
    digitalWrite(outputIndicator, LOW);
  }
  Serial.println(ledState);

}

boolean inArray(String id, String *array){
  for (int n = 0; n < storedCount; n++) {

    if (array[n] != id) {
      return false;
    }
  }
  return true;
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  read_rfid = "";
  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid  + String(buffer[i], HEX);
  }
}

void requestEvent() {
  Wire.write(cardPresentState); // respond with message of 6 bytes
  // as expected by master
}

void autoSetAddr(){
   Serial.println ();
  Serial.println ("I2C scanner. Scanning ...");
   int myAddr = 1;
  int maxAddr = 0;
  Wire.begin();
 for (byte i = 1; i < 30; i++)
 {
   Wire.beginTransmission (i);
   if (Wire.endTransmission () == 0)
     {
     Serial.print ("Found address: ");
     Serial.print (i, DEC);
     Serial.print (" (0x");
     Serial.print (i, HEX);
     Serial.println (")");
     count++;
     if(i>maxAddr) maxAddr = i;
     }// end of good response
    delay (5);  // give devices time to recover
 } 
 myAddr = maxAddr+1;
  // end of for loop
 Serial.println ("Done.");
 Serial.print ("Found ");
 Serial.print (count, DEC);
 Serial.println (" device(s).");


   Serial.print ("my Addr is ");
 Serial.println (myAddr);

  Wire.begin(myAddr);                // join i2c bus with address #8
  // Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event


}


