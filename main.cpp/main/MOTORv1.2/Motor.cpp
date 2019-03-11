
// Benjamin S. Bussell
// February 26, 2019

#include "Arduino.h"
#include "Motor.h"
#include <Servo.h>

Motor::~Motor () {

}

void Motor::refresh() {
    // Manual Error Detection
    int error = goal - value;
    int sign = abs(error) / error;

    if (error == 0 ) { return;}

    if (abs(error) > delta)
      value += delta*sign;
    else
      value = goal;

    serv.write(value);

}

void Motor::startup(bool accel = false) {
    // Special Conditions if the Motor is for Acceleration
    if (accel) {
      // Gives bigger margin for acceleratior
      serv.attach(pin, 200, 2000);
    } else {
      serv.attatch(pin);
    }
}

void Motor::executeArmingSequence() {
    // This loop arms the ESC
    for (value = 90; value < 100; value += 1) {
      serv.write(value);
      delay(150);
    }
    //serv.write(0);
}

void Motor::setGoal( unsigned int argument) {
  // TODO: Implement Error Checking to prevent from inappropriate Arguments
  //       Ex: if argument is out of range of motors or something dumb.
  goal = argument;
}

void Motor::setDelta( unsigned int value) {

  delta = value;
  //Serial.print(delta);
}

int Motor::loadServoValue() {
    int val;
    for ( int address = 0; address < sizeof(int); address++)
      val = (val + EEPROM.read(address)) << 8;

    return val;
}

void Motor::storeServoValue() {
  for(int address = 0; address < sizeof(int); address++) {
    byte val = (value >> (8*(sizeof(int) - address - 1))) & 255;
    EEPROM.write(address, val);
  }
}
