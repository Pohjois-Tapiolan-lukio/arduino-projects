//PINs and power threshold
#define LASER_PIN 4
#define RELAY_PIN 6
#define RELAY_POWER_THRESHOLD 60

#define INPUT_PIN A1
#define DEDECT_THRESHOLD 1000

/* update frequency, default 4 Hz*/
#define UPDATE_FREQ 4

/*Wheel settings*/
#define SPOKES 24
#define RADIUS 0.111

int polledFrequency = -1;
unsigned long printTime = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, HIGH);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  updateFrequencyCounter;

  if (millis() - printTime > 1000.0/UPDATE_FREQ) {
    float velocity = 2*3.1416*polledFrequency / SPOKES*RADIUS*3.6;
    float power = 13.04*velocity - 17.4; // linear model k*v + b
    Serial.println(power);
    printTime = millis();

    if (power > RELAY_POWER_THRESHOLD) {
      digitalWrite(RELAY_PIN, HIGH);
      }
     else {
      digitalWrite(RELAY_PIN, LOW);
      }
    }

}
// Timestamp when polledFrequency was updated
unsigned long dataUpdateTime = 0;

// Counter how many spokes were dedected since dataUpdateTime
int frequencyCounter = 0;
bool lastBlocked = false;

void updateFrequencyCounter() {
   bool currentlyBlocked = false;
   
   if (analogRead(INPUT_PIN) < DEDECT_THRESHOLD ) {
    currentlyBlocked = true;
    }

   if (currentlyBlocked && !lastBlocked ) {
    frequencyCounter++;
    }

   lastBlocked = currentlyBlocked; 

   if (millis() - dataUpdateTime > (1.0 / UPDATE_FREQ)*1000) {
    dataUpdateTime = millis();
    polledFrequency = frequencyCounter * UPDATE_FREQ;
    frequencyCounter = 0;
    }
  }
