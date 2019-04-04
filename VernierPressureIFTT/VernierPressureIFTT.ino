/* Notes about connecting the Vernier Protoboard Adapter
 *  For Feather M0
   - GND to Arduino pin GND (ground)
   - 5V to Feather M0 pin 3.3V (power)
   - SIG1 to Arduino pin A0 (0-5V output used by almost all Vernier sensors)
*/

/* Notes about the Vernier Barometer GAs Pressure Sensor GPS-BTA:
  https://www.vernier.com/files/manuals/gps-bta/gps-bta.pdf
  Barometer range 0 kPa ... 210 kP corresponds values 0 ... 255
  or
  Vernier Factory calibration:
  https://www.vernier.com/til/1406
  Factory stored calibrations are:
  kPa: slope = 51.71, intercept = -25.86
  atm: slope = 0.5103, intercept = -0.25
  mm Hg: slope = 387.8, intercept = -190.4
  y= kx + B


  IF you use BAR-BTA:
  https://www.vernier.com/files/manuals/bar-bta.pdf
  Applied model: y = kx + B
  slope 6.825 kPa/V
  intercept 76.29375 kPa
*/

/*PINS*/
//#define PIN_SERVO 8
#define PIN_SENSOR A0

//#include "Servo.h"

/* Pressure Scale: for Servo analogical display */
//#define MIN_VAL 85
//#define MAX_VAL 185

#include <WiFi101.h>

/* Get from: https://ifttt.com/services/maker_webhooks/settings */
#define IFTTT_KEY "IFTTT KEY"
#define IFTTT_EVENT "dht_to_sheet"

#define WIFI_SSID "OnePlus 6T"
#define WIFI_PASSWORD "ipad14567"

#include <WiFi101.h>

/* Get from: https://ifttt.com/services/maker_webhooks/settings */
#define IFTTT_KEY "INsert Your Key"
#define IFTTT_EVENT "Your IFTTT event_name"

#define WIFI_SSID "Your Wifi-Name"
#define WIFI_PASSWORD "your password"



int value;
float voltage;
float pressure;
//float previousPressure;


WiFiClient client;


void setup() {
  // put your setup code here, to run once:
 
  Serial.begin(115200);
  

  WiFi.setPins(8, 7, 4, 2); // Adafruit Feather M0 WiFi:ä varten tarvittu "uudelleenjärjestely"
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  connect();
}

void loop() {
  // put your main code here, to run repeatedly:
  measurePressure();

  Serial.println(value);
  Serial.print("Pressure: ");
  Serial.println(pressure);

  //updatePreviousPressure();
  sendData();
  delay(3000);
 

}
void measurePressure() {
  value = analogRead(PIN_SENSOR);
  voltage = 5.0/3.3*value / 1024.0;
  pressure = ((voltage * 5.0 ) * 6.825 + 76.29375);
}

/*
void updatePreviousPressure() {
  previousPressure = pressure;
}
*/
void sendData() {
  if (client.connect("maker.ifttt.com", 80)) {
    client.print("POST /trigger/");
    client.print(IFTTT_EVENT);
    client.print("/with/key/");
    client.print(IFTTT_KEY);
    client.print("/?value1=");
    Serial.println(pressure);
    client.print(pressure);
    client.print("&value2=");
    client.print(10);
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
