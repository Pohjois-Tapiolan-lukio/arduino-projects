/* HUOM! Varmista että seuraavat tiedot ovat oikein: */
#define WIFI_SSID "WIFI SSID"
#define WIFI_PASSWORD "WIFI PASS"
#define BLYNK_AUTH "BLYNK AUTH"

/* Koodi alkaa tästä  */
#define PIN_ECHO 10
#define PIN_TRIGGER 11
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
  Serial.print("Etaisyys: ");
  Serial.println(distance);
  Blynk.virtualWrite(V1, distance);
}

double getDistance() {
  long duration = pollDuration();
  return (duration / 2.0) / 29.1;
}

long pollDuration() {
  digitalWrite(PIN_TRIGGER, LOW);
  delay(5);
  digitalWrite(PIN_TRIGGER, HIGH);
  delay(10);
  digitalWrite(PIN_TRIGGER, LOW);
  long duration = pulseIn(PIN_ECHO, HIGH);  
  return duration;
}
