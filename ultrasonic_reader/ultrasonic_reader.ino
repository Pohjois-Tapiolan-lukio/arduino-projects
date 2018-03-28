/***********************
 * Tweakable variables *
 ***********************/

/* Should we print in the Vernier Format 2 format? */
/* (Default: false) */
#define VERNIER_FORMAT false

/* The pin connected to the sensor's Echo pin */
/* (Default: D3) */
#define PIN_ECHO 10

/* The pin connected to the sensor's Trig pin */
/* (Default: D4) */
#define PIN_TRIGGER 11

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

/***************************************
 * Code starts here, modify with care! *
 ***************************************/
#include "math.h"

/* Error checking variable for pollDistance() */
long lastDuration = -1;

void setup() {
  /* Setup the Serial logging and pins */
  Serial.begin(9600);
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  /* Print the Vernier header so the data can be easily copied for later use */
  if (VERNIER_FORMAT) printVernierHeader();
}

void loop() {
  /* Get the distance from the sensor */
  double distance = getDistance();
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
