/* Tämä ohjelma hakee arvoja shiftr.io:sta, ja siirtää servoa siitä riippuen. */

/* Pins */
#define PIN_SERVO 10

#include <Servo.h> // Sisällytetään kirjasto Servoa varten
#include <WiFi101.h> // Sisällytetään kirjasto WiFi-yhteyksiä varten
#include <MQTTClient.h> // Sisällytetään kirjasto MQTT-protokollaa varten

char ssid[] = "<WiFi SSID>"; // WiFi-verkon nimi
char pass[] = "<WiFi Password>"; // WiFi-verkon salasana
WiFiClient net; // WiFi-yhteyksiä ylläpitävä olio
MQTTClient client; // MQTT-protokollan yhteyksiä ylläpitävä olio

Servo servo; // Servoa ohjaava olio

void setup() { // Ohjelman alussa pyöritettävä funktio, pyörii aluksi kerran, ja sitten siirtyy toistamaan loop():ia
  Serial.begin(9600); // Avataan Serial-yhteys, jotta voimme printata hyödyllistä informaatiota kun ohjelma etenee

  servo.attach(PIN_SERVO); // Kiinnitetään Servo-ohjain Servon piniin

  WiFi.setPins(8, 7, 4, 2); // Adafruit Feather M0 WiFi:ä varten tarvittu "uudelleenjärjestely"
  WiFi.begin(ssid, pass); // Aloitetaan yhdistämään WiFiin

  client.begin("broker.shiftr.io", net); // Valmistellaan shiftr.io yhteys
  client.onMessage(messageReceived); // Kerrotaan MQTT:lle, että haluamme prosessoida meille lähetetyt viestit funktiossa messageReceived()

  connect(); // Yhdistetään WiFiin ja shiftr.io:hon. Kts. ohjelman viimeinen funktio lisätietoja varten
  client.subscribe("/sensori"); // Aloitetaan kuuntelemaan /sensori:a, täältä saamme viestejä joissa on sensorien arvoja
}

void messageReceived(String &topic, String &payload) {
  int sensori_arvo = payload.toInt(); // Luetaan payloadista sensorin arvo, ja muutetaan se kokonaisluvuksi (int)
  int kulma = sensori_arvo * 180 / 1024; // Muunnetaan sensorin arvo kulmaksi (väliltä 0..1024 välille 0..180)
  servo.write(kulma); // Ohjataan servo laskettuun kulmaan
}

void loop() { // setup():n jälkeen toistuvasti pyörivä funktio
  client.loop(); // Päivitetään MQTT-yhteydet

  if (!client.connected()) { // Jos yhteys on katkennut... ("!" tarkoittaa "ei" eli tämän rivin voi lukea "jos ei ole niin, että client on connected")
    connect(); // Yhdistetään uudestaan!
  }
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
