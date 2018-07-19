// Maaritellaan vakiot
#define JOYSTICK1_X A1
#define JOYSTICK1_Y A0
#define THUMB1 2
#define JOYSTICK2_X A3
#define JOYSTICK2_Y A2
#define THUMB2 3
#define BUTTON1 4
#define BUTTON2 5
#define BUTTON3 6

void setup() {
  Serial.begin(9600);
  pinMode(THUMB1, INPUT);
  pinMode(THUMB2, INPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
}

void loop() {
  int joystick1X = analogRead(JOYSTICK1_X),
      joystick1Y = analogRead(JOYSTICK1_Y),
      thumb1 = digitalRead(THUMB1);

  int joystick2X = analogRead(JOYSTICK2_X),
      joystick2Y = analogRead(JOYSTICK2_Y),
      thumb2 = digitalRead(THUMB2);

  int but1 = digitalRead(BUTTON1),
      but2 = digitalRead(BUTTON2),
      but3 = digitalRead(BUTTON3);

  Serial.println(
    String(joystick1X)
    +","
    +String(joystick1Y)
    +","
    +String(thumb1)
    +","
    +String(joystick2X)
    +","
    +String(joystick2Y)
    +","
    +String(thumb2)
    +","
    +String(but1)
    +","
    +String(but2)
    +","
    +String(but3)
    );

  delay(100);
}

// vim: et ts=2 sw=2:
