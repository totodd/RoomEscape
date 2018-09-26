/*
 * This is a double button delay controller, when both of two buttons are pressed and hold for 5 seconds,
 * The outputSignal will turn to HIGH
 * 
 * NOTE: two buttons have to be pressed one by one
 */


int button1 = 2;  // Button pin set to 2
int button2 = 4;  // Second button set to 4
int OutputSignal = 3; // External Control Pin (current using LED)
int Reset = 11;  // External Reset (highest level)
int CurrentState = 12;   // To indicate current state of this model, High: Working; LOW: Idel
int button1pressed = LOW;
int button2pressed = LOW;

unsigned long startTime; // starttime when button pressed
unsigned long duration;  // duration for button pressed
unsigned long debounceDelay = 50;    //Debounce thereshoul

void setup()
{
  Serial.begin(9600);  //
  pinMode(button1, INPUT); //
  pinMode(Reset, INPUT); //
  pinMode(OutputSignal, OUTPUT); //
  pinMode(button2, INPUT); //
}



void reset(){
  digitalWrite(OutputSignal,LOW);
}

void Delay_button(int a){                                     // Parameter a indicates how long the button will be delayed
  if (digitalRead(button1) == HIGH){
    startTime = millis();
    while (digitalRead(button1) == HIGH){
      duration = millis() - startTime; 
      if (duration >= a){
        //digitalWrite(OutputSignal,HIGH);                       // If button is pressed over 5s, the external control output turn to HIGH
        button1pressed = HIGH;
        Serial.println("Button 1 pressed");
      }
      else if (duration < a && duration >= debounceDelay){
       // digitalWrite(OutputSignal, LOW);                        // If button is pressed less than 5s, the external control output turn to LOW
        button1pressed = LOW;
      }
    }
  }
 if (digitalRead(button2) == HIGH){
    startTime = millis();
    while (digitalRead(button2) == HIGH){
      duration = millis() - startTime; 
      if (duration >= a){
        //digitalWrite(OutputSignal,HIGH);                       // If button is pressed over 5s, the external control output turn to HIGH
        button2pressed = HIGH;
        Serial.println("Button 2 pressed");
      }
      else if (duration < a && duration >= debounceDelay){
       // digitalWrite(OutputSignal, LOW);                        // If button is pressed less than 5s, the external control output turn to LOW
        button2pressed = LOW;
      }
    }
  }
  if (button1pressed == HIGH  && button2pressed == HIGH) {
              digitalWrite(OutputSignal,HIGH); 
              delay(500);
              button1pressed = LOW;
              button2pressed = LOW;
              Serial.println("READY");
              digitalWrite(OutputSignal,LOW); 
                  
    }
}

void loop()
{
  if (digitalRead(Reset) == HIGH){                            
      reset();
  }
  else {                                                  
      Delay_button(5000);
  }
}
