/*
The sensor outputs provided by the library are the raw
16-bit values obtained by concatenating the 8-bit high and
low accelerometer and gyro data registers. They can be
converted to units of g and dps (degrees per second) using
the conversion factors specified in the datasheet for your
particular device and full scale setting (gain).
Example: An LSM6DS33 gives an accelerometer Z axis reading
of 16276 with its default full scale setting of +/- 2 g. The
LA_So specification in the LSM6DS33 datasheet (page 15)
states a conversion factor of 0.061 mg/LSB (least
significant bit) at this FS setting, so the raw reading of
16276 corresponds to 16276 * 0.061 = 992.8 mg = 0.9928 g.
*/

#include <Wire.h>
#include <LSM6.h>
#include<Adafruit_NeoPixel.h>

//Pinout for Wemos mini d1. For Arduino Uno use digitalpins, #define PIN1 2
#define PIN1 D8
#define PIN2 D7
#define PIN3 D6
#define PIN4 D5

#define NUM_LEDS 5
#define BRIGHTNESS 80 // Values between 0 ..255

// Lamp consisting of four independent led strips.
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2= Adafruit_NeoPixel(NUM_LEDS, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_LEDS, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(NUM_LEDS, PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strips[] = {strip1, strip2, strip3, strip4};



boolean stripsCurrentStates[] = {false, false, false, false};
boolean stripsPreviousStates[] = {true, true, true, true};

int previousRotationState = 1;
int currentRotationState = 0;

int showType = 0;

// Select the RGB -values from 0...255.
#define RED  35
#define GREEN  196
#define BLUE  142

// Gyroscope imu
LSM6 imu;
int delayTime = 50;

//Threshold value for difference in angular velocity, causes the of change of leds states.
#define DELTA 30 



void setup(){
  Serial.begin(9600);
  Wire.begin();

  if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  imu.enableDefault();

   for(int k =0; k < 5; k = k+1){ 
   strips[k].begin();
   }

   for(int k =0; k < 5; k = k+1){ 
    for ( uint16_t i =0; i <strip1.numPixels(); i++){
      strips[k].setPixelColor(i, 255,0,0);
      strips[k].setBrightness(BRIGHTNESS);
    }
   strips[k].show();
   stripsCurrentStates[k] = !stripsCurrentStates[k];
    stripsPreviousStates[k] = !stripsPreviousStates[k]; 
   }
   
   
}

void changeStripsStates(){
   currentRotationState =checkRotationState(delayTime);
   
   if (previousRotationState == 0 and currentRotationState == 1){
    showType++;
    startShow(showType);
    }
   
   
  if ((previousRotationState  == 1 or previousRotationState == 0) and currentRotationState == -1){
    startShow(0); // Leds off
    }
  if(previousRotationState == -1 and currentRotationState == 1){
    showType++;
    startShow(showType);
    
    }
  if (showType >9){
    showType = 1;
    }
    previousRotationState = currentRotationState;
}

int checkRotationState(int delayTime){
 imu.read();
 float previousZAngularVelocity= imu.g.z*4.375/1000;
 delay(delayTime);
 imu.read();
 float currentZAngularVelocity= imu.g.z*4.375/1000;
 float difference = previousZAngularVelocity - currentZAngularVelocity; 

 if (difference < -DELTA ){
  return 1;
  }
 if (difference > DELTA){
  return -1;
  
  } else {
    return 0;
    }
}


void loop(){
  changeStripsStates();
}

void startShow(int i){
  switch(i){
    case 0: 
            for( int k = 0; k < 5; k = k+1){
              colorWipe(strips[k].Color(0, 0, 0),50); // Black/off
             //strips[k].show();
             //tripsCurrentStates[k] = !stripsCurrentStates[k];
             //stripsPreviousStates[k] = !stripsPreviousStates[k];
             //delay(100);
            }
            break;
             
    case 1: 
            for( int k = 0; k < 5; k = k+1){
              colorWipe(strips[k].Color(255, 0, 0),50); // Red
             //strips[k].show();
             //tripsCurrentStates[k] = !stripsCurrentStates[k];
             //stripsPreviousStates[k] = !stripsPreviousStates[k];
             //delay(100);
             }
             break;
            
    case 2:  
            for( int k = 0; k < 5; k = k+1){
              colorWipe(strips[k].Color(0, 255, 0),50); // Green
             //strips[k].show();
             //tripsCurrentStates[k] = !stripsCurrentStates[k];
             //stripsPreviousStates[k] = !stripsPreviousStates[k];
             //delay(200);
             }
             break;
            
    case 3: 
            for( int k = 0; k < 5; k = k+1){
              colorWipe(strips[k].Color(0, 0, 255),50); // Blue
             //strips[k].show();
             //tripsCurrentStates[k] = !stripsCurrentStates[k];
             //stripsPreviousStates[k] = !stripsPreviousStates[k];
             //delay(100);
           
            }
            break;
            
    case 4:
            for( int k = 0; k < 5; k = k+1){
              theaterChase(strips[k].Color(127, 127, 127),50); // White
             //strips[k].show();
             //tripsCurrentStates[k] = !stripsCurrentStates[k];
             //stripsPreviousStates[k] = !stripsPreviousStates[k];
             //delay(100);
              }
            break;
            
    case 5:
            for( int k = 0; k < 5; k = k+1){
              theaterChase(strips[k].Color(127, 0, 0),50); // Red
             //strips[k].show();
             //tripsCurrentStates[k] = !stripsCurrentStates[k];
             //stripsPreviousStates[k] = !stripsPreviousStates[k];
             //delay(100);
             }
             break;
             
    case 6: 
            for( int k = 0; k < 5; k = k+1){
             theaterChase(strips[k].Color(0, 0, 127),50); // Blue
             //strips[k].show();
             //delay(100);
            }
            break;
            
    case 7: 
            rainbow(20);
            break;
            
    case 8: 
            rainbowCycle(20);
            break;
            
    case 9: 
            theaterChaseRainbow(50);
            break;
            
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  //for (int k =0; k < 5; k = k+1){
    for ( uint16_t i =0; i <strips[1].numPixels(); i++){
      for(int k =0; k < 5; k = k+1){
      strips[k].setPixelColor(i, c);
      strips[k].show();
      delay(wait);
    }
  
    
    //stripsCurrentStates[k] = !stripsCurrentStates[k];
    //stripsPreviousStates[k] = !stripsPreviousStates[k];
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  
  //for (int k =0; k < 5; k = k+1){
    for(j=0; j<256; j++) {
      for(i=0; i<strips[1].numPixels(); i++) {
        for (int k =0; k < 5; k = k+1){
        strips[k].setPixelColor(i, Wheel((i+j) & 255,k));
      strips[k].show();
        }
      delay(wait);
  }
 } 
}



void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
 //for(int k=0; k < 5; k = k+1){
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strips[1].numPixels(); i++) {
      for(int k=0; k < 5; k = k+1){
      strips[k].setPixelColor(i, Wheel(((i * 256 / strips[k].numPixels()) + j) & 255, k));
      strips[k].show();
    }
    
    delay(wait);
  }
 }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
 //for(int k=0; k < 5; k = k+1){ 
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strips[1].numPixels(); i=i+3) {
        for(int k=0; k < 5; k = k+1){
        strips[k].setPixelColor(i+q, c);    //turn every third pixel on
        strips[k].show();
      }
      

      delay(wait);

      for (int i=0; i < strips[1].numPixels(); i=i+3) {
        for(int k=0; k < 5; k = k+1){
        strips[k].setPixelColor(i+q, 0);
        //turn every third pixel off
        strips[k].show();
        }
      }
    }
  }
 }
}

void theaterChaseRainbow(uint8_t wait) {
 //for(int k=0; k < 5; k = k+1){  
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strips[1].numPixels(); i=i+3) {
        for(int k=0; k < 5; k = k+1){
        strips[k].setPixelColor(i+q, Wheel( (i+j) % 255, k));    //turn every third pixel on
        strips[k].show();
      }
      

      delay(wait);

      for (int i=0; i < strips[1].numPixels(); i=i+3) {
        for(int k=0; k < 5; k = k+1){
        strips[k].setPixelColor(i+q, 0); 
        strips[k].show();//turn every third pixel off
        }
      }
    }
  }
 }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to 
uint32_t Wheel(byte WheelPos, int k) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strips[k].Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strips[k].Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strips[k].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
