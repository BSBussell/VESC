
// Benjamin S. Bussell
// February 26, 2019

// If having trouble with the code submit an issue here:
// https://github.com/BSBussell/VESC


#include "Arduino.h"
#include <Servo.h>
#include <EEPROM.h>

#include <Motor.h>
// Custom Library made to abstract the code helping readability.
// Can be found in Motor folder
// If modified needs to be recompiled from Sketch>Include Library>Add .ZIP Library
// Might need to rename the folder, just add one version number to the name.



// Motor object(pin, throttle,delta, value, goal)
Motor Steering(9,1,1,90,90);
Motor Acceleration(10,1,1,0,10);

// For keeping track of updates
unsigned long time;
unsigned long prevTime;

// if you're new to arduino this code runs before the loop.
// Any setup needs to happen here
// I initalized variable above so that they are included in the Global scope
void setup() {

  // Initalize the motors.
  Steering.startup();
  Acceleration.startup(true);

  // For some reason needed to make sure the Acceleration motor is working
  Acceleration.executeArmingSequence();

  // Initalize ports and make sure they are working, code will not run until they do
  Serial.begin(9600);
  while(!Serial) {;}

}

// This loop called every 'frame'
// Main action
void loop() {

  // set the time
  time = millis();

  // Make sure time has passed since last loop, to prevent weird stacking issues
  if ( prevTime != time ) {
    
    // Syncs the physical motor with the variables
    Acceleration.refresh();
    Steering.refresh();

  }

  // Check for inputs from pyton.
  if (Serial.available() > 1) {

    // Byte 1 - Instruction
    // Byte 2 - Argument
    // Read the IO Port
    char instruction = Serial.read();
    unsigned int argument = Serial.read();

    // Instruction naming scheme uses the same as the other group to retain
    // Compability between the two Arduino's.
    // 3/6/2019 - Added lower case for changing Delta with the Arduino
    
    
    if (instruction == 'T') {
      
      Acceleration.setGoal(argument);
    } 
    else if (instruction == 't') {
      
      Acceleration.setDelta(argument);
    }
    else if (instruction == 'S') {
  
      Steering.setGoal(argument);
    } 
    else if (instruction == 's') {

      Steering.setDelta(argument);
    }

  }

  // Set finish time for use above.
  prevTime = time;
}
