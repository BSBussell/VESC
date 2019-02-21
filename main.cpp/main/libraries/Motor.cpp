
// Benjamin S. Bussell

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

void Motor::startup() {
     serv.attach(pin);
}

void Motor::executeArmingSequence() {
    // This loop arms the ESC
    for (value = 90; value < 100; value += 1) {
      serv.write(value);
      delay(150);
    }
    serv.write(0);
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
