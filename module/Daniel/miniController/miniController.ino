/*
   This is the main logic used for magnetic detection
   The number of how many sensors can be defined easily
   Similarly, metal detection can also be used using this.
*/
#define mSensor1 2
#define mSensor2 3
#define mSensor3 4
#define mSensor4 5


#define OutputSignal1  10  //External Control Pin (current using LED)
#define OutputSignal2 11
#define Reset  A1   //External Reset (highest level)
#define CurrentState  12    //To indicate current state of this model, High: Working; LOW: Idel
#define switches1 6
#define switches2 7


int mSensor[] = {mSensor1, mSensor2, mSensor3, mSensor4};
int switches[] = {switches1, switches2};

int modeNumebr = 0;
int mRegister[4];

void setup() {
  Serial.begin(9600);
  for (int thisSignal = 0; thisSignal < 4; thisSignal++) {
    pinMode(mSensor[thisSignal], INPUT);
  }
  for (int thisswitch = 0; thisswitch < 2; thisswitch++) {
    pinMode(switches[thisswitch], INPUT_PULLUP);
  }
  pinMode(OutputSignal1, OUTPUT);
  pinMode(OutputSignal2,OUTPUT);
  pinMode(Reset, INPUT);
  pinMode(CurrentState, OUTPUT);
  digitalWrite(OutputSignal2,HIGH);
}



void multiple(int number) {

  for (int n = 0; n < number; n++) {
    if (digitalRead(mSensor[n]) == HIGH) mRegister[n] = 1;
  }
  if (check_inputCondition(number, mRegister)) {
    digitalWrite(OutputSignal1, HIGH);
    digitalWrite(OutputSignal2,LOW);
    delay(10000);
    for (int n = 0; n < number; n++) {
      mRegister[n] = 0;
    }

  } else {
    digitalWrite(OutputSignal1, LOW);
    digitalWrite(OutputSignal2,HIGH);
  }
}

  boolean check_inputCondition(int number, int *arr) {
    for (int n = 0; n < number; n++) {
      if (arr[n] != 1) {
        return false;
      }
    }
    return true;
  }

  void loop() {

    for (int n = 0; n < 3; n++) {
      Serial.print(digitalRead(mSensor[n]));
    }

    for (int n = 0; n < 3; n++) {
      Serial.print(mRegister[n]);
    }
    Serial.println();

    /*if (digitalRead(switches[1]) == LOW) {
      Serial.println("This is mode 1");
      modeNumebr = 2;
    }
    else   if (digitalRead(switches[0]) == LOW) {
      Serial.println("This is mode 3");
      modeNumebr = 4;
    }
    else {
      Serial.println("This is mode 2");
      modeNumebr = 3;

    }*/
    
    multiple(2);
  }
