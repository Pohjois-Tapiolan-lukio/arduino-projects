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
#define PIN_SENSOR A0

/***************************************
 * Code starts here, modify with care! *
 ***************************************/
#include "Servo.h"

/* The servo that displays the player's current applied force */
Servo servo;

void setup() {
  /* Setup pins */
  servo.attach(PIN_SERVO);
}

void loop() {
  /* Poll the sensor */
  float currentPower = analogRead(PIN_SENSOR) / 1024.0;

  /* Display the analog sensor's input with the servo */
  servo.write(90 + (int) (90 * (currentPower * 2 - 1)));
}
