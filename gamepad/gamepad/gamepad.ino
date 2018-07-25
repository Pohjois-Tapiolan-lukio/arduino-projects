// Maaritellaan vakiot
#define JOYSTICK1_X A1
#define JOYSTICK1_Y A0
#define JOYSTICK1_THUMB 2
#define JOYSTICK2_X A3
#define JOYSTICK2_Y A2
#define JOYSTICK2_THUMB 3
#define BUTTON1 4
#define BUTTON2 5
#define BUTTON3 6

/*
 * setup-rutiinissa maaritellaan sarjaliikenne
 * ja digitaaliset input pinnit
 */
void setup() {
  Serial.begin(9600);

  pinMode(JOYSTICK1_THUMB, INPUT);
  pinMode(JOYSTICK2_THUMB, INPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
}

/*
 * loop-rutiinissa luetaan analogisten ja digitaalisten
 * pinnien jannite ja tulostetaan ne sarjaliikennevaylaan
 */
void loop() {
  int joystick1X = analogRead(JOYSTICK1_X),
      joystick1Y = analogRead(JOYSTICK1_Y),
      joystick1_thumb = digitalRead(JOYSTICK1_THUMB);

  int joystick2X = analogRead(JOYSTICK2_X),
      joystick2Y = analogRead(JOYSTICK2_Y),
      joystick2_thumb = digitalRead(JOYSTICK2_THUMB);

  int but1 = digitalRead(BUTTON1),
      but2 = digitalRead(BUTTON2),
      but3 = digitalRead(BUTTON3);

  Serial.println(
    String(joystick1X) +","
    +String(joystick1Y) +","
    +String(joystick1_thumb) +","
    +String(joystick2X) +","
    +String(joystick2Y) +","
    +String(joystick2_thumb) +","
    +String(but1) +","
    +String(but2) +","
    +String(but3));

  delay(100);
}

// vim: et ts=2 sw=2:
