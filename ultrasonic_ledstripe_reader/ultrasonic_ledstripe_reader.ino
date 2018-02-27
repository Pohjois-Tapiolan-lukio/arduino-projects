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

/* How often are the leds updated? */
/* (Default: 24Hz) */
#define LED_UPDATE_FREQ 24
/* How much distance variation can be considered "still"? */
/* (Default: 1cm) */
#define STILL_DISTANCE_DELTA 1

/***************************************
 * Code starts here, modify with care! *
 ***************************************/
#include "math.h"

/* Led management variables */
unsigned long lastLedUpdateTime = 0;
double distanceDeltaTotal = 0;
int distanceDeltaCount = 0;
double lastDistance = 0;

/* Error checking variable for pollDistance() */
long lastDuration = -1;

void setup() {
  /* Setup the Serial logging and pins */
  Serial.begin(9600);
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  /* Print the Vernier header so the data can be easily copied for later use */
  if (VERNIER_FORMAT) printVernierHeader();

  setupLedStrip();
}

void loop() {
  /* Get the distance from the sensor */
  double distance = getDistance();
  updateLedDisplay(distance);

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

/* Led display functionality */

void updateLedDisplay(double distance) {
  distanceDeltaTotal += distance - lastDistance;
  lastDistance = distance;
  distanceDeltaCount++;
  if (millis() - lastLedUpdateTime > 1000.0 / LED_UPDATE_FREQ) {
    double avgDistanceDelta = distanceDeltaTotal / distanceDeltaCount;
    showLedDistance(avgDistanceDelta, distance);
    lastLedUpdatetime = millis();
    distanceDeltaTotal = 0;
    distanceDeltaCount = 0;
  }
}

void showLedDistance(double distanceDelta, double distance) {
  /* Reset the strip */
  setLedColor(0, 0, 0);
  /* Leds */
  int num_leds = mapLedNumber(distance);

  if (distanceDelta < STILL_DISTANCE_DELTA) {
    /* The distance is stable, show the distance on the led display */
    setLedColor(0, 5, 255, 0, 0);
    setLedColor(5, num_leds, 0, 0, 255);
  } else {
    /* The distance is changing a lot, show the loading animation */
    setLedRainbow(millis() / 1000.0);
  }

  /* Send the updated pixel color to the hardware after all the changes have been made */
  updateLeds();
}

/* Converts the measured distance range into number of leds ON. Led range from index 5...into NUM_LEDS. First 5 RED leds are always ON.
 * Linear model y=kx+B, where x=distance, y=number of leds. Points used (2,200) and (5, NUM_LED). */
int mapLedNumber(double distance) {
  return int(5 + max(0.0, min(1.0, (distance - 2.0) / 198.0)) * (NUM_LED - 5));
}
