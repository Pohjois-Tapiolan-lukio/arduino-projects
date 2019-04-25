/* Circuit Playground Express CPX esimerkki
Mitataan etäisyys ulkoisella ultraäänianturilla

Kytkennät Ultraäänianturi --> CPX
GND --> GND
VCC --> VOUT
trig --> D9 eli A1
echo --> D6 eli A2
*/

#include <Adafruit_NeoPixel.h>
#define LED_PIN 8 //10 kpl integroituja ledejä

#define PIN_KAIKU 6 //A1 in Circuit Playground Express
#define PIN_LIIPAISU 9 //A2 

Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_KAIKU, INPUT);
  pinMode(PIN_LIIPAISU, OUTPUT);

  Serial.begin(9600);

  strip.begin();
  strip.setBrightness(5);
  
  for (int i = 0; i < 10; i++) {
    strip.setPixelColor(i, 0,   0,   255);
    }
    strip.show();

    delay(2000);


}

void loop() {
  // put your main code here, to run repeatedly:

  double etaisyys = laskeEtaisyys();

  Serial.print("Etaisyys: ");
  Serial.println(etaisyys);

  paivitaLedit(etaisyys);

  delay(500);

}


double laskeEtaisyys() {
  long kesto = mittaaKesto();
  return (kesto /2.0) / 29.1;
  }

long mittaaKesto() {
  digitalWrite(PIN_LIIPAISU, LOW);
  delay(5);
  digitalWrite(PIN_LIIPAISU, HIGH);
  delay(10);
  digitalWrite(PIN_LIIPAISU, LOW);
  long kesto = pulseIn(PIN_KAIKU, HIGH);
  return kesto;
  }

// Käytetty mittausalue 5 cm ... 30 cm
// Ledit 10 kpl indeksoit 0, 1, ...9 

void paivitaLedit (float etaisyys) {
  sammutaLedit();
  int ledLKM;
  if (etaisyys >= 5 && etaisyys <= 30) {
  ledLKM = map((int)etaisyys, 5, 30, 0, 10);
  }
  else if (etaisyys > 30) {
    ledLKM = 10;
    }
  else if (etaisyys < 5) {
    ledLKM = 0;
    }  
  
  for (int i = 0; i < ledLKM; i++) {
    strip.setPixelColor(10-i, 255,   0,   0);
    }
    strip.show();
  }
void sammutaLedit() {
  for (int i = 0; i < 10; i++) {
    strip.setPixelColor(i, 0,   0,   0);
    }
    strip.show();
}
