/* This example shows how to use the FFT library with a Circuit Playground 
 * Express. Requires installing the "Adafruit Zero FFT library" and 
 * Adafruit_NeoPixel for external ledstrip.
 */

#include <Adafruit_CircuitPlayground.h>
#include "Adafruit_ZeroFFT.h"
#include <Adafruit_NeoPixel.h>


//this must be a power of 2
#define DATA_SIZE 256

#define NUM_PIXELS 10

//the sample rate
#define FS 22000

//the lowest frequency that will register on the meter
#define FREQ_MIN 600

//the highest frequency that will register on the meter
#define FREQ_MAX 3000

#define MIN_INDEX FFT_INDEX(FREQ_MIN, FS, DATA_SIZE)
#define MAX_INDEX FFT_INDEX(FREQ_MAX, FS, DATA_SIZE)

#define SCALE_FACTOR 32

// Set the threshold level for sound intensity
#define THRESHOLD 10

// LedStrip setup
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(23, PIN, NEO_GRB + NEO_KHZ800);


int16_t inputData[DATA_SIZE];


// the setup routine runs once when you press reset:
void setup() {
  CircuitPlayground.begin();
  Serial.begin(115200);
  strip.begin();
  strip.show();
}


void loop() {
  int i;
  int maxVal = 0;
  int maxIndex = 0;
  
  CircuitPlayground.mic.capture(inputData, DATA_SIZE);
  
  // Center data on average amplitude
  int32_t avg = 0;
  for(i=0; i<DATA_SIZE; i++) avg += inputData[i];
  avg /= DATA_SIZE;
  // Scale for FFT
  for(i=0; i<DATA_SIZE; i++)
    inputData[i] = (inputData[i] - avg) * SCALE_FACTOR;
  
  //run the FFT. Output stored in inputData
  ZeroFFT(inputData, DATA_SIZE);
  
  //find the maxValue and the MaxIndex
  for (int k= 0; k < DATA_SIZE / 2; k++){
    if (inputData[k] >= maxVal){
      maxVal = inputData[k];
      maxIndex = k;
      }
    }
  for (int i=0; i<= DATA_SIZE/2; i++){
    //Serial.print(FFT_BIN(i,FS, DATA_SIZE));
    //Serial.print(" ");
    
    Serial.print(i);
    Serial.print(": ");
    Serial.print(inputData[i]);
    Serial.print(" ");
    }
    Serial.println("");
    Serial.print("MaxVal: ");
    Serial.print(maxVal);

    Serial.print(" MaxIndex: ");
    Serial.print(maxIndex);
    Serial.println("");

    if (maxVal >= THRESHOLD){

      if ( maxIndex <= 9 ){
        colorWipe(strip.Color(0, 0, 0), 100);
      }

      else if ( maxIndex >= 15 ){
        colorWipe(strip.Color(255, 0, 0), 100);
      }
    } 
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i +=2) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
