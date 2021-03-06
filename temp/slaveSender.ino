// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

#define input 8

void setup() {
 Serial.begin (9600);
 Serial.println ();
 Serial.println ("I2C scanner. Scanning ...");
 byte count = 0;
 int myAddr = 1;
 myAddr = autoSetAddr();

  Wire.begin(myAddr);                // join i2c bus with address #8
  // Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event

 pinMode(input, INPUT);

}

void loop() {
  
  delay(100);
}

void requestEvent() {
  Wire.write(1); // respond with message of 6 bytes
  // as expected by master
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
// void receiveEvent(int howMany) {
//   while (1 < Wire.available()) { // loop through all but the last
//     char c = Wire.read(); // receive byte as a character
//     Serial.print(c);         // print the character
//   }
//   int x = Wire.read();    // receive byte as an integer
//   Serial.println(x);         // print the integer
// }






int autoSetAddr(){
  int tempAddr = 1;
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
 tempAddr = maxAddr+1;
  // end of for loop
 Serial.println ("Done.");
 Serial.print ("Found ");
 Serial.print (count, DEC);
 Serial.println (" device(s).");


   Serial.print ("my Addr is ");
 Serial.println (tempAddr);
 return tempAddr;
}
