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
 * output Indicator         A4
*/


#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above

//each SS_x_PIN variable indicates the unique SS pin for another RFID reader
#define SS_PIN        2         // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 2



#define output A4

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

boolean cardPresentState = false;
int lastButtonState = LOW;   // the previous reading from the input pin
int buttonState;  
/**
 * Initialize.
 */
void setup() {

  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);  


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

  lastButtonState = cardPresentState;

}


