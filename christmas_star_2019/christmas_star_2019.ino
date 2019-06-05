#include <Adafruit_NeoPixel.h>

/* WiFi Info */
#define WIFI_SSID "Wifi-network"
#define WIFI_PASS "wifi-passwd"

/* Shiftr Info */
#define SHIFTR_NAME "LedStrip"
#define SHIFTR_USER "shiftr-key"
#define SHIFTR_PASS "shiftr-secret"

/* Pins */
#define PIN D4
#define NUM_LEDS 23
#define BRIGHTNESS 255  // Values between 0 ... 255.
#define DEALAY_VAL 10

//#define LED_PIN D8

#include <ESP8266WiFi.h>
//#include <WiFi101.h> // Sisällytetään kirjasto WiFi-yhteyksiä varten
#include <MQTTClient.h> // Sisällytetään kirjasto MQTT-protokollaa varten

WiFiClient net; // WiFi-yhteyksiä ylläpitävä olio
MQTTClient client; // MQTT-protokollan yhteyksiä ylläpitävä olio

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  //pinMode(LED_PIN,OUTPUT);
  
  strip.begin();
  Serial.begin(9600); 

  //WiFi.setPins(8, 7, 4, 2); // Adafruit Feather M0 WiFi:ä varten tarvittu "uudelleenjärjestely"
  WiFi.begin(WIFI_SSID, WIFI_PASS); 

  client.begin("broker.shiftr.io", net); // Valmistellaan shiftr.io yhteys
  client.onMessage(messageReceived); // Kerrotaan MQTT:lle, että haluamme prosessoida meille lähetetyt viestit funktiossa messageReceived()

  connect(); // Yhdistetään WiFiin ja shiftr.io:hon. Kts. ohjelman viimeinen funktio lisätietoja varten
  client.subscribe("/messages"); // Aloitetaan kuuntelemaan /leda, täältä saamme viestejä joissa on sensorien arvoja 
  colorWipe(strip.Color(255, 0, 0), 50); // Red
}

void messageReceived(String &topic, String &payload) {
  String message = payload; // 
  if (message == "red glitter"){
    theaterChase(strip.Color(127, 0, 0), 100); // Red
    colorWipe(strip.Color(127, 0, 0), 50);
    }
   else if (message == "blue glitter"){
    theaterChase(strip.Color(0, 0, 127), 100); // Blue
    colorWipe(strip.Color(0, 0, 127), 50);
    }
  else if (message == "green glitter"){
    theaterChase(strip.Color(0, 127, 0), 100); // Green
    colorWipe(strip.Color(0, 127, 0), 50);
    }
    
  else if (message == "green"){
    colorWipe(strip.Color(0, 255, 0), 50); // Green
    /*for (int i=0; i <= NUM_LEDS; i++){
      strip.setPixelColor(i, strip.Color(0,255,0));
      }
    strip.show();
    */
    }
  else if (message == "blue"){
    colorWipe(strip.Color(0, 0, 255), 50); // Blue
    }
  else if (message == "nolight"){
    for (int i=0; i <= NUM_LEDS; i++){
      strip.setPixelColor(i, strip.Color(0,0,0));
      }
    strip.show();
    }
   else if (message == "red"){
    colorWipe(strip.Color(255, 0, 0), 50); // Red
    
    
    }
  else if (message == "sparkle"){
    theaterChase(strip.Color(127, 127, 127), 100); // White
    colorWipe(strip.Color(127, 127, 127), 50);
    /*for(int i=0; i < 4; i++){
      chase(strip.Color(255, 0, 0)); // Red
      chase(strip.Color(0, 255, 0)); // Green
      chase(strip.Color(0, 0, 255)); // Blue
     }*/
    }
  else if (message == "rainbow"){
    rainbow(20);
    rainbowCycle(20);
    //theaterChaseRainbow(50);
    }
}

void loop() {
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
  while (!client.connect(SHIFTR_NAME, SHIFTR_USER, SHIFTR_PASS)) { // Yritetään yhdistää shiftr.io:hon
    Serial.print(".");
    delay(1000);
  }
  Serial.println(""); // Printataan taas hieman tyhjiä rivejä siisteyden vuoksi

  Serial.println("Valmista tuli!"); // Nyt on kaikki yhdistelyt valmiita, printataan vielä varmistus Serialiin
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

static void chase(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels() + 4; i++) {
    strip.setPixelColor(i  , c); // Draw new pixel
    strip.setPixelColor(i - 4, 0); // Erase pixel a few steps back
    strip.show();
    delay(25);
  }}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

  
