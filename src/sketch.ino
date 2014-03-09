// -*- c -*-
// myRobotEdge Arduino sketch
//
// Copyright (c) 2012 Michael Margolis
// Copyright (c) 2013,2014 Dave Sieh
//
// See LICENSE.txt for details.

#include <LEDBlinker.h>
#include <SoftServo.h>
#include <RobotMotor.h>
#include <pspc_support.h>
#include "IRSensors.h"
#include "Look.h"
#include "Move.h"

// Analog Pins
#define LEFT_IR_SENSOR_PIN 0
#define RIGHT_IR_SENSOR_PIN 1

// Digital Pins
#define SWEEP_SERVO_PIN 9
#define LED_PIN 13

LEDBlinker blinker(LED_PIN);
SoftServo sweepServo(SWEEP_SERVO_PIN);
IRSensors irSensors(LEFT_IR_SENSOR_PIN, RIGHT_IR_SENSOR_PIN);
Look looker(&irSensors);
Move mover(&looker);

void setup() {
  Serial.begin(9600);
  blinker.begin();
  blinker.blinkNumber(8); // For Leonardo
  sweepServo.begin();

  mover.begin();

  // Set the initial speed of the robot - 10% above minimum speed
  mover.setSpeed(MIN_SPEED + 10);

  // Center the servo
  sweepServo.write(90, 2000);

  Serial.println(P("Ready"));
}

void loop() {
  // Code for roaming around and avoiding obstacles
  if (looker.lookForObstacle(OBST_FRONT_EDGE)) {

#ifdef ROBOTEDGE_DEBUG
    Serial.println(P("both sensors detected edge"));
#endif
    mover.timedMove(MOV_BACK, 500);
    mover.rotate(120);

  } else if (looker.lookForObstacle(OBST_LEFT_EDGE)) {

#ifdef ROBOTEDGE_DEBUG
    Serial.println(P("left sensor detected edge"));
#endif
    mover.timedMove(MOV_BACK, 500);
    mover.rotate(60);

  } else if (looker.lookForObstacle(OBST_RIGHT_EDGE)) {

#ifdef ROBOTEDGE_DEBUG
    Serial.println(P("right sensor detected edge"));
#endif
    mover.timedMove(MOV_BACK, 500);
    mover.rotate(-60);

  } else {

    mover.setSpeed(MIN_SPEED);
    mover.forward();

  }
}
