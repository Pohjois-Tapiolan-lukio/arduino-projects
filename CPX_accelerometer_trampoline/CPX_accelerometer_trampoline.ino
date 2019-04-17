#include <Adafruit_Circuit_Playground.h>

float threshold = 9.9;

int jumps = 0;
int stopTask = -1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  CircuitPlayground.begin();
  

}

void loop() {
  // put your main code here, to run repeatedly:

   int value = Serial.parseInt();

   if (value == 1){
    // Start counting jumps
    jumps = 0;
   }
    float accX = CircuitPlayground.motionX();
    float accY = CircuitPlayground.motionY();
    float accZ = CircuitPlayground.motionZ();

   
  
    if( (abs(accX) > threshold) || (abs(accY) > threshold) || (abs(accZ) > threshold) ){
      jumps += 1;
    }

     
    if ( value == 0){
      // Stop Counting and send value and Check
      Serial.println(jumps);
      delay(100);
      Serial.println(stopTask);
      }


   delay(10);
}
