int button = 2;  // Button pin set to 2
int OutputSignal1 = 3; // External Control Pin (current using LED)
int OutputSignal2 = 4;
int Reset = 11;  // External Reset (highest level)
int CurrentState = 12;   // To indicate current state of this model, High: Working; LOW: Idel

unsigned long startTime; // starttime when button pressed
unsigned long duration;  // duration for button pressed
unsigned long debounceDelay = 50;    //Debounce thereshoul

void setup()
{
  Serial.begin(9600);  //
  pinMode(button, INPUT); //
  pinMode(Reset, INPUT); //
  pinMode(OutputSignal1, OUTPUT); //
  pinMode(OutputSignal2, OUTPUT);
}




void Delay_button(int a){                                     // Parameter a indicates how long the button will be delayed
  if (digitalRead(button) == HIGH){
    startTime = millis();
    while (digitalRead(button) == HIGH){
      duration = millis() - startTime; 
      if (duration >= a){
        digitalWrite(OutputSignal1,HIGH);                       // If button is pressed over 5s, the external control output turn to HIGH
        digitalWrite(OutputSignal2,HIGH);
        //delay(10000);
        //digitalWrite(OutputSignal,LOW);
      }
      else if (duration < a && duration >= debounceDelay){
        digitalWrite(OutputSignal2,LOW);
        digitalWrite(OutputSignal1, LOW);                        // If button is pressed less than 5s, the external control output turn to LOW
      }
    }
  }
}

void loop()
{                                                 
      Delay_button(3000); 
}
