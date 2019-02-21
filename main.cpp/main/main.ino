
// Benjamin S. Bussell
// Based off of Areeb Mohammed's Code Base

#include "Arduino.h"
#include <Servo.h>
#include <EEPROM.h>
#include <Motor.h>


// Motor object(pin, throttle,delta, value, goal)
Motor Steering(9,1,1,90,90);
Motor Acceleration(10,1,1,0,10);

unsigned long time;
unsigned long prevTime;

void setup() {
  Steering.startup();
  Acceleration.startup();

  Acceleration.executeArmingSequence();

  Serial.begin(9600);
  while(!Serial) {;}

}

void loop() {
  time = millis();

  if ( prevTime != time ) {
    Acceleration.refresh();
    Steering.refresh();

  }

  prevTime = time;
}
