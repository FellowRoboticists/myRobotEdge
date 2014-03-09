// myRobotEdge Arduino sketch
//
// Copyright (c) 2012 Michael Margolis
// Copyright (c) 2013,2014 Dave Sieh
//
// See LICENSE.txt for details.

#ifndef _IR_SENSORS_H_
#define _IR_SENSORS_H_

#define NBR_SENSORS 2
#define SENSE_IR_LEFT 0
#define SENSE_IR_RIGHT 1

class IRSensors {
 public:

  IRSensors(byte leftPin, byte rightPin);

  void begin();
  boolean edgeDetect(int sensor);

 private:
  void calibrate(byte sensor);

  byte IR_SENSORS[NBR_SENSORS] = { -1, -1 };
  int irSensorAmbient[NBR_SENSORS]; // sensor value with no reflection
  int irSensorReflect[NBR_SENSORS]; // value considered detecting an object
  int irSensorEdge[NBR_SENSORS];    // value considered detecting an edge
  boolean isDetected[NBR_SENSORS] = { false, false }; // True if detected

  const int irReflectThreshold = 10; // % level below ambient to trigger reflection
  const int irEdgeThreshold    = 5; // % level above ambient to trigger edge

};

#endif
