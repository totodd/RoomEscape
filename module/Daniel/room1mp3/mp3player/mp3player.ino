/*
   THIS FILE FOR "KONGBU TONGYAO" ONLY

   Tong Yao will play "TongYaoDuration" + "Lightingtime"*"LightingDuration"

*/


#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <EEPROM.h>

// Variable defination for lighting and TongYao time measurement.
unsigned long lightingStartTime;
unsigned long lightingDuration;
unsigned long tongyaoStartTime;
unsigned long tongyaoDuration;

// Only when this triger is high, it will play lighting system
int lightingsoundTrigger = 0;

// This variable indicates which lighting is playing, the first one, second one or third one.
int LightingTime = 0;

// This variable defines how many times lighting system should play during two TongYao
int FixedLightingTime = 2;

// This variable defines which TongYao is playing so far
int currentTongYao = 0;

// Number of Total Tong Yao
int totalTongYao = 10;

// This pin defines whether it is playing or not
int Busy = 2;

// This pin is used to send to main controller to controll the whole electornicity system, high means should off, low means should on
int Electricity = 9;  // HIGH means cut off
int Lighting = 3;

void setup () {
  Serial.begin (9600);
  mp3_set_serial (Serial);    //set Serial for DFPlayer-mini mp3 module
  mp3_set_volume (20);
  pinMode(Busy, INPUT);
  pinMode(Electricity, OUTPUT);
  pinMode(Lighting,OUTPUT);

}

void loop() {
  combination(5000);
  while (1) {
    lightingMode(5000);
    lightingDuration = 0;
  }

}


/*
   This is the fucntion which controls lighting system

   parameter: a, means how long will each lighting play,

   for example, if a = 2000, it means, the lighting system will

   use 2 minutes to play, it will wait 2 minutes and then play


*/
void lightingMode(int a) {

  // initialize the lighting start time is the time when the funciton is called
  lightingStartTime = millis();
  // while until reach a m seconds
  while (lightingDuration < a) {
    // Calculate the current duration from the function is called
    lightingDuration = millis() - lightingStartTime;
    // This trigger used to control only sound can be played once.
    lightingsoundTrigger = 1;
  }

  // When reaches two minutes
  if (lightingsoundTrigger == 1) {
    // Play the lighting sound
    mp3_play(11);
    digitalWrite(Lighting,HIGH);
    Serial.println("This is lighting");
  }
}

/*
   This is the function which controlls TongYao playing system

   Parameter: a, means which TongYao should play so far.

   for example, 0 will play the first one and 1 will play the second one.
*/
// Play the TongYao mode, will loop the 10 Tongyao, 10 minutes each.
void Tongyaomode(int a) {
  // Keep running this mode, until all 10 Tongyao is finished
  mp3_play(a + 1);

  Serial.print("This is TongYao");
  Serial.print(a);
  Serial.println();


  // Play the lighting
  while (LightingTime < FixedLightingTime) {
          // The following two scentence need to work together to make it play again and again.
              digitalWrite(Lighting,LOW);
    lightingMode(5000);
    lightingDuration = 0;
    LightingTime++;
    // When the first lighting finished, cut off the whole electroncity system (send signal to main controller)
    if (a == 0 && LightingTime == 2) {
      digitalWrite(Electricity, HIGH);
    }
  }
  LightingTime = 0;
}


/*
    This funciton is used to combine lighting system and TongYao playing, so that

    They can act as a whole.

    parameter, a: use to define how long the next TongYao should wait after the last lighting.

*/
void combination(int a) {

  // check as long as not all TongYao are played, then loop
  while (currentTongYao < totalTongYao) {

    // Call this function to play lighting (3 times) and TongYao (1 time)
    Tongyaomode(currentTongYao);

    /* Measure the time between any two TongYao,When the duration is larger than 2 mins
    ** The reason why it is 2 minus is because, 2 times lighting will use 6 minutes, and
    ** the TongYao will use 2 minus, in order to get the full 10 minus, need to wait another
    ** 2 minus
    * */
    tongyaoStartTime = millis();
    while (tongyaoDuration < a) {
      tongyaoDuration = millis() - tongyaoStartTime;
    }
    currentTongYao ++;
    tongyaoDuration = 0;
  }
}
