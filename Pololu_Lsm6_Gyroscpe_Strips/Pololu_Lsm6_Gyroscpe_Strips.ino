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

#define PIN1 2
#define PIN2 3
#define PIN3 4
#define PIN4 5

#define NUM_LEDS 5
#define BRIGHTNESS 125 // Values between 0 ..255

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2= Adafruit_NeoPixel(NUM_LEDS, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_LEDS, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(NUM_LEDS, PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strips[] = {strip1, strip2, strip3, strip4};

boolean strip1CurrentState = false;

boolean stripsCurrentStates[] = {false, false, false, false};
boolean stripsPreviousStates[] = {true, true, true, true};

boolean strip1PreviousState = true;

 

#define RED  35
#define GREEN  196
#define BLUE  142

LSM6 imu;
int delayTime = 50;
#define DELTA 50

char report[80];

void setup()
{
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
  int rotationState =checkRotationState(delayTime);
  for(int k =0; k < 5; k = k+1){
   if (stripsPreviousStates[k] == 0 and rotationState == 1){
    for ( uint16_t i =0; i <strips[k].numPixels(); i++){
    strips[k].setPixelColor(i, RED,GREEN,BLUE);
    }
    strips[k].show();
    stripsCurrentStates[k] = !stripsCurrentStates[k];
    stripsPreviousStates[k] = !stripsPreviousStates[k];
    delay(200);
   }
  
  if (stripsPreviousStates[k] == 1 and rotationState == -1){
    for ( uint16_t i =0; i <strips[k].numPixels(); i++){
    strips[k].setPixelColor(i, 0,0,0);
    }
    strips[k].show();
    stripsCurrentStates[k] = !stripsCurrentStates[k];
    stripsPreviousStates[k] = !stripsPreviousStates[k];
    delay(500);
    }
 }
}
int checkRotationState(int delayTime){
 imu.read();
 float previousZAngularVelocity= imu.g.z*4.375/1000;
  Serial.print("previous Z-value: ");
  Serial.println(previousZAngularVelocity);  
 delay(delayTime);
 imu.read();
 float currentZAngularVelocity= imu.g.z*4.375/1000;
  Serial.print("current Z-value: ");
  Serial.println(currentZAngularVelocity);
  
 float difference = previousZAngularVelocity - currentZAngularVelocity;
 Serial.print("difference: ");
 Serial.println(difference);
 Serial.println("**");  

 if (difference < -DELTA ){
  //Serial.println("case1");
  return 1;
  }
 if (difference > DELTA){
  //Serial.println("case2");
  return -1;
  
  } else {
    //Serial.println("case3");
    return 0;
    }
}
void loop()
{
  
  changeStripsStates();


}

void lightUp(Adafruit_NeoPixel strip, int red, int green, int blue){

 for ( uint16_t i =0; i <strip.numPixels(); i++){
    strip.setPixelColor(i, red,green,blue);
    }
    strip.show();
}
