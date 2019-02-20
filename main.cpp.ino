
// Benjamin S. Bussell
// Based off of Areeb Mohammed's Code Base

#include <Servo.h>
#include <EEPROM.h>


class Motor {

  public:
    
    Servo serv;
    int pin;
    int throttle;
    int delta;
    int value;
    int goal;
    
    Motor(int Pin, int Throttle, int Delta, int Value, int Goal){
        pin = Pin;
        throttle = Throttle;
        delta = Delta;
        value = Value;
        goal = Goal;
    }
    
    void refresh() {
      // Manual Error Detection
      int error = goal - value;
      int sign = abs(error) / error;

      if(error==0) {return;}
  
      if (abs(error) > delta) 
        value += delta*sign;
      else 
        value = goal;

      serv.write(value);
  
    }
    
    void startup() {
      serv.attach(pin);
    }
    
    void executeArmingSequence() {
      // This loop arms the ESC
      for (value = 90; value < 100; value += 1) {
        serv.write(value);
        delay(150);
      }
      serv.write(0);
    }

    int loadServoValue() {

      int val;
      for ( int address = 0; address < sizeof(int); address++)
        val = (val + EEPROM.read(address)) << 8;

      return val;
    }

    void storeServoValue() {
      for(int address = 0; address < sizeof(int); address++) {
        byte val = (servoAngle >> (8*(sizeof(int) - address - 1))) & 255;
        EEPROM.write(address, val);
      }
    }
};

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
