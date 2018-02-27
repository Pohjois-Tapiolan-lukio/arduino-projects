/* This arduino project counts the frequency of a sensor going on and off. */

/***********************
 * Tweakable variables *
 ***********************/
/* The pin of the sensor which we measure the frequency of */
#define PIN_SENSOR D4

/* The frequency at which polledFrequency is updated. */
/* (Default: 1Hz) */
#define DATA_UPDATE_FREQ 1

/***************************************
 * Code starts here, modify with care! *
 ***************************************/
/* The latest measured frequency of PIN_SENSOR */
int polledFrequency = -1;

void setup() {
  /* Setup the sensor pin */
  pinMode(PIN_SENSOR, INPUT);
}

void loop() {
  /* Update the frequency counter */
  updateFrequencyCounter();

  /* Print the current frequency */
  Serial.println(polledFrequency);
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
