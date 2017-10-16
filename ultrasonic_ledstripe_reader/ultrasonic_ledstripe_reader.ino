/* Copyright 2017 Jens Pitkanen
 *
 * Permission to use, copy, modify, and/or distribute this software for any 
 * purpose with or without fee is hereby granted, provided that the above 
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE.
 */

/***********************
 * Tweakable variables *
 ***********************/

/* Should we print in the Vernier Format 2 format? */
/* (Default: false) */
#define VERNIER_FORMAT false

/* The pin connected to the sensor's Echo pin */
/* (Default: D3) */
#define PIN_ECHO 3

/* The pin connected to the sensor's Trig pin */
/* (Default: D4) */
#define PIN_TRIGGER 4

/* Variables for tweaking the error filtering */
/* How many times should we re-poll in case of errors? */
/* (Default: 3) */
#define RE_POLL_TIMES 3
/* How many seconds do we allow valid data to change per poll? */
/* (Default: 2s) */
#define ERROR_MARGIN 2
/* How large an error (in cm) will we simply ignore even after multiple tries? */
/* (Default: 5m) */
#define ERROR_MARGIN_ABSOLUTE 500

/* Adafruit_Neopixel led-strip settings.*/

#define  STRIP_PIN 6    //Strip guidance.

#define NUM_LEDS 100     //Choose according to your strip.

#define BRIGHTNESS 100  // Values between 0 ... 255.

#define DEALAY_VAL 10   // Delay for leds.

/***************************************
 * Code starts here, modify with care! *
 ***************************************/
#include "math.h"
#include "Adafruit_NeoPixel.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, STRIP_PIN, NEO_GRB + NEO_KHZ800); 

/* Error checking variable for pollDistance() */
long lastDuration = -1;
int lastNumLeds = 0;

void setup() {
  /* Setup the Serial logging and pins */
  Serial.begin(9600);
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  /* Print the Vernier header so the data can be easily copied for later use */
  if (VERNIER_FORMAT) printVernierHeader();

  strip.begin();
  putStripON();
}

void loop() {
  /* Get the distance from the sensor */
  double distance = getDistance();

  showLedDistance(distance);

  //delay(500);
 // putStripOFF();
  

  
  /* Print the data in a Vernier-compatible format */
  if (VERNIER_FORMAT) printVernierData(distance, millis());
  else Serial.println(distance);
}

/* Prints the Vernier Format 2 header */
void printVernierHeader() {
  Serial.println("Vernier Format 2");
  Serial.println("Ultrasonic sensor readings");
  Serial.println("Data\tSet");
  Serial.println("Time\tDistance");
  Serial.println("t\tx");
  Serial.println("seconds\tcm");
}

/* Prints a point of data in the Vernier Format 2 format */
void printVernierData(double distance, unsigned long time) {
  Serial.print(time / 1000.0);
  Serial.print("\t");
  Serial.println(distance);
}

/* Calculates the distance by polling the duration and converting it into cm */
double getDistance() {
  long duration = pollDuration(0);
  return (duration / 2.0) / 29.1;
}

/* Returns the echo duration that was polled */
long pollDuration(int tryCount) {
  /* Get the duration of the ping from the sensors */
  digitalWrite(PIN_TRIGGER, LOW);
  delay(5);
  digitalWrite(PIN_TRIGGER, HIGH);
  delay(10);
  digitalWrite(PIN_TRIGGER, LOW);
  long duration = pulseIn(PIN_ECHO, HIGH);

  /* NOTE: You could just `return duration` here, and remove the rest of the code. */
  /*       It's all just filtering code for polling errors.                        */

  
  /* Check the polled data for errors (if this isn't the first run) */
  if (lastDuration != -1) {
    /* Calculate the delta */
    long delta = abs(duration - lastDuration);
    if (delta > ERROR_MARGIN && tryCount < RE_POLL_TIMES) {
      /* The delta is larger than the error margin, try polling again. */
      duration = pollDuration(tryCount + 1);
    }
    if (delta > ERROR_MARGIN_ABSOLUTE) {
      /* The data is still very erroneus-looking, ignore this poll */
      return lastDuration;
    }
  }

  /* Update lastDuration for use next time */
  lastDuration = duration;
  
  return duration;
}

/*
Converts the measured distance range into number of leds ON. Led range from index 5...into NUM_LEDS. First 5 REDleds allways ON.
Linear model y=kx+B, where x=distance, y=number of leds. Points used (2,200) and (5, NU_LED).

*/

int mapLedNumber(double distance) {
  return int(distance*95/198 + 400/99);

}



void putStripON() {
  for(int i=0;i<5;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    strip.setPixelColor(i, strip.Color(255,0,0)); // First 5 leds are RED.

    strip.show(); // This sends the updated pixel color to the hardware.

    //delay(delayval);
 }
}

void putStripOFF() {
  for(int i=5;i<NUM_LEDS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    strip.setPixelColor(i, strip.Color(0,0,0)); // First 5 leds are RED.

    strip.show(); // This sends the updated pixel color to the hardware.

    //delay(delayval);
 }
}


void showLedDistance(double distance) {

  int num_leds = mapLedNumber(distance);

  if ( num_leds != lastNumLeds){
  putStripOFF();
  //delay(50);

  
  for(int i=5;i<num_leds;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    strip.setPixelColor(i, strip.Color(0,0,255)); // Distance prested with blue LEDs.

    strip.show(); // This sends the updated pixel color to the hardware.

    //delay(delayval);
  }
  lastNumLeds = num_leds;
 }
}
