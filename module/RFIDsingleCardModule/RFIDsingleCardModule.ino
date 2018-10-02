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


#include <EEPROM.h>

#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above

//each SS_x_PIN variable indicates the unique SS pin for another RFID reader
#define SS_PIN        2         // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 2
#define modeSwitch 4

#define MAX_NR_OF_CARDS  12

#define output 6
#define outputIndicator 6
byte count = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

boolean cardPresentState = false;
boolean settingMode = false;
String read_rfid;
String cardStored[MAX_NR_OF_CARDS];
int storedCount = 0;
int scanCount = 0;

int ledState = LOW;
boolean isBlinking = false;


unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 500;
/**
   Initialize.
*/
void setup() {

  delay(1000);
  pinMode(output, OUTPUT);
  pinMode(outputIndicator, OUTPUT);

  pinMode(modeSwitch, INPUT_PULLUP);
  digitalWrite(output, LOW);
  digitalWrite(outputIndicator, LOW);
  autoSetAddr();

  // readSavedInfoROM();

  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  mfrc522.PCD_DumpVersionToSerial();

}

/**
   Main loop.
*/
void loop() {
  // if(digitalRead(modeSwitch) == 0){
  //   settingMode = !settingMode;
  // }
  if (storedCount == 0) settingMode = true;
  else settingMode = false;

  Serial.print("Setting mode: ");
  Serial.print(settingMode);
  Serial.print("  storedCount:");
  Serial.print(storedCount);
  Serial.print("  cardStored 0: ");

  Serial.print(cardStored[0]);
  Serial.print(" card: ");
  Serial.println(mfrc522.uid.size);

  if(settingMode && !isBlinking) blinkLED();
  if(!settingMode) stopBlink();


  if (settingMode) {
    // if (!isBlinking) blinkLED();
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      Serial.println("card sensed");

      cardPresentState = true;
      dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);


      if (!inArray(read_rfid, cardStored)) {
        cardStored[storedCount] = read_rfid;

        write_StringEE(storedCount, read_rfid);
        // EEPROM.write(storedCount, read_rfid);
        storedCount++;
      } else {
        if (!inArray(read_rfid, cardStored)) waitAction();
        else {
          if (read_rfid == cardStored[scanCount]) {
            scanCount++;
            if (scanCount == storedCount) passAction();
            else return;
          } else {
            scanCount = 0;
          }
        }
      }
    }  else if (!mfrc522.PICC_ReadCardSerial()) {
      cardPresentState = false;
      waitAction();
    }
  } else {
    // if (isBlinking) stopBlink();
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      cardPresentState = true;
      dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
      if (!inArray(read_rfid, cardStored)) waitAction();
      else {
        if (read_rfid == cardStored[scanCount]) {
          scanCount++;
          if (scanCount == storedCount) passAction();
          else return;
        } else {
          scanCount = 0;
        }
      }
    }  else if (!mfrc522.PICC_ReadCardSerial()) {
      cardPresentState = false;
      waitAction();
    }
  }

  // Serial.println(cardPresentState);
  delay(100);

}


void passAction() {
  digitalWrite(output, HIGH);
  digitalWrite(outputIndicator, HIGH);
  delay(3000);
  scanCount = 0;
}

void waitAction() {
  digitalWrite(output, LOW);
  digitalWrite(outputIndicator, LOW);
  isBlinking = false;

}

void stopBlink() {
  digitalWrite(outputIndicator, LOW);
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

boolean inArray(String id, String *array) {
  for (int n = 0; n < storedCount; n++) {

    if (array[n] == id) {
      return true;
    }
  }
  return false;
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

void autoSetAddr() {
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
      if (i > maxAddr) maxAddr = i;
    }// end of good response
    delay (5);  // give devices time to recover
  }
  myAddr = maxAddr + 1;
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

void readSavedInfoROM(){
  int cnt = 0;
  for (int n = 0; n < MAX_NR_OF_CARDS; n++) {
    cardStored[n] =  read_StringEE(0, 8);//Read String starting a address 0 with given lenth of String

  }
}



/*
 Test code for writing a String to the EEPROM and then reading it Back
 Written by Mario Avenoso, M-tech Creations
 4/10/16
*/

#include 

const int EEPROM_MIN_ADDR = 0;
const int EEPROM_MAX_ADDR = 511;



//http://ediy.com.my/index.php/tutorials/item/68-arduino-reading-and-writing-string-to-eeprom



// Returns true if the address is between the
// minimum and maximum allowed values, false otherwise.
//
// This function is used by the other, higher-level functions
// to prevent bugs and runtime errors due to invalid addresses.
boolean eeprom_is_addr_ok(int addr) {
 return ((addr &gt;= EEPROM_MIN_ADDR) &amp;&amp; (addr &lt;= EEPROM_MAX_ADDR));
}


// Writes a sequence of bytes to eeprom starting at the specified address.
// Returns true if the whole array is successfully written.
// Returns false if the start or end addresses aren't between
// the minimum and maximum allowed values.
// When returning false, nothing gets written to eeprom.
boolean eeprom_write_bytes(int startAddr, const byte* array, int numBytes) {
 // counter
 int i;

 // both first byte and last byte addresses must fall within
 // the allowed range
 if (!eeprom_is_addr_ok(startAddr) || !eeprom_is_addr_ok(startAddr + numBytes)) {
 return false;
 }

 for (i = 0; i &lt; numBytes; i++) {
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
 while ((ch != 0x00) &amp;&amp; (bytesRead &lt; bufSize) &amp;&amp; ((addr + bytesRead) &lt;= EEPROM_MAX_ADDR)) { // if no stop condition is met, read the next byte from eeprom ch = EEPROM.read(addr + bytesRead); buffer[bytesRead] = ch; // store it into the user buffer bytesRead++; // increment byte counter } // make sure the user buffer has a string terminator, (0x00) as its last byte if ((ch != 0x00) &amp;&amp; (bytesRead &gt;= 1)) {
 buffer[bytesRead - 1] = 0;
 }

 return true;
}

//Takes in a String and converts it to be used with the EEPROM Functions
//
bool write_StringEE(int Addr, String input)
{
 char cbuff[input.length() + 1];
 input.toCharArray(cbuff, input.length() + 1);
 return eeprom_write_string(Addr, cbuff);
}

//Given the starting address, and the length, this function will return
//a String and not a Char array
String read_StringEE(int Addr, int length)
{
 
 char cbuff[length+1];
 eeprom_read_string(Addr, cbuff, length+1);
 
 String stemp(cbuff);
 return stemp;
 
}





