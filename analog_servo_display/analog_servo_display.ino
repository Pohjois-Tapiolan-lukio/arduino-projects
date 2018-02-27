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
#define PIN_SENSOR A2
#define SCALE  10

/* Sample printing frequency (0 disables printing) */
#define SAMPLE_PRINT_FREQ 2

/***************************************
 * Code starts here, modify with care! *
 ***************************************/
#include "Servo.h"

/* The servo that displays the player's current applied force */
Servo servo;

/* Printing variables */
float totalSamples = 0;
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
  float currentPower = SCALE*(analogRead(PIN_SENSOR) / 512.0 - 1.0);

  /* Display the analog sensor's input with the servo */
  servo.write(90 + (int) (90 * currentPower));

  /* Keep track of samples if printing is enabled */
  if (SAMPLE_PRINT_FREQ > 0) {
    /* Add the current sample to the total and keep count */
    totalSamples += currentPower;
    totalSampleCount++;
    if (millis() - printTime > 1000.0 / SAMPLE_PRINT_FREQ) {
      /* Calculate the average sample */
      float sampleAvg = totalSamples / totalSampleCount;
      /* Print the sample (x = time, y = sensor samples) */
      printGeoGebraPoint(millis() / 1000.0, sampleAvg);
      /* Reset printing and average counting variables */
      printTime = millis();
      totalSamples = 0;
      totalSampleCount = 0;
    }
  }
}
