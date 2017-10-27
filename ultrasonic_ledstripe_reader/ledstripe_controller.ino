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

/* Adafruit_Neopixel led-strip settings.*/
/* Strip guidance.*/
#define STRIP_PIN 6
/* Choose according to your strip. */
#define NUM_LEDS 100
/* Values between 0 ... 255. */
#define BRIGHTNESS 100
/* How fast animations should scroll? */
/* (Default: 1Hz) */
#define SCROLLING_SPEED 1

#include "Adafruit_NeoPixel.h"
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, STRIP_PIN, NEO_GRB + NEO_KHZ800);

void setupLedStrip() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
}

void setLedRainbow(float time) {
  setLedRainbow(0, NUM_LEDS, time);
}

/* Time is in seconds */
void setLedRainbow(int start, int end, float time) {
  for (float i = start; i < end; i++) {
    float norm = i / (end - start);
    float rgb[3] = {0, 0, 0};
    hueToRgbArray((norm + time * SCROLLING_SPEED) * 360, rgb);
    strip.setPixelColor(i, 255 * rgb[0], 255 * rgb[1], 255 * rgb[2]);
  }
}

void setLedColor(int start, int end, int r, int g, int b)  {
  for (int i = start; i < end; i++) {
    strip.setPixelColor(i, r, g, b);
  }
}

void setLedColor(unsigned int r, unsigned int g, unsigned int b)  {
  setLedColor(0, NUM_LEDS, r, g, b);
}

void updateLeds() {
  strip.show();
}

void hueToRgbArray(int h, float* rgb) {
  /* This code (and hueToRgb() is based on https://en.wikipedia.org/wiki/HSL_and_HSV#From_HSV) */
  for (int i = 0; i < 3; i++) {
    rgb[i] = hueToRgb(i, h);
  }
}

float hueToRgb(int offset, int hue) {
  int hue_ = (int) (hue / 60);
  int x = (6 + hue_ - offset * 2) % 6;
  if (x < 1) {
    return 1;
  } else if (x < 2) {
    return 1.0 - (hue - hue_ * 60) / 60.0;
  } else if (x < 3) {
    return 0;
  } else if (x < 4) {
    return 0;
  } else if (x < 5) {
    return (hue - hue_ * 60) / 60.0;
  } else {
    return 1.0;
  }
}
