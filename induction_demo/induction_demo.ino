#define SPEAKER 5
#define SENSOR A0
#define LED_POS 7
#define LED_NEG 8
#define DEADZONE 2

int value;
int zero;
int absolute;

void setup(){
  Serial.begin(9600);
  zero = analogRead(SENSOR);
}
void loop() {
  value = analogRead(SENSOR)-zero;
  absolute = abs(value);
  
  if(absolute>DEADZONE){
    analogWrite(SPEAKER,map(absolute,DEADZONE,20,1,5));
  }else{
    analogWrite(SPEAKER,LOW);
  }
  
  if(value>DEADZONE){
    digitalWrite(LED_POS,HIGH);
    digitalWrite(LED_NEG,LOW); 
  }else if(value<-DEADZONE){
    digitalWrite(LED_POS,LOW);
    digitalWrite(LED_NEG,HIGH); 
  }else{
    digitalWrite(LED_POS,LOW); 
    digitalWrite(LED_NEG,LOW);
  }
  
  Serial.println(value);
}
