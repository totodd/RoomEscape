
#define WIRELESS_INP 12
#define MODE_BUTTON_INP 11
#define STOPBUTTON_INP 10
#define RES_INP 4

#define MODE_OUTP 2
#define COUNT_OUTP 3
#define RES_OUTPUT 6

bool fakeMode = false;

void setup(){
  Serial.begin(9600);
  pinMode(WIRELESS_INP, INPUT); // need pull down
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
    if(!digitalRead(MODE_BUTTON_INP)|| digitalRead(WIRELESS_INP)){
    fakeMode = !fakeMode;
    }
  }
  if(!digitalRead(STOPBUTTON_INP)){
    digitalWrite(COUNT_OUTP, LOW);
    delay(5);
    digitalWrite(COUNT_OUTP, HIGH);
  }
}

