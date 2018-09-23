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
 * output Indicator         A4
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

#define randSwitch A0
#define outputIndicator A4
#define output A2
#define randIndicator A1


//must have one SS_x_PIN for each reader connected
#define MAX_NR_OF_READERS   7
int NR_OF_READERS = 0;
int cardCount = 0;
int NoCardCnt = 10;

int currentPlaceSequence[MAX_NR_OF_READERS];
int targetPlaceSequence[MAX_NR_OF_READERS];


//reset button
#define rst 9
boolean lastButtonState = LOW;   // the previous reading from the input pin
boolean buttonState;             // the current reading from the input pin

byte ssPins[7] = {SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN, SS_5_PIN, SS_6_PIN, SS_7_PIN};

MFRC522 mfrc522[MAX_NR_OF_READERS];   // Create MFRC522 instance.
String read_rfid;


boolean randPlaceMode; // true if random place, false if sequence place
boolean currentMode;



/**
 * Initialize.
 */
void setup() {


  pinMode(rst, INPUT);



  //this is the pin the relay is on, change as needed for your code
  int readerNumber = 0;
 

  randPlaceMode = true;

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
    Serial.println(mfrc522[reader].PCD_ReadRegister(mfrc522[reader]);
    if (ver != 0) {
      readerNumber++;
    } 
  }

  Serial.println(readerNumber);
  NR_OF_READERS = readerNumber;
 
}

/**
 * Main loop.
 */
void loop() {


}
