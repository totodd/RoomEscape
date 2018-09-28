/*********************************************************************
  This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

  This example is for a 128x32 size display using I2C to communicate
  3 pins are required to interface (2 I2C and one reset)

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



Adafruit_SSD1306 display;



#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif



void setup()   {
  Serial.begin(9600);
  initDisplay();
}

  int num = 1;

void loop() {
  num++;
  showTextNumber("Current #: ", num);
  delay(100);

}

void initDisplay(){
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

