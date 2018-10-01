/*
   This is the main logic used for magnetic detection
   The number of how many sensors can be defined easily
   Similarly, metal detection can also be used using this.
*/
#define mSensor1 2
#define mSensor2 3
#define mSensor3 4
#define mSensor4 5


#define OutputSignal  10  //External Control Pin (current using LED)
#define Reset  11   //External Reset (highest level)
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
  pinMode(OutputSignal, OUTPUT);
  pinMode(Reset, INPUT);
  pinMode(CurrentState, OUTPUT);
}

//function to compare two arrays.
boolean array_cmp(int *a, int *b, int len_a, int len_b) {
  int n;

  //if their lengths are different, return false
  if (len_a != len_b) return false;

  //test each element to be the same. if not, return false
  for (n = 0; n < len_a; n++) if (a[n] != b[n]) return false;

  //ok, if we have not returned yet, they are equal :)
  return true;
}


void multiple(int number) {

  for(int n = 0; n < number; n++){
    if(digitalRead(mSensor[n]) == HIGH) mRegister[n] = 1;
  }
  // Serial.println(check_inputCondition(number, mRegister));
  if (check_inputCondition(number, mRegister)){
        digitalWrite(OutputSignal, HIGH);
        delay(2000);
        for(int n = 0; n < number; n++){
          mRegister[n] = 0;
        }

  }else{
        digitalWrite(OutputSignal, LOW);
  }
  // Serial.println();
  // delay(500);

  // if (array_cmp(mRegister, check, number, number) == true) {
  //   Serial.println("Start");
  //   digitalWrite(OutputSignal, HIGH);
  //   digitalWrite(CurrentState, HIGH);
  //   delay(10000);
  //   digitalWrite(OutputSignal, LOW);
  //   digitalWrite(CurrentState, LOW);
  //   for (int thisSignal = 0; thisSignal < number; thisSignal++) {
  //     mRegister[thisSignal] = 0;
  //     Serial.println("clear");
  //   }
  // }
}

void loop() {

    for(int n = 0; n < 3; n++){
      Serial.print(digitalRead(mSensor[n]));
    }

    for(int n = 0; n < 3; n++){
      Serial.print(mRegister[n]);
    }

  if (digitalRead(switches[1]) == LOW) {
    Serial.println("This is mode 1");
      modeNumebr = 2;
        // delay(500);
    }
    else   if (digitalRead(switches[0]) == LOW) {
    Serial.println("This is mode 3");
            modeNumebr = 4;

        // delay(500);
    }
    else {
    Serial.println("This is mode 2");
    // delay(500);
            modeNumebr = 3;

    }

    multiple(modeNumebr);
    // Serial.println();


}

boolean check_inputCondition(int number, int *arr){
  for(int n = 0; n < number; n++){
    if(arr[n] != 1){
      return false;
    }
  }
  return true;
}



