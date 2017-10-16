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

/* Displays the input current in an analog pin by moving a servo. */

/* Notes about connecting the Vernier Protoboard Adapter
 * - GND to Arduino pin GND (ground)
 * - Vres to Arduino pin A4 (resistance reference)
 * - 5V to Arduino pin 5V (power)
 * - SIG1 to Arduino pin A0 (0-5V output used by almost all Vernier sensors)
 */

/***********************
 * Tweakable variables *
 ***********************/

/* Pins */
#define PIN_SERVO 8
#define PIN_SENSOR1 A0
#define PIN_SENSOR2 A2
#define SCALE1  0.025
#define SCALE2 5

/* Sample printing frequency (0 disables printing) */
#define SAMPLE_PRINT_FREQ 2

/***************************************
 * Code starts here, modify with care! *
 ***************************************/
#include "Servo.h"

/* The servo that displays the player's current applied force */
Servo servo;

/* Printing variables */
float totalSamples1 = 0;
float totalSamples2 = 0;
int totalSampleCount = 0;
unsigned long printTime = 0;

void setup() {
  /* Setup pins */
  servo.attach(PIN_SERVO);
  /* Setup serial if printing is enabled */
  if (SAMPLE_PRINT_FREQ > 0) Serial.begin(9600);
}

void loop() {
  /* Poll the sensor (map from 0..1024 to -1..1) */
  float currentQuantity1 = SCALE1*(analogRead(PIN_SENSOR1) / 512.0 - 1.0);
  float currentQuantity2 = SCALE2*(analogRead(PIN_SENSOR2) / 512.0 - 1.0);

  /* Display the analog sensor's input with the servo */
  servo.write(90 + (int) (90 * currentQuantity1));

  /* Keep track of samples if printing is enabled */
  if (SAMPLE_PRINT_FREQ > 0) {
    /* Add the current sample to the total and keep count */
    totalSamples1 += currentQuantity1;
    totalSamples2 += currentQuantity2;
    totalSampleCount++;
    if (millis() - printTime > 1000.0 / SAMPLE_PRINT_FREQ) {
      /* Calculate the average sample */
      float sampleAvg1 = totalSamples1 / totalSampleCount;
      float sampleAvg2 = totalSamples2 / totalSampleCount;
      /* Print the sample (x = time, y = sensor samples) */
      printGeoGebraPoint(sampleAvg1, sampleAvg2);
      /* Reset printing and average counting variables */
      printTime = millis();
      totalSamples1 = 0;
      totalSamples2 = 0;
      totalSampleCount = 0;
    }
  }
}
