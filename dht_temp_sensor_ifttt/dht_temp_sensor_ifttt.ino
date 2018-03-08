/* Tässä on erikseen ladattavat kirjastot:
 * - FirebaseArduino: https://github.com/firebase/firebase-arduino
 * - Adafruit BMP085: https://github.com/adafruit/Adafruit-BMP085-Library
 * - DHT: https://github.com/adafruit/DHT-sensor-library
 * - Adafruit Unified Sensor: https://github.com/adafruit/Adafruit_Sensor
 * - ArduinoJSON
 */

/*
  | WeMos | DHT | BMP |
  |-------|-----|-----|
  | GND   | DHT | BMP |
  | 3V3   | 3V3 | 3V3 |
  | D1    | -   | D1  |
  | D2    | -   | D2  |
  | D4    | D4  | -   |
 */

#include <DHT.h>
#include <WiFi101.h>
#include <Adafruit_BMP085.h>

/* Get from: https://ifttt.com/services/maker_webhooks/settings */
#define IFTTT_KEY "IFTTT KEY"
#define IFTTT_EVENT "dht_sensor"

#define WIFI_SSID "WIFIN SSID"
#define WIFI_PASSWORD "WIFIN SALASANA"

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
WiFiClient client;

long aloitusAika = 0;

void setup() {
  Serial.begin(9600);

  WiFi.setPins(8, 7, 4, 2); // Adafruit Feather M0 WiFi:ä varten tarvittu "uudelleenjärjestely"
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  if (!bmp.begin()) {
    Serial.println("BMP Sensoria ei löytynyt!");
    while (1) {}
  }

  dht.begin();

  connect();
  aloitusAika = millis();
}


void loop() {
  float kosteus= dht.readHumidity();
  float lampotila = dht.readTemperature();
  float paine = bmp.readPressure();

  long aika = millis();
  long kulunutAika = (aika - aloitusAika) / 1000;

  if (isnan(kosteus) || isnan(lampotila)) {
    Serial.println();
    Serial.println("Lukeminen DHT anturista epaonnistui!");
  } else {
    sendData(kosteus, lampotila, paine);
    delay(10);
  }

  Serial.println();
  Serial.print("Aika: ");
  Serial.print(kulunutAika);
  Serial.println(" s");
  Serial.print("Kosteus: ");
  Serial.print(kosteus);
  Serial.println(" %");
  Serial.print("Lampotila: ");
  Serial.print(lampotila);
  Serial.println(" °C");
  Serial.print("Paine: ");
  Serial.print(paine);
  Serial.println(" Pa");
  delay(5000);
}

void sendData(float kosteus, float lampotila, float paine) {
  if (client.connect("maker.ifttt.com", 80)) {
    client.print("POST /trigger/");
    client.print(IFTTT_EVENT);
    client.print("/with/key/");
    client.print(IFTTT_KEY);
    client.print("/?value1=");
    client.print(kosteus);
    client.print("&value2=");
    client.print(lampotila);
    client.print("&value3=");
    client.print(paine);
    client.println(" HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println();
    client.println("");
  }
}

void connect() {
  Serial.print("Yhdistetaan WiFiin.."); // Printataan Serial-yhteyden kautta, että olemme yhdistämisvaiheessa
  while (WiFi.status() != WL_CONNECTED) { // Toistetaan seuraavaa niin pitkään kun WiFi ei ole yhdistetty (kun "status" ei ole "connected")
    Serial.print("."); // Printataan piste, jotta tiedämme ettei ohjelma jäätynyt
    delay(1000); // Odotetaan sekunti (1000 millisekuntia), sitten tarkistetaan yhteys uudestan
  }
  Serial.println(""); // Printataan hieman tyhjiä rivejä, jotta ulostulo olisi siistimpi

  Serial.println("Valmista tuli!"); // Nyt on kaikki yhdistelyt valmiita, printataan vielä varmistus Serialiin
}
