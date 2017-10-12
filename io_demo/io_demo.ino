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

/* The idea of this arduino project is to make a motor spin, and have some
 *  "warning" led lights and a piezo while it is spinning. The motor starts
 *  spinning when the button is pressed.
 */

/***********************
 * The pin definitions *
 ***********************/
#define PIN_BUTTON 13
#define PIN_RED 2
#define PIN_GREEN 4
#define PIN_PIEZO 6
#define PIN_MOTOR 8

/***************************************
 * Code starts here, modify with care! *
 ***************************************/

/* Track the button pin state */
bool buttonWasPressed = false;
/* Track the output state */
bool running = false;

void setup() {
  /* Initialize the pins */
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_PIEZO, OUTPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);
}

void loop() {
  /* Update the state when the button is pressed */
  if (buttonPressed()) {
    running = !running;
  }

  if (running) {
    /* Set the motor spinning */
    digitalWrite(PIN_MOTOR, HIGH);

    /* Get the state of the leds from the time (green on even seconds) */
    bool green = (int)floor(millis() / 1000) % 2 == 0;
    if (green) {
      /* Set the green led on, red led & piezo off */
      digitalWrite(PIN_GREEN, HIGH);
      digitalWrite(PIN_RED, LOW);
      noTone(PIN_PIEZO);
    } else {
      /* Set the red led & piezo on, green led off */
      digitalWrite(PIN_RED, HIGH);
      digitalWrite(PIN_GREEN, LOW);
      tone(PIN_PIEZO, 440, 1000);
    }
  } else {
    /* Shut down everything when not running */
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_PIEZO, LOW);
    digitalWrite(PIN_MOTOR, LOW);
    noTone(PIN_PIEZO);
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
