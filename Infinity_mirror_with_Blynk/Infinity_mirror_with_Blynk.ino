/*************
 Done with Feather M0

 In Blynk app choose 
 Generic Board (Wi-Fi)
 Slider widget V0 virtual port 0
 ZeRGBa widget V1 virtual port 1
 * 
 */
#include <Adafruit_NeoPixel.h>
#include <WiFi101.h>
#include <BlynkSimpleWiFiShield101.h>

#define WIFI_SSID "Your Wifi network Name"
#define WIFI_PASS "Your wifi password"

char auth[] = "Your secret token from Blynk app";


/* Pins */
#define PIN 12
#define NUM_LEDS 27   // default H- Letter 10
#define BRIGHTNESS 255  // Values between 0 ... 255.
#define DEALAY_VAL 10

//int brightness = 255;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  // put your setup code here, to run once:
  WiFi.setPins(8, 7, 4, 2);
  Blynk.begin(auth, WIFI_SSID, WIFI_PASS);
  
  strip.begin();
  delay(500);
  
  for ( uint16_t i =0; i <strip.numPixels(); i++){
    strip.setPixelColor(i, 35,196,142);
    }
    
  strip.setBrightness(125);
  strip.show();

}
BLYNK_WRITE(V0){
  int brightness = param.asInt();
  strip.setBrightness(brightness); 
  strip.show();
  }
BLYNK_WRITE(V1) //Button Widget is writing to pin V1
{
  int red = param[0].asInt(); //values 0 or 1
  int green = param[1].asInt();
  int blue = param[2].asInt();
  //digitalWrite(D5, pinData);
  
  for ( uint16_t i =0; i <strip.numPixels(); i++){
    strip.setPixelColor(i, red,green,blue);
    }
    strip.show();
  
}
void loop() {
  // put your main code here, to run repeatedly:
  /*chase(strip.Color(0,0,255));
  strip.show();
  delay(3000);
  for ( uint16_t i =0; i <strip.numPixels(); i++){
    strip.setPixelColor(i, 255,0,0);
    }
  
  strip.show();
  delay(3000); */
  Blynk.run();

}
static void chase(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels() + 4; i++) {
    strip.setPixelColor(i  , c); // Draw new pixel
    strip.setPixelColor(i - 4, 0); // Erase pixel a few steps back
    strip.show();
    delay(25);
  }
}
