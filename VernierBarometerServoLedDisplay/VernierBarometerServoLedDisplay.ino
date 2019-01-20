/* Notes about connecting the Vernier Protoboard Adapter
 * - GND to Arduino pin GND (ground)
 * - Vres to Arduino pin A4 (resistance reference)
 * - 5V to Arduino pin 5V (power)
 * - SIG1 to Arduino pin A0 (0-5V output used by almost all Vernier sensors)
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
#define PIN_SERVO 8
#define PIN_SENSOR A0

#include "Servo.h"

/* Pressure Scale: for Servo analogical display */
#define MIN_VAL 85
#define MAX_VAL 185

Servo myServo;
int value;
float voltage;
float pressure;
float previousPressure;


/* Optional LedStrip */
#include <Adafruit_NeoPixel.h>
#define PIN 12
#define NUM_LEDS 35   // default H- Letter 10
#define BRIGHTNESS 255  // Values between 0 ... 255.
#define DEALAY_VAL 10

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);



void setup() {
  // put your setup code here, to run once:
  myServo.attach(PIN_SERVO);
  Serial.begin(115200);
  myServo.write(90);

  strip.begin();

  for( uint16_t i =0; i < 5; i++){
    strip.setPixelColor(i, 0,0,255);
    }

  strip.setBrightness(125);
  strip.show();

  value = analogRead(PIN_SENSOR);
  voltage = value/1024.0*5.0;
  previousPressure = voltage*51.71 - 25.86;

}

void loop() {
  // put your main code here, to run repeatedly:
  measurePressure();
  updateServo();
  updateLeds();
  
  Serial.println(value);
  Serial.print("Pressure: ");
  Serial.println(pressure);
   
  updatePreviousPressure();
  delay(10);
 
}
void measurePressure(){
  value = analogRead(PIN_SENSOR);
  voltage = value/1024.0*5.0;
  pressure = (voltage*51.71 - 25.86);
  }
  
void updatePreviousPressure(){
  previousPressure = pressure;
  }

void updateServo(){
  float delta = abs(pressure - previousPressure);
  Serial.print("Servo delta: ");
  Serial.println(delta);

  if (delta > 2.0){
    int servoAngle = map(pressure, MIN_VAL, MAX_VAL, 180, 0);
    myServo.write(servoAngle);
    }
  }
void updateLeds(){
  float delta = abs(pressure - previousPressure);

  if (delta > 2.0){
    for( uint16_t i =5; i < strip.numPixels(); i++){
    strip.setPixelColor(i, 0,0,0);
    }
  strip.show();

  int ledCount = map(pressure, MIN_VAL, MAX_VAL, 0, 30);
   for( uint16_t i =5; i < ledCount; i++){
    strip.setPixelColor(i, 255,0,0);
    }

  //strip.setBrightness(125);
  strip.show();


    }
  }
