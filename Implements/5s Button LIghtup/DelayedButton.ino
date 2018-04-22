int button = 2;  // Button pin set to 2
int OutputSignal = 3; // External Control Pin (current using LED)
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
  pinMode(OutputSignal, OUTPUT); //
}



void reset(){
  digitalWrite(OutputSignal,LOW);
}

void Delay_button(int a){                                     // Parameter a indicates how long the button will be delayed
  if (digitalRead(button) == HIGH){
    startTime = millis();
    while (digitalRead(button) == HIGH){
      duration = millis() - startTime; 
      if (duration >= a){
        digitalWrite(OutputSignal,HIGH);                       // If button is pressed over 5s, the external control output turn to HIGH
      }
      else if (duration < a && duration >= debounceDelay){
        digitalWrite(OutputSignal, LOW);                        // If button is pressed less than 5s, the external control output turn to LOW
      }
    }
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

