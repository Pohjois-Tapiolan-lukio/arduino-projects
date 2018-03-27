/* HUOM! Varmista että seuraavat tiedot ovat oikein: */
#define WIFI_SSID "WIFIN SSID"
#define WIFI_PASSWORD "WIFIN SALASANA"
#define BLYNK_AUTH "BLYNK AUTH KEY"

/* Koodi alkaa tästä  */
#define PIN_ECHO 3
#define PIN_TRIGGER 4
#define BLYNK_PRINT Serial

#include <WiFi101.h>
#include <BlynkSimpleWiFiShield101.h>
#include "math.h"

void setup() {
  Serial.begin(9600);
  
  WiFi.setPins(8, 7, 4, 2); // Adafruit Feather M0 WiFi:ä varten tarvittu "uudelleenjärjestely"
  Blynk.begin(BLYNK_AUTH, WIFI_SSID, WIFI_PASSWORD);

  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
}

void loop() {
  Blynk.run();
}

BLYNK_READ(V1) {
  double distance = getDistance();
  Blynk.virtualWrite(V1, distance);
}

double getDistance() {
  long duration = pollDuration(0);
  return (duration / 2.0) / 29.1;
}

long pollDuration(int tryCount) {
  digitalWrite(PIN_TRIGGER, LOW);
  delay(5);
  digitalWrite(PIN_TRIGGER, HIGH);
  delay(10);
  digitalWrite(PIN_TRIGGER, LOW);
  long duration = pulseIn(PIN_ECHO, HIGH);  
  return duration;
}
