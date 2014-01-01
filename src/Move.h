// myRobotEdge Arduino sketch
//
// Copyright (c) 2012 Michael Margolis
// Copyright (c) 2013 Dave Sieh
//
// See LICENSE.txt for details.

#ifndef _MOVE_H_
#define _MOVE_H_

// Defines for directions
#define DIR_LEFT   0
#define DIR_RIGHT  1
#define DIR_CENTER 2

enum MOTIONS {
  MOV_LEFT, MOV_RIGHT, MOV_FORWARD, MOV_BACK, MOV_ROTATE, MOV_STOP
};

class Look;

class Move {
 public:
  Move(Look *look);

  void begin();
  void left();
  void right();
  void stop();
  void brake();
  void backward();
  void forward();
  void setSpeed(int speed);
  void slower();
  void faster();
  int getState();
  void rotate(int angle);
  void timedMove(int direction, int duration);

 private:
  int changeMoveState(int newState);
  long rotationAngleToTime(int angle, int speed);
  void calibrateRotationRate(int direction, int angle);
  void movingDelay(long duration);

  Look *looker;

  int moveState = MOV_STOP;  // what the robot is doing
  
  int moveSpeed = 0; // move speed stored here (0-100%)
  int speedIncrement = 10; // percent to increase or decrease speed

};

#endif
