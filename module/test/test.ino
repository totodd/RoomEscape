bool start;
bool yellow = HIGH;
void setup() {
  // put your setup code here, to run once:
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, INPUT_PULLUP);      // Push-Button On Bread Board


  digitalWrite(5, LOW );
  digitalWrite(6, yellow);
  start = HIGH;
  Serial.begin(9600);

}

void loop() {
  bool buttonState = digitalRead(2);  // store current state of pin 12
  if(buttonState == LOW){
    start = LOW;
  }else{
    start = HIGH;
  }
  digitalWrite(5, start);
    digitalWrite(6, yellow);

  
//  Serial.print(buttonState);
  Serial.print("黄   ");
  Serial.print(yellow);
    Serial.print("绿   ");
  Serial.println(start);
  // put your main code here, to run repeatedly:
  

}
