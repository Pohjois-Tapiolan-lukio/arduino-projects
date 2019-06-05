#include <Adafruit_NeoPixel.h>
#include <IRLibAll.h>
 
IRrecv myReceiver(2);//receiver on pin 2
IRdecode myDecoder;//Decoder object
 
//One NeoPixel connected to pin 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1,6,NEO_GRB + NEO_KHZ800);
 
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  myReceiver.enableIRIn(); // Start the receiver
}
 
void loop() {
  if (myReceiver.getResults()) {
    myDecoder.decode();
    if (myDecoder.protocolNum == NEC) {
      switch(myDecoder.value) {
        case 0xfd00ff:  //Volume Down
          strip.setPixelColor(0,255,0,0);//Red
          break;
        case 0xfd807f:  //Play/Pause
          strip.setPixelColor(0,0,255,0);//Green
          break;
        case 0xfd40bf:  //Volume Up
          strip.setPixelColor(0,0,0,255);//Blue
          break;
      }
    strip.show();
    myReceiver.enableIRIn(); //Restart the receiver
    }
  }
}
