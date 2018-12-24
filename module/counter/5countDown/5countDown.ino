#define outp 3 // LOW to trigger the start of counter
#define wirelessInp 12// wireless triger = HIGH
#define longButton 4
#define buttonInp 5 // = LOW
#define displayDone 6 // =LOW
#define indicater 7
#define timeoutOutp 8
#define rst 9

void setup() {
   Serial.begin(9600);
  pinMode(buttonInp, INPUT_PULLUP);
  pinMode(wirelessInp, INPUT);
  pinMode(outp, OUTPUT);
  pinMode(indicater, OUTPUT);
  pinMode(displayDone, INPUT);
  pinMode(timeoutOutp, OUTPUT);
  pinMode(rst, OUTPUT);
  pinMode(longButton, INPUT_PULLUP);
  digitalWrite(indicater, LOW);
  digitalWrite(rst, LOW);
}

void loop() {
  Serial.println(digitalRead(12));
  bool wireless = digitalRead(wirelessInp);
  // start counting
  if(!digitalRead(buttonInp) || wireless || !digitalRead(longButton)){
    digitalWrite(rst, HIGH);
    digitalWrite(outp, LOW);
    digitalWrite(indicater, HIGH);
  }else{
    digitalWrite(outp, HIGH);
  }

  if (!digitalRead(displayDone)) {
    digitalWrite(indicater, LOW);
    digitalWrite(timeoutOutp, HIGH);

  }else{
    digitalWrite(timeoutOutp, LOW);
  }

}
