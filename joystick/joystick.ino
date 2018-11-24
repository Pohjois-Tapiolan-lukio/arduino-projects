// Luodaan tarvittavat muuttujat
short x; // Joystickin x- ja y-koordinaatit
short y;
bool c; // Kertoo, onko joystickin nappi painettu

void setup() {
  pinMode(2, INPUT); // Asetetaan pinni 2 lukutilaan
  Serial.begin(9600); // Käynnistetään sarjaliikennne tietokoneen kanssa
}

void loop() {
  // Luetaan joystickin arvot ja sijoitetaan ne muuttujiin
  c = digitalRead(2);
  x = analogRead(A0);
  y = analogRead(A1);

  // Lähetetään luetut arvot tietokoneelle sarjaliikenteellä
  Serial.println(String(c) + ", X=" + String(x) + ", Y=" + String(y));
  delay(100); // Odotetaan 100ms
}
