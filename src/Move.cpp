#include <Arduino.h>
#include <RobotMotor.h>
#include "Move.h"
#include "Look.h"

const char *states[] = { "MoveLeft", "MoveRight", "MoveForward", "MoveBack", "MoveRotate", "MoveStop" };

const char *locationString[] = { "Left", "Right", "Center" };

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
  Serial.print(" Slower: ");
  if (moveSpeed >= speedIncrement + MIN_SPEED) {
    moveSpeed -= speedIncrement;
  } else {
    moveSpeed = MIN_SPEED;
  }
  setSpeed(moveSpeed);
}

void Move::faster() {
  Serial.print(" Faster: ");
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
    Serial.print("Changing move state from ");
    Serial.print(states[moveState]);
    Serial.print(" to ");
    Serial.println(states[newState]);
    moveState = newState;
  }
}

void Move::rotate(int angle) {
  Serial.print("Rotating "); 
  Serial.println(angle);
  if (angle < 0) {
    Serial.println(" (left)");
    motorReverse(MOTOR_LEFT, moveSpeed);
    motorForward(MOTOR_RIGHT, moveSpeed);
    angle = -angle; 
    changeMoveState(MOV_ROTATE);
  } else if (angle > 0) {
    Serial.println(" (right)");
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
    Serial.print("index = ");
    Serial.print(index);
    Serial.print(", t0 = ");
    Serial.print(t0);
    Serial.print(", t1 = ");
    Serial.print(t1);
  }
  Serial.print(" full rotation time = ");
  Serial.println(fullRotationTime);
  long result = map(angle, 0, 360, 0, fullRotationTime);
  return result;
}

void Move::calibrateRotationRate(int direction, int angle) {
  Serial.print(locationString[direction]);
  Serial.println(" calibration");
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
      Serial.println("Invalid direction");
    }
    
    int time = rotationAngleToTime(angle, speed);
    
    Serial.print(locationString[direction]);
    Serial.print(": rotate ");
    Serial.print(angle);
    Serial.print(" degrees at speed ");
    Serial.print(speed);
    Serial.print(" for ");
    Serial.print(time);
    Serial.print("ms");
    delay(time);
    motorStop(MOTOR_LEFT);
    motorStop(MOTOR_RIGHT);
    delay(2000); // Two second delay between speeds
  }
}

void Move::timedMove(int direction, int duration) {
  Serial.print("Timed move ");
  if (direction == MOV_FORWARD) {
    Serial.println("forward");
    forward();
  } else if (direction == MOV_BACK) {
    Serial.println("back");
    backward();
  } else {
    Serial.println("?");
  }
  movingDelay(duration);
  stop();
}

void Move::movingDelay(long duration) {
  long startTime = millis();
  while (millis() - startTime < duration) {
    // function in Look module checks for obstacle in direction of movement
    if (looker->checkMovement() == false) {
      if (moveState != MOV_ROTATE) {
        // rotate is only valid movement
        Serial.println("Stopping in moving Delay()");
        brake();
      }
    }
  }
}
