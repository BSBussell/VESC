

#ifndef Servo_h
#define Servo_h

#include <Servo.h>
#include <EEPROM.h>

#include "Arduino.h"

class Motor {

private:

  Servo serv;
  int pin;
  int throttle;
  int delta;
  int value;
  int goal;

public:

    Motor(int Pin, int Throttle, int Delta, int Value, int Goal) :
        pin (Pin), throttle (Throttle), delta (Delta), value (Value), goal (Goal)
    { }

    void refresh();
    void startup();
    void executeArmingSequence();

    int loadServoValue();
    void storeServoValue();

};



#endif
