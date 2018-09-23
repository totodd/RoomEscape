// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

#define MAX_NR_OF_SLAVES    20
int count = 0;
int slaveArray[MAX_NR_OF_SLAVES];

#define output 10

void setup() {
  Serial.begin (9600);
  scanSlaves();

  pinMode(output, OUTPUT);

}

int reading[MAX_NR_OF_SLAVES];

/////////LOOOOOP/////////////////
void loop() {
  for (int i = 0; i < count; i++) {
    int readed;
    Wire.requestFrom(slaveArray[i], 1);    // request 6 bytes from slave device #8

    while (Wire.available()) { // slave may send less than requested
      readed = Wire.read(); // receive a byte as character
    }
    reading[i] = readed;
    Serial.print("slave #");
    Serial.print(slaveArray[i]);
    Serial.print(":  ");
    Serial.print(reading[i]);
    Serial.print("  ");

  }
  Serial.println();

  if (check_condition(reading)) {
    digitalWrite(output, HIGH);
  } else {
    digitalWrite(output, LOW);
  }

  delay(100);
}



boolean check_condition(int *arr) {
  for (int n = 0; n < count; n++) {

    if (arr[n] != 1) {
      return false;
    }
  }
  return true;
}

void scanSlaves(){
    Serial.println ();
  Serial.println ("I2C scanner. Scanning ...");
  Wire.begin();
  for (byte i = 1; i < MAX_NR_OF_SLAVES; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      slaveArray[count] = i;
      count++;

    } // end of good response
    delay (5);  // give devices time to recover
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");

}
