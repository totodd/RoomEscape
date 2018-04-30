

![alt text](https://www.arduino.cc/en/uploads/Tutorial/button.png)

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


in this application

Pin arrangement:

SS1 -> Pin 10
SS2 -> Pin 8
indicater1 -> Pin 2
indicater2 -> Pin 3
output -> Pin 4
reset -> Pin 5

variables:

NR_OF_READERS 	-> number of readers 
ssPins 			-> reader list 				
targetSequence 	-> the truth sequence of tapping readers
passAction() 	-> action after getting correct sequence
failAction() 	-> action after getting wrong sequence



