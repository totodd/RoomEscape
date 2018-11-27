/**

   Typical pin layout used:
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9                      D9
   SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required *
   SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required *
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15

*/

// change SDA SCL pin to active pin
//

#include <EEPROM.h>

#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above

//each SS_x_PIN variable indicates the unique SS pin for another RFID reader
#define SS_PIN        2         // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 2
#define MODE_SWITCH 4

#define MAX_NR_OF_CARDS  10
// #define ENABLE_PIN A4
#define output 7
#define outputIndicator 6
byte count = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String savedKey = "cardstor";
boolean cardPresentState = false;
boolean settingMode = false;
String read_rfid;
String cardStored[MAX_NR_OF_CARDS];
String tempStored[MAX_NR_OF_CARDS];
int storedCount = 0;
int scanCount = 0;

int ledState = LOW;
boolean isBlinking = false;

const String masterCardIDs[] ={"8a677b89","95adf17","96a2e11","4973ea17","8949c18","899cec17","59c22b17","a9682b11","49e83c11","9c3e917"};

unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long previousMillisCard = 0;
// constants won't change:
const long interval = 500;
const long flashinterval = 80;
const long settingInterval = 3000;
/**d
   Initialize.
*/
void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  pinMode(output, OUTPUT);
  pinMode(outputIndicator, OUTPUT);
  // pinMode(ENABLE_PIN, INPUT);
  pinMode(MODE_SWITCH, INPUT_PULLUP);
  digitalWrite(output, LOW);
  digitalWrite(outputIndicator, LOW);
  // autoSetAddr();


  readSavedInfoROM();


  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  mfrc522.PCD_DumpVersionToSerial();

}

// bool disable;
/**
   Main loop.
*/
void loop() {


    Serial.print("Setting mode: ");
    Serial.print(settingMode);
    Serial.print("  storedCount:");
    Serial.print(storedCount);
    Serial.print("  cardStored 0: ");

    Serial.print(cardStored[0]);
    Serial.print(" card: ");
    Serial.print(mfrc522.uid.size);
    Serial.print(" card scanned id:");
    Serial.print(read_rfid);
    Serial.print(" scanned #:");
    Serial.println(scanCount);



    if (settingMode && storedCount == 0) digitalWrite(outputIndicator, HIGH);
    if (!settingMode) digitalWrite(outputIndicator, LOW);


    if (settingMode) {
      // if (!isBlinking) blinkLED();
      if (((millis() - previousMillisCard) >= settingInterval) && (storedCount > 0)) {
        write_StringEE(MAX_NR_OF_CARDS, savedKey);
        EEPROM.write((MAX_NR_OF_CARDS + 1) * 8 + 1, storedCount);
        settingMode = false;
      }

      if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        Serial.println("card sensed");


        cardPresentState = true;
        dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
        Serial.println(isMaster(read_rfid));

        if (!isMaster(read_rfid)) {
          Serial.println("not master");

          if (!inArray(read_rfid, cardStored)) {
            cardStored[storedCount] = read_rfid;

            write_StringEE(storedCount, read_rfid);
            // EEPROM.write(storedCount, read_rfid);
            storedCount++;
            flashLED();
            previousMillisCard = millis();

          }
        }
      }
    }
    if (!settingMode) {
      // if (isBlinking) stopBlink();
      if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

        cardPresentState = true;
        dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
        //mastercard scanned
        if (isMaster(read_rfid)) {
          storedCount = 0;
          scanCount = 0;
          for (int n = 0; n < MAX_NR_OF_CARDS; n++) {
            cardStored[n] = "";
          }
          read_rfid = "";
          for (int n = 0; n < EEPROM.length(); n++) {
            EEPROM.write(n, 0);
          }
          settingMode = true;
          // setup();
          digitalWrite(output, LOW);
          digitalWrite(outputIndicator, LOW);
        }

        // check pass condition
        if (!inArray(read_rfid, tempStored) && inArray(read_rfid, cardStored)) {
          tempStored[scanCount] = read_rfid;
          scanCount++;
        }

        //  else if (!mfrc522.PICC_ReadCardSerial()) {
        //   cardPresentState = false;
        //   waitAction();
        // }
      }
      if(scanCount == storedCount && storedCount != 0){
            if(isPassed()) passAction();
            else {
              scanCount = 0;
              for(int n = 0; n < storedCount; n++){
                tempStored[n] = "";
              }
            }
      }

      // Serial.println(cardPresentState);
//      delay(100);

    }

}

boolean isPassed() {
  for(int n = 0; n < storedCount; n++){
    if(tempStored[n] != cardStored[n]) return false;
  }
  return true;
}

void passAction() {
  digitalWrite(output, HIGH);
  digitalWrite(outputIndicator, HIGH);
//  delay(1500);
//  digitalWrite(output, LOW);
//  digitalWrite(outputIndicator, LOW);
  scanCount = 0;
  for(int n = 0; n < storedCount; n++){
    tempStored[n] = "";
  }
}

void waitAction() {
  digitalWrite(output, LOW);
  digitalWrite(outputIndicator, LOW);
  isBlinking = false;

}

void stopBlink() {
  digitalWrite(outputIndicator, LOW);
}

void flashLED() {
  for (int n = 0; n < 3; n++) {
    digitalWrite(outputIndicator, HIGH);
    delay(flashinterval);
    digitalWrite(outputIndicator, LOW);
    delay(flashinterval);
  }
}

void blinkLED() {
  isBlinking = true;
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

boolean array_cmp(String *a, String *b, int len_a, int len_b) {
  int n;

  //if their lengths are different, return false
  if (len_a != len_b) return false;

  //test each element to be the same. if not, return false
  for (n = 0; n < len_a; n++) if (a[n] != b[n]) return false;

  //ok, if we have not returned yet, they are equal :)
  return true;
}

boolean inArray(String id, String * array) {
  for (int n = 0; n < storedCount; n++) {

    if (array[n] == id) {
      return true;
    }
  }
  return false;
}

void dump_byte_array(byte * buffer, byte bufferSize) {
  read_rfid = "";
  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid  + String(buffer[i], HEX);
  }
}

void requestEvent() {
  Wire.write(cardPresentState); // respond with message of 6 bytes
  // as expected by master
}

// void autoSetAddr() {
//   Serial.println ();
//   Serial.println ("I2C scanner. Scanning ...");
//   int myAddr = 1;
//   int maxAddr = 0;
//   Wire.begin();
//   for (byte i = 1; i < 30; i++)
//   {
//     Wire.beginTransmission (i);
//     if (Wire.endTransmission () == 0)
//     {
//       Serial.print ("Found address: ");
//       Serial.print (i, DEC);
//       Serial.print (" (0x");
//       Serial.print (i, HEX);
//       Serial.println (")");
//       count++;
//       if (i > maxAddr) maxAddr = i;
//     }// end of good response
//     delay (5);  // give devices time to recover
//   }
//   myAddr = maxAddr + 1;
//   // end of for loop
//   Serial.println ("Done.");
//   Serial.print ("Found ");
//   Serial.print (count, DEC);
//   Serial.println (" device(s).");
//
//
//   Serial.print ("my Addr is ");
//   Serial.println (myAddr);
//
//   Wire.begin(myAddr);                // join i2c bus with address #8
//   // Wire.onReceive(receiveEvent); // register event
//   Wire.onRequest(requestEvent); // register event
//
//
// }

void readSavedInfoROM() {
  // int cnt = 0;
  Serial.println("start reading rom");
  Serial.print("card saved?");
  Serial.println(read_StringEE(MAX_NR_OF_CARDS, 8));
  // card is saved in ROM
  if (read_StringEE(MAX_NR_OF_CARDS, 8) == savedKey) {
    settingMode = false;
    storedCount = EEPROM.read((MAX_NR_OF_CARDS + 1) * 8 + 1);
    Serial.print("cardCount from ROM: ");
    Serial.println(storedCount);

    Serial.print("cards saved: ");
    for (int n = 0; n < MAX_NR_OF_CARDS; n++) {
      cardStored[n] = read_StringEE(n, 8);//Read String starting a address 0 with given lenth of String
      Serial.println(cardStored[n]);
    }
  } else { //  card is not saved in ROM
    settingMode = true;
  }
}


bool write_StringEE(int Addr, String input)
{
  char cbuff[input.length() + 1]; //Finds length of string to make a buffer
  input.toCharArray(cbuff, input.length() + 1); //Converts String into character array
  return eeprom_write_string(Addr*8, cbuff); //Saves String
}

String read_StringEE(int Addr, int length)
{

  char cbuff[length + 1];
  boolean didget = eeprom_read_string(Addr * 8, cbuff, length + 1);

  String stemp(cbuff);
  if (didget) {
    Serial.print("got 1 from rom ");
    Serial.print(Addr);
    Serial.print("   ");
    Serial.println(stemp);
  }

  return stemp;

}
/*
  Test code for writing a String to the EEPROM and then reading it Back
  Written by Mario Avenoso, M-tech Creations
  4/10/16
*/

//http://ediy.com.my/index.php/tutorials/item/68-arduino-reading-and-writing-string-to-eeprom

const int EEPROM_MIN_ADDR = 0;
const int EEPROM_MAX_ADDR = 511;

// Returns true if the address is between the
// minimum and maximum allowed values, false otherwise.
//
// This function is used by the other, higher-level functions
// to prevent bugs and runtime errors due to invalid addresses.
boolean eeprom_is_addr_ok(int addr) {
  return ((addr >= EEPROM_MIN_ADDR) && (addr <= EEPROM_MAX_ADDR));
}


// Writes a sequence of bytes to eeprom starting at the specified address.
// Returns true if the whole array is successfully written.
// Returns false if the start or end addresses aren't between
// the minimum and maximum allowed values.
// When returning false, nothing gets written to eeprom.
boolean eeprom_write_bytes(int startAddr, const byte * array, int numBytes) {
  // counter
  int i;

  // both first byte and last byte addresses must fall within
  // the allowed range
  if (!eeprom_is_addr_ok(startAddr) || !eeprom_is_addr_ok(startAddr + numBytes)) {
    return false;
  }

  for (i = 0; i < numBytes; i++) {
    EEPROM.write(startAddr + i, array[i]);
  }

  return true;
}


// Writes a string starting at the specified address.
// Returns true if the whole string is successfully written.
// Returns false if the address of one or more bytes fall outside the allowed range.
// If false is returned, nothing gets written to the eeprom.
boolean eeprom_write_string(int addr, const char* string) {

  int numBytes; // actual number of bytes to be written

  //write the string contents plus the string terminator byte (0x00)
  numBytes = strlen(string) + 1;

  return eeprom_write_bytes(addr, (const byte*)string, numBytes);
}


// Reads a string starting from the specified address.
// Returns true if at least one byte (even only the string terminator one) is read.
// Returns false if the start address falls outside the allowed range or declare buffer size is zero.
//
// The reading might stop for several reasons:
// - no more space in the provided buffer
// - last eeprom address reached
// - string terminator byte (0x00) encountered.
boolean eeprom_read_string(int addr, char* buffer, int bufSize) {
  byte ch; // byte read from eeprom
  int bytesRead; // number of bytes read so far

  if (!eeprom_is_addr_ok(addr)) { // check start address
    return false;
  }

  if (bufSize == 0) { // how can we store bytes in an empty buffer ?
    return false;
  }

  // is there is room for the string terminator only, no reason to go further
  if (bufSize == 1) {
    buffer[0] = 0;
    return true;
  }

  bytesRead = 0; // initialize byte counter
  ch = EEPROM.read(addr + bytesRead); // read next byte from eeprom
  buffer[bytesRead] = ch; // store it into the user buffer
  bytesRead++; // increment byte counter

  // stop conditions:
  // - the character just read is the string terminator one (0x00)
  // - we have filled the user buffer
  // - we have reached the last eeprom address
  while ( (ch != 0x00) && (bytesRead < bufSize) && ((addr + bytesRead) <= EEPROM_MAX_ADDR) ) {
    // if no stop condition is met, read the next byte from eeprom
    ch = EEPROM.read(addr + bytesRead);
    buffer[bytesRead] = ch; // store it into the user buffer
    bytesRead++; // increment byte counter
  }

  // make sure the user buffer has a string terminator, (0x00) as its last byte
  if ((ch != 0x00) && (bytesRead >= 1)) {
    buffer[bytesRead - 1] = 0;
  }

  return true;
}

bool isMaster(String s){
  for (int i = 0; i < 10; i++) {
    if(s == masterCardIDs[i]) {
      Serial.println(s + "  " + masterCardIDs[i] + " " + "at index"+i);
      Serial.println(sizeof(masterCardIDs));
      return true;
    }
  }
  return false;
}

