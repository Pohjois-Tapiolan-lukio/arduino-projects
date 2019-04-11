/*
 * Example how Circuit Playground Express (CPX) interacts 
 * with Raspberry pi through USB-Serial. Telegram Bot 
 * laksy_bot.py runs on raspberry
 * https://github.com/Pohjois-Tapiolan-lukio/raspberry_pi-projects/blob/master/projects/laksy_bot.py
 */
#include <Adafruit_NeoPixel.h>
#define PIXEL_NUM 24
int currentPixelCount = 0;
// LedStrip setup
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_NUM, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  strip.begin();
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0){
   int value = Serial.parseInt();
   if (value == 1){
    currentPixelCount += 8;
    if (currentPixelCount > PIXEL_NUM){
      currentPixelCount = PIXEL_NUM;
      }
      theaterChase(strip.Color(0, 127, 0), 20); // Green
      colorWipe(strip.Color(0, 255, 0), 100);//Green
    }
   else if (value < 0){
    currentPixelCount -= 8;
    if (currentPixelCount < 0){
      currentPixelCount = 0;
      }
      theaterChase(strip.Color(127, 0, 0), 20); // red
      turnOffLeds();
      colorWipe(strip.Color(0, 255, 0), 100);
   }
    else if (value == 0){
      currentPixelCount = 0;
      turnOffLeds();
      }
    
   else if (value > 1){
    currentPixelCount = 0;
    theaterChase(strip.Color(0, 0, 127), 20);
    rainbow(40); // You won!
    turnOffLeds();
    
    }
  //colorWipe(strip.Color(0, 255, 0), 100);
  Serial.flush();
  }
  delay(10);
}
void turnOffLeds(){
  for( uint16_t i =0; i < strip.numPixels(); i+=2){
    strip.setPixelColor(i, 0,0,0);
    }
  strip.show();
  }
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<currentPixelCount; i +=2) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i +=2) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i+=2) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 6; q+=2) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+4) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+4) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 6; q+=2) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+4) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+4) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
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
