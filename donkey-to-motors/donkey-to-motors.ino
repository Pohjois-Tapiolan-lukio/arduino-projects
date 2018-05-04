// The purpose of this project is to be a bridge between a Donkey Car Raspberry and a car that has two motors.

// These pins are connected to the Donkey Car
#define PIN_SPEED A0
#define PIN_STEER A1

// These pins control the directions of the motors
#define PIN_MOTOR_LEFT_A 9
#define PIN_MOTOR_LEFT_B 10
#define PIN_MOTOR_RIGHT_A 11
#define PIN_MOTOR_RIGHT_B 12

// These pins control the speed of the motors
#define PIN_MOTOR_LEFT_SPEED A2
#define PIN_MOTOR_RIGHT_SPEED A3

// Values controlling the car's speed and turning
// Max speed: If the car is going max speed, what value should we write? 0..1024
// Turn ratio: if the car is turning left at max speed, how fast should the left wheel spin compared to the right one?
#define CAR_MAX_SPEED 1024
#define CAR_TURN_RATIO 0.25

void setup() {
  pinMode(PIN_MOTOR_LEFT_A, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_B, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_A, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_B, OUTPUT);

  digitalWrite(PIN_MOTOR_LEFT_A, LOW);
  digitalWrite(PIN_MOTOR_LEFT_B, HIGH);
  digitalWrite(PIN_MOTOR_RIGHT_A, LOW);
  digitalWrite(PIN_MOTOR_RIGHT_B, HIGH);
}

void loop() {
  // Map to 0..CAR_MAX_SPEED
  float speed = analogRead(PIN_SPEED) / 1024.0 * CAR_MAX_SPEED;
  // Map to -1..1
  float direction = analogRead(PIN_STEER) / 512.0 - 1.0;
  // Map to CAR_TURN_RATIO..1
  float steer = (1.0 - abs(direction) * (1.0 - CAR_TURN_RATIO));
  if (direction < 0) {
    analogWrite(PIN_MOTOR_LEFT_SPEED, speed * steer);
    analogWrite(PIN_MOTOR_RIGHT_SPEED, speed);  
  } else {
    analogWrite(PIN_MOTOR_LEFT_SPEED, speed);
    analogWrite(PIN_MOTOR_RIGHT_SPEED, speed * steer);  
  }
}

