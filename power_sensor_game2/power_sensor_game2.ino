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
/* (Default: 30%) */
#define THRESHOLD_YELLOW 0.3
/* How close (%) does the input have to be to trigger green light? */
/* (Default: 10%) */
//#define THRESHOLD_GREEN 0.1

#define THRESHOLD_GREEN 0.1

/* How long do notes play? */
/* (Default: 250ms) */
#define NOTE_LENGTH 100

/* Jingles */
/* A A - */
int notesStart[3] = {220, 220, 0};
/* G H D */
int notesWin[3]   = {196, 247, 294};
/* H G#  */
int notesStop[3]  = {247, 208, 208};

/***************************************
 * Code starts here, modify with care! *
 ***************************************/
#include "Servo.h"

/* The servo that displays the player's current applied force */
Servo servo;

/* The game state */
bool running = false;
float targetPower = -1;

/* Winning conditions */
bool winning = false;
unsigned long winTime = 0;

/* Button data */
bool buttonWasPressed = false;

/* Sound system data */
int* notes;
int notesCount;
int currentIndex = -1;
unsigned long soundStartTime = 0;

void setup() {
  /* Setup pins */
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_PIEZO, OUTPUT);
  pinMode(PIN_SERVO, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);
  servo.attach(PIN_SERVO);
}

void loop() {
  if (buttonPressed()) {
    /* When the button is pressed, start/stop the game */
    toggleGameState();
  }
  if (running) {
    /* Update game when running */
    updateGame();
  } else {
    /* Game is not running, set all leds off and return the servo to normal state */
    digitalWrite(PIN_RED, LOW);
    analogWrite(PIN_YELLOW, 0);
    analogWrite(PIN_GREEN, 0);
    servo.write(90);
  }
  /* Update the sound system */
  updateSoundSystem();
}

void toggleGameState() {
  /* Switch the game state */
  running = !running;
  if (running) {
    /* If the game was started, play starting jingle */
    playSound(notesStart);

    /* Initialize the game state variables */
    /* Randomize the target power */
    randomSeed(millis());
    targetPower = random(0, 1024) / 1024.0;
    /* Ensure the winning state is appropriate */
    winning = false;
    winTime = 0;
  } else {
    /* The game was stopped, play stop jingle */
    playSound(notesStop);
  }
}

void playSound(int* notes_) {
  /* Update the notes and set the sound start time to the current time */
  /* (See updateSoundSystem() for what these variables are used for.) */
  notes = notes_;
  soundStartTime = millis();
  currentIndex = 0;
}

void updateGame() {
  /* Poll the sensor */
  float currentPower = analogRead(PIN_POWER) / 1024.0;
  /* Calculate the distance from the target */
  float distance = abs(targetPower - currentPower);
  /* If we're within the threshold, set yellow and/or green to a value between 0 and 1 */
  /* (0 means the player is at the threshold, 1 means the player is right on the target) */
  float yellow = max(0, THRESHOLD_YELLOW - distance) / THRESHOLD_YELLOW;
  float green = max(0, THRESHOLD_GREEN - distance) / THRESHOLD_GREEN;

  /* Display the player's input with the servo */
  servo.write(90 + (int) (90 * (currentPower * 2 - 1)));

  /* If the player is currently winning and has been for a second, win the game and play the jingle */
  if (winning && millis() - winTime > 1000) {
    running = false;
    playSound(notesWin);
  }

  if (green > 0) {
    /* Player is within the green threshold, set yellow and red off */
    digitalWrite(PIN_RED, LOW);
    analogWrite(PIN_YELLOW, 0);
    /* Set the green led on */
    analogWrite(PIN_GREEN, (int) floor(128 + 127 * green));
    /* If the player wasn't winning already, start counting by setting the time */
    if (!winning) {
      winning = true;
      winTime = millis();
    }
  } else if (yellow > 0) {
    /* Player is within the yellow threshold, set green and red off */
    digitalWrite(PIN_RED, LOW);
    analogWrite(PIN_GREEN, 0);
    /* Set the yellow led on */
    analogWrite(PIN_YELLOW, (int) floor(128 + 127 * yellow));
    /* Ensure the player is not winning anymore */
    winning = false;
  } else {
    /* Player is not within any threshold, set only the red led on */
    digitalWrite(PIN_RED, HIGH);
    analogWrite(PIN_YELLOW, 0);
    analogWrite(PIN_GREEN, 0);
    /* Ensure the player is not winning anymore */
    winning = false;
  }
}

void updateSoundSystem() {
  if (currentIndex != -1 && millis() - soundStartTime > 250 * currentIndex) {
    /* FIXME: Make this 3 non-hardcoded */
    if (currentIndex >= 3) {
      /* Add to the currentIndex, if there are no more notes left, set it to -1 */
      currentIndex = -1;
      noTone(PIN_PIEZO);
    } else {
      /* There are still sounds to play (current index is valid) */
      /* And NOTE_LENGTH has passed since the last note was played */
      int note = notes[currentIndex];
      /* Play the note (if it exists) */
      if (note != 0) tone(PIN_PIEZO, note);
      else noTone(PIN_PIEZO);
      /* Increment currentIndex */
      currentIndex++;
    }
  }
}

bool buttonPressed() {
  /* Get the button state */
  bool pressed = digitalRead(PIN_BUTTON) == HIGH;
  /* Only register button presses for one loop per press */
  /* Ie. if button is pressed now and wasn't during the last loop */
  bool justPressed = pressed && !buttonWasPressed;

  buttonWasPressed = pressed;
  return justPressed;
}
