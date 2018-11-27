
#define WIRELESS_INP
#define MODE_BUTTON_INP
#define STOPBUTTON_INP
#deifne RES_INP

#define MODE_OUTP
#define COUNT_OUTP
#define RES_OUTPUT

bool fakeMode = false;

void setup(){
  Serial.begin(9600);
  pinMode(WIRELESS_INP, INPUT_PULLUP); // high
  pinMode(MODE_BUTTON_INP, INPUT_PULLUP);
  pinMode(STOPBUTTON_INP, INPUT_PULLUP);
  pinMode(RES_INP, INPUT_PULLUP);

  pinMode(MODE_OUTP, OUTPUT);
  pinMode(COUNT_OUTP, OUTPUT);
  pinMode(RES_OUTPUT, OUTPUT); // high if pass

}

void loop(){
  digitalWrite(MODE_OUTP, fakeMode);
  digitalWrite(RES_OUTPUT, !digitalRead(RES_INP));
  if(!digitalRead(MODE_BUTTON_INP)|| digitalRead(WIRELESS_INP)){
    delay(100);
    fakeMode = !fakeMode;
  }
  if(!digitalRead(STOPBUTTON_INP)){
    digitalWrite(COUNT_OUTP, LOW);
    delay(5);
    digitalWrite(COUNT_OUTP, HIGH);
  }
}
