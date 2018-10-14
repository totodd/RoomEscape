/*
 * This is a double button delay controller, when both of two buttons are pressed and hold for 5 seconds,
 * The OutputSignal1 will turn to HIGH
 * 
 * NOTE: two buttons have to be pressed one by one
 */


int button1 = 2;  // Button pin set to 2
int button2 = 3;  // Second button set to 4
int OutputSignal1 = 4; // External Control Pin (current using LED)
int OutputSignal2 = 5; // External Control Pin (current using LED)
int Reset = 11;  // External Reset (highest level)
int CurrentState = 12;   // To indicate current state of this model, High: Working; LOW: Idel
int button1pressed = LOW;
int button2pressed = LOW;
int button1State;
int button2State;


// time delay
int duration = 0;
int startTime;
bool startingTimeSet = false;
void setup()
{
  Serial.begin(9600);  //
  pinMode(button1, INPUT); //
  pinMode(Reset, INPUT); //
  pinMode(OutputSignal1, OUTPUT); //
  pinMode(OutputSignal2, OUTPUT); //
  pinMode(button2, INPUT); //
}

void loop(){
  button1State = digitalRead(button1);
  button2State = digitalRead(button2);
  if (button1State == HIGH && button2State == HIGH && !startingTimeSet){
    startTime = millis();
    startingTimeSet = true;
  }
  if (button1State == HIGH && button2State == HIGH){
    duration = millis()-startTime;
    Serial.println("counting Time");
    Serial.println(duration);
  }
  else{
    startingTimeSet = false;
    duration = 0;
    Serial.println("SOME OF THE BUTTON IS LESS THAN");
    digitalWrite(OutputSignal2,LOW);
  }
  if(duration >= 3000){
    digitalWrite(OutputSignal1,HIGH);
    digitalWrite(OutputSignal2,HIGH);
    delay(10000);
  }
}
