/*
   This is the main logic used for magnetic detection
   The number of how many sensors can be defined easily
   Similarly, metal detection can also be used using this.
*/
int number = 4;
int mSensor[4] = {3, 4, 5, 6};

int OutputSignal = 2;  //External Control Pin (current using LED)
int Reset = 11;   //External Reset (highest level)
int CurrentState = 12;    //To indicate current state of this model, High: Working; LOW: Idel
int switches[2] = {9, 10};

void setup() {
  Serial.begin(9600);
  for (int thisSignal = 0; thisSignal < number; thisSignal++) {
    pinMode(mSensor[thisSignal], INPUT);
  }
  for (int thisswitch = 0; thisswitch < 2; thisswitch++) {
    pinMode(switches[thisswitch], INPUT);
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
  int check[number];
  int mRegister[number];
  for (int thisSignal = 0; thisSignal < number; thisSignal++) {
    check[thisSignal] = 1;
    if (digitalRead(mSensor[thisSignal]) == HIGH) {
      mRegister[thisSignal] = 1;
      delay(100);
    }
  }

  if (array_cmp(mRegister, check, number, number) == true) {
    Serial.println("Start");
    digitalWrite(OutputSignal, HIGH);
    digitalWrite(CurrentState, HIGH);
    delay(10000);
    digitalWrite(OutputSignal, LOW);
    digitalWrite(CurrentState, LOW);
    for (int thisSignal = 0; thisSignal < number; thisSignal++) {
      mRegister[thisSignal] = 0;
      Serial.print(mRegister[thisSignal]);
    }
    Serial.println();

  }
}

void loop() {
  if (digitalRead(switches[0]) == HIGH && digitalRead(switches[1]) == LOW) {
    Serial.println("This is mode 1");
      multiple(2);
        delay(500);
    }
    else   if (digitalRead(switches[0]) == LOW && digitalRead(switches[1]) == LOW) {
    Serial.println("This is mode 2");
      multiple(3);
        delay(500);
    }
    else {
    Serial.println("This is mode 3");
    delay(500);
      multiple(4);
    }


}





