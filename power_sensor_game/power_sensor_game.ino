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

/* A game for the arduino, requires a power sensor, a piezo, a button,
 * and 3 leds. The power sensor used in testing is the
 * Vernier Dual-Range Force Sensor.
 */

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
#define PIN_RED 2
#define PIN_YELLOW A2
#define PIN_GREEN A1
#define PIN_PIEZO 6
#define PIN_BUTTON 12
#define PIN_SERVO 8
#define PIN_POWER A0

/* How close (%) does the input have to be to trigger yellow light? */
#define THRESHOLD_YELLOW 0.3
/* How close (%) does the input have to be to trigger green light? */
#define THRESHOLD_GREEN 0.11

/***************************************
 * Code starts here, modify with care! *
 ***************************************/
#include "Servo.h"

Servo servo;

/* The game state */
bool running = false;
float targetPower;

/* Winning conditions */
bool winning = false;
unsigned long winTime = 0;

void setup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_PIEZO, OUTPUT);
  pinMode(PIN_SERVO, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);
  servo.attach(PIN_SERVO);
}

void loop() {
  if (buttonPressed()) {
    toggleGameState();
  }
  if (running) {
    updateGame();
  } else {
    digitalWrite(PIN_RED, LOW);
    analogWrite(PIN_YELLOW, 0);
    analogWrite(PIN_GREEN, 0);
    servo.write(90);
  }
}

void toggleGameState() {
  running = !running;
  if (running) {
    tone(PIN_PIEZO, 360, 300);
    randomSeed(millis());
    targetPower = random(0, 1024) / 1024.0;
    winning = false;
    winTime = 0;
  } else {
    tone(PIN_PIEZO, 110, 300);
  }
}

void updateGame() {
  float currentPower = analogRead(PIN_POWER) / 1024.0;
  float distance = abs(targetPower - currentPower);
  float yellow = max(0, THRESHOLD_YELLOW - distance) / THRESHOLD_YELLOW;
  float green = max(0, THRESHOLD_GREEN - distance) / THRESHOLD_GREEN;

  servo.write(90 + (int) (90 * (currentPower * 2 - 1)));

  if (winning && millis() - winTime > 1000) {
    running = false;
    tone(PIN_PIEZO, 600, 300);
  }

  if (green > 0) {
    digitalWrite(PIN_RED, LOW);
    analogWrite(PIN_YELLOW, 0);
    analogWrite(PIN_GREEN, (int) floor(128 + 127 * green));
    if (!winning) {
      winning = true;
      winTime = millis();
    }
  } else if (yellow > 0) {
    digitalWrite(PIN_RED, LOW);
    analogWrite(PIN_YELLOW, (int) floor(128 + 127 * yellow));
    analogWrite(PIN_GREEN, 0);
    winning = false;
  } else {
    digitalWrite(PIN_RED, HIGH);
    analogWrite(PIN_YELLOW, 0);
    analogWrite(PIN_GREEN, 0);
    winning = false;
  }
}

bool buttonWasPressed = false;
bool buttonPressed() {
  /* Get the button state */
  bool pressed = digitalRead(PIN_BUTTON) == HIGH;
  /* Only register button presses for one loop per press */
  /* Ie. if button is pressed now and wasn't during the last loop */
  bool justPressed = pressed && !buttonWasPressed;

  buttonWasPressed = pressed;
  return justPressed;
}
