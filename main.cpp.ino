
// Benjamin S. Bussell
// Based off of Areeb Mohammed's Code Base

#include <Servo.h>
#include <EEPROM.h>

Servo Steering;
Servo Acceleration;

int pos = 0;    // variable to store the servo position

int accelerationPin = 10;
int steeringPin = 9;

// Larger the Number the smoother the movement.
int accelerationUpdate = 1; // Unused For Now
int steeringUpdate = 1; // Unused For Now

// Bigger Number results in Jerkier Movement.
int accelerationDelta = 1;
int steeringDelta = 1;

unsigned long time;
unsigned long prevTime;

int accelerationValue = 0;
int accelerationGoal = 100;

int steeringAngle = 90;
int steeringGoal = 90;



void setup() {
  Steering.attach(steeringPin);
  Acceleration.attach(accelerationPin);

  executeArmingSequence();

  Serial.begin(9600);
  while(!Serial) {;}
   
}

void loop() {
  time = millis();

  if ( prevTime != time ) {
    
  }
}

void executeArmingSequence() {
    // This loop arms the ESC
    for (accelerationValue = 90; accelerationValue < 100; accelerationValue += 1) {
      Acceleration.write(accelerationValue);
      delay(150);
    }
    Acceleration.write(0);
}

void updateAcceleration() {

  // Manual Error Detection
  int error = accelerationGoal - accelerationValue;
  int sign = abs(error) / error;

  if(error==0) {return;}
  
  if (abs(error) > steeringDelta) 
    steeringAngle += steeringDelta*sign;
  else 
    steeringAngle = steeringGoal;

  Steering.write(steeringAngle);
  
}
