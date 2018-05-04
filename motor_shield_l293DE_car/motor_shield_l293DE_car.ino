
//OUTPUTS RIGHT AND LEFT MOTORS
#define DIRECTION_PORT_R 13 //DIGITAL PORT
#define MOTORSPEED_PORT_R 11  //PWM PORT
#define BRAKE_PORT_R 7

#define DIRECTION_PORT_L 12
#define MOTORSPEED_PORT_L 10 //PWM PORT
#define BRAKE_PORT_L 4      //DIGITAL PORT


#define PWM_MAX 255
#define PWM_MIN 0

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);

  pinMode(DIRECTION_PORT_R, OUTPUT);
  pinMode(MOTORSPEED_PORT_R, OUTPUT);
  pinMode(BRAKE_PORT_R, OUTPUT);

  pinMode(DIRECTION_PORT_L, OUTPUT);
  pinMode(MOTORSPEED_PORT_L, OUTPUT);
  pinMode(BRAKE_PORT_L, OUTPUT);

  Serial.println("Car setup finished!");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Forward");
  setMotorRight(255, true);
  setMotorLeft(255, true);
  

  delay(2000);
  Serial.println("Backwards");
  setMotorRight(255, false);
  setMotorLeft(255, false);
  delay(2000);

  stop();
  delay(3000);
  }

void setMotorRight(int speed, boolean forward){
  digitalWrite(DIRECTION_PORT_R, forward); // Define the spin direction
  digitalWrite(BRAKE_PORT_R, !forward); // Don't brake 
  analogWrite(MOTORSPEED_PORT_R, speed);
  }

void setMotorLeft(int speed, boolean forward){
  digitalWrite(DIRECTION_PORT_L, forward); // Define the spin direction
  digitalWrite(BRAKE_PORT_L, !forward); // Don't brake 
  analogWrite(MOTORSPEED_PORT_L, speed);
  }

void stop(){
  digitalWrite(DIRECTION_PORT_R, false); // Define the spin direction
  digitalWrite(BRAKE_PORT_R, true); // brake 
  analogWrite(MOTORSPEED_PORT_R, 0);

  digitalWrite(DIRECTION_PORT_L, false); // Define the spin direction
  digitalWrite(BRAKE_PORT_L, true); // brake 
  analogWrite(MOTORSPEED_PORT_L, 0);
  }

