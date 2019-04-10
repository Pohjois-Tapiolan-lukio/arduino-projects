/*
 * Example of Serial Communication between Circuit Playground Express and Raspberry
 * Pi which send heartrate BPM value to telegram. Python script for raspberry:
 * https://github.com/Pohjois-Tapiolan-lukio/raspberry_pi-projects/blob/master/projects/cpx_serial_telegram.py
 * 
 */
#include <Adafruit_Circuit_Playground.h>
//#include <Adafruit_NeoPixel.h>




int rate[10];
unsigned long sampleCounter = 0;          // used to determine pulse timing
unsigned long lastBeatTime = 0;           // used to find IBB = intervalBetweenBeats
int maxValue =512;                      // used to find peak in pulse wave, default value 
int minValue = 512;                     // used to find trough in pulse wave, default value
int threshold = 525;                    // used to find instant moment of heart beat
int intervalBetweenBeats = 600;
int amplitude = 100;                   // used to hold amplitude of pulse waveform, default value
int BPM = 0;
boolean firstBeat = true;        // used to reset rate array so we startup with reasonable BPM
boolean secondBeat = false; // used to seed rate array so we startup with reasonable BPM

boolean pulse = false;


void setup() {
  // put your setup code here, to run once:
  
    Serial.begin(115200);
    Serial.println("CPX");

    CircuitPlayground.begin();
    CircuitPlayground.setBrightness(255);
    CircuitPlayground.setPixelColor(0, 0, 255,0);
    CircuitPlayground.setPixelColor(1, 0, 255,0);
    

}


void loop() {

  uint16_t reading = CircuitPlayground.lightSensor();

  int N = millis() - lastBeatTime;

  if (reading < threshold && N > (intervalBetweenBeats/5)*3){ // avoid noise by waiting
    if (reading < minValue){
      minValue = reading;
      }
    }

   if (reading > threshold && reading > maxValue){
    maxValue = reading;
    }
  
  if (N > 250){
    if (reading > threshold && pulse == false && (N > (intervalBetweenBeats/5)*3) ){ // 3/5N > IBB to avoid noice in signal
      pulse = true;
      intervalBetweenBeats = millis() - lastBeatTime;
      lastBeatTime = millis();
      //colorWipe(20);

      if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
          secondBeat = false;                  
          for(int i=0; i<=9; i++){             // reset the running total to get a realisitic BPM at startup
            rate[i] = intervalBetweenBeats;                      
          }  
      }
      if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
          firstBeat = false;                   // clear firstBeat flag
          secondBeat = true;                   // set the second beat flag
      }

      int runningTotal = 0;
      for(int i=0; i<=8; i++){                // shift data in the rate array
        rate[i] = rate[i+1];                  // and drop the oldest IBB value 
        runningTotal += rate[i];              // add up the 9 oldest IBB values
      }

      rate[9] = intervalBetweenBeats;         // add the latest IBB to the rate array
      runningTotal += rate[9];                // add the latest IBB to runningTotal
      runningTotal /= 10;                     // average the last 10 IBB values 
      BPM = 60000/runningTotal; 
    }
 }
 if (reading < threshold && pulse == true){   // when the values are going down, the beat is over
    pulse = false;                         // reset the Pulse flag so we can do it again
    amplitude = maxValue - minValue;                           // get amplitude of the pulse wave
    threshold = amplitude/2 + minValue;                    // set thresh at 50% of the amplitude
    maxValue = threshold;                            // reset these for next time
    minValue = threshold;

    for(uint16_t i=5; i<=7; i +=1) {
    //CircuitPlayground.setPixelColor(i, 0, 0,0);
    }
  }

  if (N > 2500){                           // if 2.5 seconds go by without a beat
    threshold = 512;                          // set thresh default
    maxValue = 512;                               // set maxValue default
    minValue = 512;                               // set minValue default
    lastBeatTime = millis();          // bring the lastBeatTime up to date        
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
    BPM = 0;  
  }
  Serial.println(BPM);
  delay(10);
}

void colorWipe(uint8_t wait) {
  for(uint16_t i=5; i<=7; i +=1) {
    CircuitPlayground.setPixelColor(i, 125, 255,0);
    delay(wait);
  }
}
