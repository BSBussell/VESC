// Benjamin S. Bussell
// February 26, 2019

#ifndef Motor_h
#define Motor_h

#include "Arduino.h"
#include <Servo.h>
#include <EEPROM.h>



class Motor {

private:

  Servo serv;
  int pin;
  int throttle;
  double delta;
  double value;
  double goal;

public:

    Motor(int Pin, int Throttle, int Delta, int Value, int Goal):
        pin (Pin), throttle (Throttle), delta (double(Delta)), value (double(Value)), goal (double(Goal)){};
    ~Motor();
    void refresh();
    void startup();
    void executeArmingSequence();

    void setGoal(unsigned int argument);
    void setDelta(unsigned int value);

    int loadServoValue();
    void storeServoValue();

};



#endif
