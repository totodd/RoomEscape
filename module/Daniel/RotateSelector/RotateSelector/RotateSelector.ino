int a[3] = {2,3,4};
int output[3] = {0,0,0};

void setup(){
  Serial.begin(9600);
  pinMode(a[1],INPUT);
  pinMode(a[2],INPUT);
  pinMode(a[3],INPUT);
  }

void loop(){
  for (int i = 0;i<3;i++){
    output[i] = digitalRead(a[i]);
    Serial.print(output[i]);
  }
  Serial.println();
  delay(100);
}
