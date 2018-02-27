/* Tämä ohjelma lähettää Shiftr.io palveluun analogisesta pinistä luettuja arvoja. 
 * Kirjoitettu Vernierin BTA mielessä, mutta toimii minkä tahansa analogisen sisääntulon kanssa. */

/* Huomioita Vernier Protoboard Adapterin pineistä
 * - GND - Arduino pin GND (maa)
 * - Vres - Arduino pin A4 (resistanssin vertailuarvo)
 * - 5V - Arduino pin 5V (virta)
 * - SIG1 - Arduino pin A0 (0-5V ulostulo jota Vernierin sensorit yleensä käyttävät) */

/* Pins */
#define PIN_SENSOR A0

#include <WiFi101.h> // Sisällytetään kirjasto WiFi-yhteyksiä varten
#include <MQTTClient.h> // Sisällytetään kirjasto MQTT-protokollaa varten

char ssid[] = "<WiFi SSID>"; // WiFi-verkon nimi
char pass[] = "<WiFi Password>"; // WiFi-verkon salasana
WiFiClient net; // WiFi-yhteyksiä ylläpitävä olio
MQTTClient client; // MQTT-protokollan yhteyksiä ylläpitävä olio

void setup() { // Ohjelman alussa pyöritettävä funktio, pyörii aluksi kerran, ja sitten siirtyy toistamaan loop():ia
  Serial.begin(9600); // Avataan Serial-yhteys, jotta voimme printata hyödyllistä informaatiota kun ohjelma etenee
  
  WiFi.setPins(8, 7, 4, 2); // Adafruit Feather M0 WiFi:ä varten tarvittu "uudelleenjärjestely"
  WiFi.begin(ssid, pass); // Aloitetaan yhdistämään WiFiin

  client.begin("broker.shiftr.io", net); // Valmistellaan shiftr.io yhteys

  connect(); // Yhdistetään WiFiin ja shiftr.io:hon. Kts. ohjelman viimeinen funktio lisätietoja varten
}

void loop() { // setup():n jälkeen toistuvasti pyörivä funktio
  client.loop(); // Päivitetään MQTT-yhteydet

  if (!client.connected()) { // Jos yhteys on katkennut... ("!" tarkoittaa "ei" eli tämän rivin voi lukea "jos ei ole niin, että client on connected")
    connect(); // Yhdistetään uudestaan!
  }
  
  int sensor_value = analogRead(PIN_SENSOR); // Luetaan sensorin arvo
  client.publish("/sensori", (String) sensor_value); // Lähetetään sensorin arvo (tekstinä, eli muutetaan String:ksi) shiftr.io:n /sensori:in
  delay(500); // Odotetaan puoli sekuntia (500 millisekuntia) ettei täytetä shiftr.io:n palvelimia turhaan
}

void connect() {
  Serial.print("Yhdistetaan WiFiin.."); // Printataan Serial-yhteyden kautta, että olemme yhdistämisvaiheessa
  while (WiFi.status() != WL_CONNECTED) { // Toistetaan seuraavaa niin pitkään kun WiFi ei ole yhdistetty (kun "status" ei ole "connected")
    Serial.print("."); // Printataan piste, jotta tiedämme ettei ohjelma jäätynyt
    delay(1000); // Odotetaan sekunti (1000 millisekuntia), sitten tarkistetaan yhteys uudestan
  }
  Serial.println(""); // Printataan hieman tyhjiä rivejä, jotta ulostulo olisi siistimpi

  Serial.print("Yhdistetaan shiftr.io:n..."); // Sama kuin aiemmin WiFi:n kanssa, mutta nyt shiftr.io:n kanssa
  while (!client.connect("arduino", "<SHIFTR_IO_USERNAME>", "<SHIFTR_IO_PASSWORD>")) { // Yritetään yhdistää shiftr.io:hon
    Serial.print(".");
    delay(1000);
  }
  Serial.println(""); // Printataan taas hieman tyhjiä rivejä siisteyden vuoksi

  Serial.println("Valmista tuli!"); // Nyt on kaikki yhdistelyt valmiita, printataan vielä varmistus Serialiin
}

