// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define output 10
#define MAX_NR_OF_SLAVES    20
#define IC1 5

int count = 0;
int slaveArray[MAX_NR_OF_SLAVES];
void setup() {
  Serial.begin (9600);
  initDisplay();

  scanSlaves();

  pinMode(output, OUTPUT);
  pinMode(IC1, INPUT);
  delay(1000);

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

  showTextNumber("Devices: ", count);

  if(digitalRead(IC1)) digitalWrite(output, HIGH);
  else digitalWrite(output, LOW);

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

void scanSlaves() {
  Serial.println ();
  Serial.println ("I2C scanner. Scanning ...");
  Wire.begin();
  showText("Scanning..");

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

void initDisplay() {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  display.clearDisplay();
  display.setRotation(0);
  display.dim(0);
}


void showTextNumber(String s, int num) {
  // display.setFont(&FreeMonoBold12pt7b);  // Set a custom font
  char string[10];

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setTextWrap(true);
  dtostrf(num, 3, 0, string);

  display.setCursor(0, 0);
  display.clearDisplay();
  display.print(s);
  display.println(string);
  display.display();
  // delay(1);
}


void showText(String s) {
  // display.setFont(&FreeMonoBold12pt7b);  // Set a custom font
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setTextWrap(true);

  display.setCursor(0, 0);
  display.clearDisplay();
  display.println(s);
  display.display();
  // delay(1);
}

