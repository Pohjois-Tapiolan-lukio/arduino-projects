/* This arduino project counts the frequency of a sensor going on and off.
 *  This is a more specific patch of sensor_freq_counter.
 */

/***********************
 * Tweakable variables *
 ***********************/
/* The pin of the sensor which we measure the frequency of */
#define PIN_SENSOR 2

/* The frequency at which polledFrequency is updated. */
/* (Default: 4Hz) */
#define DATA_UPDATE_FREQ 4

/*Application */
#define SPOKES 36
#define RADIUS 0.311

/***************************************
 * Code starts here, modify with care! *
 ***************************************/
/* The latest measured frequency of PIN_SENSOR */
int polledFrequency = -1;

/* Printing delay time */
unsigned long printTime = 0;

void setup() {
  /* Setup the sensor pin */
  pinMode(PIN_SENSOR, INPUT);
  Serial.begin(9600);
}

void loop() {
  /* Update the frequency counter */
  updateFrequencyCounter();

  /* Print the current frequency */
  if (millis() - printTime > 1000.0 / DATA_UPDATE_FREQ) {
    float velocity = 2 * 3.1416 * polledFrequency / SPOKES * RADIUS * 3.6;
    float power = 13.04 * velocity  - 17.4;
    Serial.println(power);
    printTime = millis();
  }
}


/***********************************
 * Frequency counter functionality *
 ***********************************/

/* Timestamp of the last time polledFrequency was updated */
unsigned long dataUpdateTime = 0;
/* How many times the PIN_SENSOR has switched state to HIGH since dataUpdateTime */
int frequencyCounter = 0;
/* State of the counter for comparison */
bool lastBlocked = false;

void updateFrequencyCounter() {
  /* Read the sensor state */
  bool currentlyBlocked = digitalRead(PIN_SENSOR);
  if (currentlyBlocked && !lastBlocked) {
    /* Add to the counter if the sensor just switched to HIGH */
    frequencyCounter++;
  }
  /* Update state for next time */
  lastBlocked = currentlyBlocked;

  if (millis() - dataUpdateTime > (1.0 / DATA_UPDATE_FREQ) * 1000) {
    /* Update polledFrequency at the frequency of DATA_UPDATE_FREQ */
    dataUpdateTime = millis();
    /* Hz = N * Hz */
    polledFrequency = frequencyCounter * DATA_UPDATE_FREQ;
    /* Reset the frequency counter so it starts counting up again */
    frequencyCounter = 0;
  }
}
