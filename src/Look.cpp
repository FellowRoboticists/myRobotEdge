#include <Arduino.h>
#include "Look.h"
#include "IRSensors.h"

Look::Look(IRSensors *sensors) {
  irSensors = sensors;
}

void Look::begin() {
  irSensors->begin();
}

// returns true if the given obstacle is detected
boolean Look::lookForObstacle(int obstacle) {
  switch (obstacle) {
  case OBST_FRONT_EDGE:
    return irSensors->edgeDetect(SENSE_IR_LEFT) && irSensors->edgeDetect(SENSE_IR_RIGHT);
  case OBST_LEFT_EDGE:
    return irSensors->edgeDetect(SENSE_IR_LEFT);
  case OBST_RIGHT_EDGE:
    return irSensors->edgeDetect(SENSE_IR_RIGHT);
  }
  return false;
}

boolean Look::checkMovement() {
  return true;
}
