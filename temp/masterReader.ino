// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
 byte count = 0;

#define output 10

void setup() {
   Serial.begin (9600);
 Serial.println ();
 Serial.println ("I2C scanner. Scanning ...");
 Wire.begin();
 for (byte i = 1; i < 10; i++)
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

     } // end of good response
    delay (5);  // give devices time to recover
 } // end of for loop
 Serial.println ("Done.");
 Serial.print ("Found ");
 Serial.print (count, DEC);
 Serial.println (" device(s).");

 pinMode(output, OUTPUT);

}

byte x = 0;
int reading[count]; 

/////////LOOOOOP/////////////////
void loop() {
 for (byte i = 1; i < count+1; i++){

  Wire.requestFrom(i, 6);    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    int read = Wire.read(); // receive a byte as character
    reading[i] = read;
  }
  }

  if(check_condition(reading)){
      digitalWrite(output, HIGH);
  }else{
    digitalWrite(output, LOW);
  }

}



boolean check_condition(int *arr){
  for(n=0;n<sizeof(arr);n++){
    if(arr[n]!=1){
      return false;
    }
  }
  return true;
}

