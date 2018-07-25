// Maaritellaan vakiot
#define JOYSTICK1_X A1

/*
 * setup-rutiinissa maaritellaan sarjaliikenne
 * ja digitaaliset input pinnit
 */
void setup() {
  Serial.begin(9600);
}

/*
 * loop-rutiinissa luetaan analogisen pinnin jannite
 * ja tulostetaan se sarjaliikennevaylaan
 */
void loop() {
  int joystick1X = analogRead(JOYSTICK1_X);

  Serial.println(joystick1X);

  delay(100);
}

// vim: et ts=2 sw=2:
