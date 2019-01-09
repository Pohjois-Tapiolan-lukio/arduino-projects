#define JOYSTICK1_X A1
#define JOYSTICK1_Y A0
#define JOYSTICK1_THUMB 2

float serialValueX = 0.0;
float serialValueY = 0.0;
int serialButtonValue = 0;


  
void setup() {
  // put your setup code here, to run once:

Serial.begin(115200);

  pinMode(JOYSTICK1_THUMB, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.flush();
  int joystick1X = analogRead(JOYSTICK1_X);
  int joystick1Y = analogRead(JOYSTICK1_Y);
  int joystick1_thumb = digitalRead(JOYSTICK1_THUMB);


  Serial.print(joystick1X);
  Serial.print(",");
  Serial.print(joystick1Y);
  Serial.print(",");
  Serial.print(joystick1_thumb);
  Serial.println();
  
  delay(16);

}
