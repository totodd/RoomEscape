int analogpin=0;              //定义analogpin为模拟口0    
int ExternalSignal = 13;           
int val,val5;                     //定义变量val,val5
int val2=0;                     //定义变量val2
int val3=0;                     //定义变量val3
int val4=0;                     //定义变量val4
int duration = 0;
int startTime;
bool startingTimeSet = false;
void setup()
{
    Serial.begin(9600);     //设置波特率位9600
    pinMode(ExternalSignal,OUTPUT);
}
void loop()
{
    int val,val5;
    float val1;
    val=analogRead(analogpin);               //读出模拟口的值并赋给变量val
    val1=val/3.9;
    val5=(int)val1;
    val3=val5/100;
    val2=(val5%100)/10;
    val4=val5%10;
    
    if (val2 >=5 && !startingTimeSet){
      startTime = millis();    
      startingTimeSet = true;
      Serial.println("starting time is reset");  
    }
    if (val2 >=5){
      duration = millis()-startTime;
      Serial.println("counting time");
      Serial.println(duration);
    }else{
      duration = 0;
      Serial.println("input is less than 5, make it larger");
      startingTimeSet = false;
      digitalWrite(ExternalSignal,LOW);
    }
    if (duration>= 5000){
      digitalWrite(ExternalSignal,HIGH);
    }

}
