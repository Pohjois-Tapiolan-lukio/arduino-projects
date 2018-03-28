/* HUOM! Varmista että seuraavat tiedot ovat oikein: */
#define WIFI_SSID "WIFI SSID"
#define WIFI_PASSWORD "WIFI PASS"
#define BLYNK_AUTH "BLYNK AUTH"

/* Koodi alkaa tästä  */
#define PIN_LED A2
#define BLYNK_PRINT Serial

#include <WiFi101.h>
#include <BlynkSimpleWiFiShield101.h>

void setup() {
  Serial.begin(9600);
  
  WiFi.setPins(8, 7, 4, 2); // Adafruit Feather M0 WiFi:ä varten tarvittu "uudelleenjärjestely"
  Blynk.begin(BLYNK_AUTH, WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
  Blynk.run();
}

BLYNK_WRITE(V0) {
  int brightness = param.asInt();
  Serial.println(brightness);
  analogWrite(PIN_LED, brightness);
}

