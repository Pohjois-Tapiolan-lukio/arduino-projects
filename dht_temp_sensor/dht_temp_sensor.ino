#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SimpleTimer.h>
#include "DHT.h"
#include <Adafruit_BMP085.h>
#include <Wire.h>

#define FIREBASE_HOST "FIREBASE PROJEKTIN OSOITE" //Muodossa "testi123.firebaseio.com"
#define FIREBASE_AUTH "FIRE BASE PROJEKTIN AVAIN"


#define WIFI_SSID "WIFIN SSID"
#define WIFI_PASSWORD "WIFIN SALASANA"

#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP085 bmp;

int n = 0;
long AloitusAika = 0;
long Aika = 0;
long KulunutAika = 0;

void setup() {
  Serial.begin(9600);

  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Yhdistetaan");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Yhdistetty: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(500);

  if (!bmp.begin()) {
    Serial.println("BMP Sensoria ei löytynyt!");
    while (1) {}
  }

  dht.begin();

  long AloitusAika = millis();
}


void loop() {
  float kosteus_Data = dht.readHumidity();
  float C_Data = dht.readTemperature();
  float C2_Data = bmp.readTemperature();
  float Paine_Data = bmp.readPressure();
  float korkeus_Data = bmp.readAltitude();

  long Aika = millis();
  long KulunutAika = ( Aika - AloitusAika) / 1000;

  if (isnan(kosteus_Data) || isnan(C_Data)) {
    Serial.println();
    Serial.println("Lukeminen DHT anturista epaonnistui!");
  } else {
    Firebase.setInt("Mittaukset/" + String(n) + "/Aika", KulunutAika);
    Firebase.setInt("Mittaukset/" + String(n) + "/Kosteus", kosteus_Data);
    Firebase.setInt("Mittaukset/" + String(n) + "/Lampotila", C_Data);
    Firebase.setInt("Mittaukset/" + String(n) + "/LampotilaBMP", C2_Data);
    Firebase.setInt("Mittaukset/" + String(n) + "/Paine", Paine_Data);
    Firebase.setInt("Mittaukset/" + String(n) + "/Korkeus", korkeus_Data);
    delay(10);
    n++;
  }

  if (Firebase.failed()) {
    Serial.print("Firebase lahetys epaonnistui:");
    Serial.println(Firebase.error());
    return;
  }

  Serial.println();
  Serial.print("Aika: ");
  Serial.println(KulunutAika);
  Serial.print("Kosteus: ");
  Serial.print(kosteus_Data);
  Serial.println("%");
  Serial.print("Lampotila: ");
  Serial.print(C_Data);
  Serial.println("*C");
  Serial.print("Lampotila 2: ");
  Serial.print(C2_Data);
  Serial.println("*C");
  Serial.print("Paine: ");
  Serial.print(Paine_Data);
  Serial.println("Pa");
  Serial.print("Korkeus: ");
  Serial.print(korkeus_Data);
  Serial.println("m");
  delay(5000);
}