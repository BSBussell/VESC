

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
// If modified needs to be recompiled from Sketch > Include Library > Add .ZIP Library
// Might need to rename the folder, just add one version number to the name.

#include <SPI.h> // SparkFun IMU
#include <Wire.h> // SparkFun IMU
#include <SparkFunLSM9DS1.h> // SparkFun IMU

// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M   0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

// Red wire goes to GND
// Yellow Wire goes to 3.3v
// Green Goes to A4
// Blue goes to A5

// IMU Constructor
LSM9DS1 imu;


// Motor object(pin, throttle,delta, value, goal)
Motor Steering(9, 1, 1, 90, 90);
Motor Acceleration(10, 1, 1, 0, 10);



bool imuSuccess = true;

// For keeping track of updates
unsigned long time;
unsigned long prevTime;

// if you're new to arduino this code runs before the loop.
// Any setup needs to happen here
// I initalized variable above so that they are included in the Global scope
void setup() {

  // Configure Settings for how IMU is wired.
  imu.settings.device.commInterface = IMU_MODE_I2C; // Set mode to I2C
  imu.settings.device.mAddress = LSM9DS1_M; // Set mag address to 0x1E
  imu.settings.device.agAddress = LSM9DS1_AG; // Set ag address to 0x6B
  
  // Initalize the IMU
  if (!imu.begin()) {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Returned Values will now All be 0");
    imuSuccess = false;
  } else {
    imu.settings.accel.scale = 16; // Set accel range to +/-16g
    imu.settings.gyro.scale = 2000; // Set gyro range to +/-2000dps
    imu.settings.mag.scale = 8; // Set mag range to +/-8Gs
    imu.begin(); // Call begin to update the sensor's new settings

  }

  
  // Initalize the motors.
  Steering.startup();
  Acceleration.startup();

  // For some reason needed to make sure the Acceleration motor is working
  Acceleration.executeArmingSequence();

  // Initalize ports and make sure they are working, code will not run until they do
  Serial.begin(9600);
  
  while (!Serial) {
    ;
  }

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
    prevTime = time;
  }

  // Check for inputs from python.
  if (Serial.available() > 2) {

    // Byte 1 - Instruction
    // Byte 2 - High Value
    // Byte 3 - Low Value
    // Read the IO Port
    
    char instruction = Serial.read();
    unsigned int argument = parseBytes(); // Parse Bytes reads the two bytes sent and merges them to a 16 bit int
    
    // 3/6/2019 - Added lower case for changing Delta with the Arduino
    
    switch(instruction) {
      case 'T':
        Acceleration.setGoal(argument);
      
      case 't': 
        Acceleration.setDelta(argument);
        break;
      case 'S':
        Steering.setGoal(argument);
        break;
      case 's':
        Steering.setDelta(argument);
         break;
      case 'R':
        Serial.println(Acceleration.readPhysical());
        break;
      case 'r':
        Serial.println(Steering.readPhysical());
        break;
      case 'G':
        imu.readGyro();
        Serial.println(imu.calcGyro(imu.gx));
        Serial.println(imu.calcGyro(imu.gy));
        Serial.println(imu.calcGyro(imu.gz));
        break;
      case 'A':
        imu.readAccel();
        Serial.println(imu.calcAccel(imu.ax));
        Serial.println(imu.calcAccel(imu.ay));
        Serial.println(imu.calcAccel(imu.az));
        break;
      case 'M':
        imu.readMag();
        Serial.println(imu.calcMag(imu.mx));
        Serial.println(imu.calcMag(imu.my));
        Serial.println(imu.calcMag(imu.mz));
        break;
      case 'C':
        imu.calibrate(true);
        break;
    }

  }
  
  // Set finish time for use above.
  
}

int parseBytes() {
  
  byte high = Serial.read();
  byte low  = Serial.read();
  
  return high * 256 + low;
}
