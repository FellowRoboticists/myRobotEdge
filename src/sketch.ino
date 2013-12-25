// -*- c -*-
#include <LEDBlinker.h>
#include <SoftServo.h>
#include <RobotMotor.h>
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

  Serial.println("Ready");
}

void loop() {
  // Code for roaming around and avoiding obstacles
  if (looker.lookForObstacle(OBST_FRONT_EDGE)) {
    Serial.println("both sensors detected edge");
    mover.timedMove(MOV_BACK, 500);
    mover.rotate(120);
    while (looker.lookForObstacle(OBST_FRONT_EDGE)) {
      mover.stop(); // Stop motors if still over cliff
    }
  } else if (looker.lookForObstacle(OBST_LEFT_EDGE)) {
    Serial.println("left sensor detected edge");
    mover.timedMove(MOV_BACK, 500);
    mover.rotate(60);
  } else if (looker.lookForObstacle(OBST_RIGHT_EDGE)) {
    Serial.println("right sensor detected edge");
    mover.timedMove(MOV_BACK, 500);
    mover.rotate(-60);
  } else {
    mover.setSpeed(MIN_SPEED);
    mover.forward();
  }
}
