
#include <Servo.h>
#include <EEPROM.h>


int throttlePin = 10;
int servoPin = 9;

// Larger Update -> Slower Reaction -> Less Sudden/Jerky Movements
int throttleUpdate = 1;
int servoUpdate = 1;
// Larger Delta -> Bigger Reaction -> More Sudden/Jerky Movements
int throttleDelta = 1;
int servoDelta = 1;
unsigned long time;
unsigned long oldTime;

int throttleValue = 0;
int throttleGoal = 100;

int servoAngle = 90;
int servoGoal = 90;

Servo steeringServo;
Servo throttle;


void setup()
{
  // Attach the Servo objects to their respective pins and arm the Traxxas ESC
  steeringServo.attach(servoPin);
  throttle.attach(throttlePin);
  executeArmingSequence();

  // Retrieve last stored value of servoAngle
  //servoAngle = loadServoAngle();

  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //throttleGoal = 100;
}

void loop()
{
  time = millis();

  if(oldTime != time)
  {
    // This code operates under the assumption that
    // this main loop function will take less than
    // 1 millisecond to execute.
    //if(time%throttleUpdate == 0)
    //{
      updateThrottle();
    //}

    //throttle.write(throttleValue);

    //if(time%servoUpdate == 0)
    //{
      updateServo();
      //Serial.println(servoAngle);
    //}
  }

  if (Serial.available() > 1)
  {
    // Instruction Format:
    // Byte 1 - Instruction
    // Byte 2 - Argument
    char instruction = Serial.read();
    unsigned int argument = Serial.read();

    if(instruction == 'T')
    {
      throttleGoal = argument;
      //throttle.write(throttleValue);
    } else if(instruction == 'S')
    {
      servoGoal = argument;
    }
  }
  oldTime = time;
  //throttle.write(throttleValue);
  // Serial Stability delay
  //delayMicroseconds(100);
}

void executeArmingSequence()
{
    // This loop arms the ESC
    for (throttleValue = 90; throttleValue < 100; throttleValue += 1)
    {
      throttle.write(throttleValue);
      delay(150);
    }
}

void updateThrottle()
{
  // Find the direction and strength of the update
  int error = throttleGoal - throttleValue;
  int sign = abs(error) / error;

  if(error == 0)
    return;

  // Choose the smaller value, throttleDelta or error
  if(abs(error) > throttleDelta)
  {
    throttleValue += throttleDelta*sign;
  } else
  {
    throttleValue = throttleGoal;
  }

  //Serial.println(error);
  //Serial.println(throttleGoal);
  //Serial.println(throttleValue);
  throttle.write(throttleValue);
}

void updateServo()
{
  // Find the direction and strength of the update
  int error = servoGoal - servoAngle;
  int sign = abs(error) / error;

  if(error == 0)
    return;

  // Choose the smaller value, servoDelta or error
  if(abs(error) > servoDelta)
  {
    servoAngle += servoDelta*sign;
  } else {
    servoAngle = servoGoal;
  }
  steeringServo.write(servoAngle);

  // Backup the value everytime it's updated
  //storeServoAngle();
}

int loadServoAngle()
{
  // Retrieve the previously stored value for the servoAngle
  // This ensures that the servo isn't jerked back to the
  // start position upon resetting the Arduino
  // This data is stored in the first <sizeof(int)> bytes of EEPROM
  int val;
  for(int address = 0; address < sizeof(int); address++)
    val = (val + EEPROM.read(address)) << 8;

  return val;
}

void storeServoAngle()
{
  // Store the servoAngle in EEPROM for retrieval
  // upon reset of the Arduino
  // This data is stored in the first <sizeof(int)> bytes of EEPROM
  for(int address = 0; address < sizeof(int); address++)
  {
    byte val = (servoAngle >> (8*(sizeof(int) - address - 1))) & 255;
    EEPROM.write(address, val);
  }
}
