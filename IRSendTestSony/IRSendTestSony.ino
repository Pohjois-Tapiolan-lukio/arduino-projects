
// Adafruit Circuit Playground Express as microcontroller RX and TX IR-leds.
// Arduino/libaries/IRremote/boarddefs.h file DEFAULT_PIN = 25 changed, orginal value for default is 9.

#include <IRremote.h>
 

 
// Create IR Send Object
IRsend irsend;
 
void setup()
{
 
  Serial.begin(9600);
  //Serial.setTimeout(50);
}
 
void loop() {
  String input;
  while(Serial.available() ){
    input = Serial.readStringUntil('\n');

    if (input.equals("1") ){
      for (int i = 0; i < 3; i++) {
        irsend.sendSony(0x10, 12); // TV channel 1
        delay(20);
      }
      delay(1000);
    }

    else if (input.equals("2") ){
      for (int i = 0; i < 3; i++) {
        irsend.sendSony(0xC10, 12); // TV channel 4
        delay(20);
      }
      delay(1000);
    }
    else{
      for (int i = 0; i < 3; i++) {
        irsend.sendSony(0xA10, 12); // TV channel 6
        delay(20);
      }
      delay(1000);
      }
  delay(20);  
 }
} 
