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




#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above

//each SS_x_PIN variable indicates the unique SS pin for another RFID reader
#define SS_PIN        2         // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 2



#define output 6
 byte count = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

boolean cardPresentState = false;

/**
 * Initialize.
 */
void setup() {

  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);  
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
void loop() {


  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    cardPresentState = true;

  }
  else if (!mfrc522.PICC_ReadCardSerial()){
    cardPresentState = false;
  }



  if(cardPresentState){
    digitalWrite(output,HIGH);
        delay(3000);

  }else{
    digitalWrite(output,LOW);
  }

  Serial.println(cardPresentState);
  delay(100);

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

