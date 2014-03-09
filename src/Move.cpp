// myRobotEdge Arduino sketch
//
// Copyright (c) 2012 Michael Margolis
// Copyright (c) 2013,2014 Dave Sieh
//
// See LICENSE.txt for details.

#include <Arduino.h>
#include <RobotMotor.h>
#include <pspc_support.h>
#include "Move.h"
#include "Look.h"

#ifdef MOVE_DEBUG
#define STATE_NAME(i) STRING_FROM_TABLE(stateNames,i)

const char stateMoveLeft[] PROGMEM = "MoveLeft";
const char stateMoveRight[] PROGMEM = "MoveRight";
const char stateMoveForward[] PROGMEM = "MoveForward";
const char stateMoveBack[] PROGMEM = "MoveBack";
const char stateMoveRotate[] PROGMEM = "MoveRotate";
const char stateMoveStop[] PROGMEM = "MoveStop";

PGM_P const stateNames[] PROGMEM = {
  stateMoveLeft,
  stateMoveRight,
  stateMoveForward,
  stateMoveBack,
  stateMoveRotate,
  stateMoveStop 
};

#define LOCATION_NAME(i) STRING_FROM_TABLE(locationNames,i)

const char locLeft[] PROGMEM = "Left";
const char locRight[] PROGMEM = "Right";
const char locCenter[] PROGMEM = "Center";

PGM_P const locationNames[] PROGMEM = {
  locLeft,
  locRight,
  locCenter
};

#endif

Move::Move(Look *look) {
  looker = look;
}

void Move::begin() {
  looker->begin();
  motorBegin(MOTOR_LEFT);
  motorBegin(MOTOR_RIGHT);
  stop();
}

void Move::left() {
  motorForward(MOTOR_LEFT, 0);
  motorForward(MOTOR_RIGHT, moveSpeed);
  changeMoveState(MOV_LEFT);
}

void Move::right() {
  motorForward(MOTOR_LEFT, moveSpeed);
  motorForward(MOTOR_RIGHT, 0);
  changeMoveState(MOV_RIGHT);
}

void Move::stop() {
  motorStop(MOTOR_LEFT);
  motorStop(MOTOR_RIGHT);
  changeMoveState(MOV_STOP);
}

void Move::brake() {
  motorBrake(MOTOR_LEFT);
  motorBrake(MOTOR_RIGHT);
  changeMoveState(MOV_STOP);
}

void Move::backward() {
  motorReverse(MOTOR_LEFT, moveSpeed);
  motorReverse(MOTOR_RIGHT, moveSpeed);
  changeMoveState(MOV_BACK);
}

void Move::forward() {
  motorForward(MOTOR_LEFT, moveSpeed);
  motorForward(MOTOR_RIGHT, moveSpeed);
  changeMoveState(MOV_FORWARD);
}

void Move::setSpeed(int speed) {
  motorSetSpeed(MOTOR_LEFT, speed);
  motorSetSpeed(MOTOR_RIGHT, speed);
  moveSpeed = speed;
}

void Move::slower() {
#ifdef MOVE_DEBUG
  Serial.print(P(" Slower: "));
#endif
  if (moveSpeed >= speedIncrement + MIN_SPEED) {
    moveSpeed -= speedIncrement;
  } else {
    moveSpeed = MIN_SPEED;
  }
  setSpeed(moveSpeed);
}

void Move::faster() {
#ifdef MOVE_DEBUG
  Serial.print(P(" Faster: "));
#endif
  moveSpeed += speedIncrement;
  if (moveSpeed > 100) {
    moveSpeed = 100;
  }
  setSpeed(moveSpeed);
}

int Move::getState() {
  return moveState;
}

int Move::changeMoveState(int newState) {
  if (newState != moveState) {
#ifdef MOVE_DEBUG
    Serial.print(P("Changing move state from "));
    Serial.print(STATE_NAME(moveState));
    Serial.print(P(" to "));
    Serial.println(STATE_NAME(newState));
#endif
    moveState = newState;
  }
}

void Move::rotate(int angle) {
#ifdef MOVE_DEBUG
  Serial.print(P("Rotating ")); 
  Serial.println(angle);
#endif
  if (angle < 0) {

#ifdef MOVE_DEBUG
    Serial.println(P(" (left)"));
#endif
    motorReverse(MOTOR_LEFT, moveSpeed);
    motorForward(MOTOR_RIGHT, moveSpeed);
    angle = -angle; 
    changeMoveState(MOV_ROTATE);

  } else if (angle > 0) {

#ifdef MOVE_DEBUG
    Serial.println(P(" (right)"));
#endif
    motorForward(MOTOR_LEFT, moveSpeed);
    motorReverse(MOTOR_RIGHT, moveSpeed);
    changeMoveState(MOV_ROTATE);

  }

  int ms = rotationAngleToTime(angle, moveSpeed);
  movingDelay(ms);
  brake();
}

long Move::rotationAngleToTime(int angle, int speed) {
  int fullRotationTime; // Time to rotate 360 degrees at given speed
  
  if (speed < MIN_SPEED) {
    return 0; // Ignore speeds slower than the first table entry
  }
  
  angle = abs(angle);
  
  if (speed >= 100) {
    fullRotationTime = rotationTime[NBR_SPEEDS - 1];
  } else {
    int index = (speed - MIN_SPEED) / SPEED_TABLE_INTERVAL;
    int t0 = rotationTime[index];
    int t1 = rotationTime[index + 1];
    fullRotationTime = map(speed, speedTable[index], speedTable[index + 1], t0, t1);
#ifdef MOVE_DEBUG
    Serial.print(P("index = "));
    Serial.print(index);
    Serial.print(P(", t0 = "));
    Serial.print(t0);
    Serial.print(P(", t1 = "));
    Serial.print(t1);
#endif
  }
#ifdef MOVE_DEBUG
  Serial.print(P(" full rotation time = "));
  Serial.println(fullRotationTime);
#endif
  long result = map(angle, 0, 360, 0, fullRotationTime);
  return result;
}

void Move::calibrateRotationRate(int direction, int angle) {
#ifdef MOVE_DEBUG
  Serial.print(LOCATION_NAME(direction));
  Serial.println(P(" calibration"));
#endif
  for (int speed = MIN_SPEED; speed <= 100; speed += SPEED_TABLE_INTERVAL) {
    delay(1000);
    
    if (direction == DIR_LEFT) {
      // Rotate left
      motorReverse(MOTOR_LEFT, speed);
      motorForward(MOTOR_RIGHT, speed);
    } else if (direction == DIR_RIGHT) {
      // Rotate right
      motorForward(MOTOR_LEFT, speed);
      motorReverse(MOTOR_RIGHT, speed);
    } else {
#ifdef MOVE_DEBUG
      Serial.println(P("Invalid direction"));
#endif
    }
    
    int time = rotationAngleToTime(angle, speed);
    
#ifdef MOVE_DEBUG
    Serial.print(LOCATION_NAME(direction));
    Serial.print(P(": rotate "));
    Serial.print(angle);
    Serial.print(P(" degrees at speed "));
    Serial.print(speed);
    Serial.print(P(" for "));
    Serial.print(time);
    Serial.print(P("ms"));
#endif
    delay(time);
    motorStop(MOTOR_LEFT);
    motorStop(MOTOR_RIGHT);
    delay(2000); // Two second delay between speeds
  }
}

void Move::timedMove(int direction, long duration) {
#ifdef MOVE_DEBUG
  Serial.print(P("Timed move "));
#endif
  if (direction == MOV_FORWARD) {
#ifdef MOVE_DEBUG
    Serial.println(P("forward"));
#endif
    forward();
  } else if (direction == MOV_BACK) {
#ifdef MOVE_DEBUG
    Serial.println(P("back"));
#endif
    backward();
  } else {
#ifdef MOVE_DEBUG
    Serial.println(P("?"));
#endif
  }
  movingDelay(duration);
  stop();
}

void Move::movingDelay(long duration) {
  long startTime = millis();
  long elapsedTime = 0;
  
  while (elapsedTime < duration) {
    // function in Look module checks for obstacle in direction of movement 
    if (looker->checkMovement() == false) { 
      if (moveState != MOV_ROTATE) {
        // rotate is only valid movement
#ifdef MOVE_DEBUG
        Serial.println(P("Stopping in moving Delay()")); 
#endif
        brake(); 
      }
    }  
    elapsedTime = millis() - startTime;
  }  
}
