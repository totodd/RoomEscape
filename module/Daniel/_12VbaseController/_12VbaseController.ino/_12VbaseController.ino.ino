
const int ExternalSignal = 2;     // the number of the external signal
const int RelayPin =  10;      // the number of the relay pin
const int Interrupt = 3;

// variables will change:
int ExternalState = 0;         // variable for reading the external status

void setup() {
  // initialize the realy pin as an output:
  pinMode(RelayPin, OUTPUT);
  // initialize the external signal pin as an input:
  pinMode(ExternalSignal, INPUT);
  pinMode(Interrupt,OUTPUT);
}

void loop() {
  // read the state of the external value
  ExternalState = digitalRead(ExternalSignal);

  // check if the external signal. If it is, the externalStatef is HIGH:
  if (ExternalState == HIGH) {
    digitalWrite(RelayPin, HIGH);
    delay(10000);
    digitalWrite(RelayPin, LOW);
  } 
  else if (digitalRead(Interrupt) == HIGH){
    digitalWrite(RelayPin, HIGH);
    delay(10000);
    digitalWrite(RelayPin, LOW);
  }
  else {
    digitalWrite(RelayPin, LOW);
  }

  //Used for interrupt.

}
