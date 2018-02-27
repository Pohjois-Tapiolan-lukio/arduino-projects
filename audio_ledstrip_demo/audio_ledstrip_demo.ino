/***********************
 * Tweakable variables *
 ***********************/

#define PIN_AUDIO A0
#define NUM_LEDS 100
#define MIN_VOL 0.4
#define MAX_VOL 0.8

/***************************************
 * Code starts here, modify with care! *
 ***************************************/
#include "math.h"

void setup() {
  /* Setup the Serial logging and pins */
  Serial.begin(9600);
  setupLedStrip();
}

void loop() {
  /* Get the distance from the sensor */
  double volume = max(0, min(1, (analogRead(PIN_AUDIO) / 1024.0 - MIN_VOL) / (MAX_VOL - MIN_VOL)));
  showLedVolume(volume);
  Serial.println(volume);
}

/* Led display functionality */
void showLedVolume(double volume) {
  /* Reset the strip */
  setLedColor(0, 0, 0);
  /* Leds */
  int num_leds = (int) round(volume * (NUM_LEDS - 1));
  setLedColor(0, 1, 190, 160, 0);
  setLedColor(1, 1 + num_leds, 230 * volume * volume, 140 * (1 - volume), 230 * (1 - volume));

  /* Send the updated pixel color to the hardware after all the changes have been made */
  updateLeds();
}
