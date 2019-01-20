/* Notes about connecting the Vernier Protoboard Adapter
 * - GND to Arduino pin GND (ground)
 * - Vres to Arduino pin A4 (resistance reference)
 * - 5V to Arduino pin 5V (power)
 * - SIG1 to Arduino pin A0 (0-5V output used by almost all Vernier sensors)
 */

#define HR_INPUT A0
int threshold = 550;
int signalValue;

int sample; 
int oldSample = 0;

//If FastLed is used
//#include <FastLED.h>
//#define NUM_LEDS 150

//If Adafruit_NeoPixel is used..
#include <Adafruit_NeoPixel.h>
#define PIN 12
#define NUM_LEDS 150   
#define BRIGHTNESS 255  // Values between 0 ... 255.
#define DEALAY_VAL 10

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  strip.begin();

  for( uint16_t i =0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, 0,0,255);
    }

  strip.setBrightness(70);
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  measureBeat();
  if (sample and (oldSample != sample) ){
    heartBeat();
    }
    oldSample = sample;
    
   for (volatile int i=0; i<strip.numPixels(); i++){
      strip.setPixelColor(i, strip.Color(20, 0, 0)); 
    }
   strip.show(); 
}

 void measureBeat(){
  signalValue = analogRead(HR_INPUT);
  Serial.println(signalValue);
  if (signalValue > threshold){
    sample = 1;
    }
  else{ 
    sample = 0;
  }
}
  

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
 
void heartBeat (){
  for (volatile int i=0; i<strip.numPixels(); i++){
      strip.setPixelColor(i, strip.Color(255, 0, 0)); 
    }
    strip.show();
   delay(10);
}
  
