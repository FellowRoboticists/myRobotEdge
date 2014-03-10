// myRobotEdge Arduino sketch
//
// Copyright (c) 2012 Michael Margolis
// Copyright (c) 2013,2014 Dave Sieh
//
// See LICENSE.txt for details.

#include <Arduino.h>
#include <pspc_support.h>
#include "IRSensors.h"

#ifdef IRSENSORS_DEBUG
#define LOCATION_NAME(i) STRING_FROM_TABLE(locationNames,i)

const char loc_left[] PROGMEM = "Left";
const char loc_right[] PROGMEM = "Right";

PGM_P const locationNames[] PROGMEM {
  loc_left,
  loc_right
};

#endif

IRSensors::IRSensors(byte leftPin, byte rightPin) {
  IR_SENSORS[0] = leftPin;
  IR_SENSORS[1] = rightPin;
}

void IRSensors::begin() {
  for (int sensor = 0; sensor < NBR_SENSORS; sensor++) {
    calibrate(sensor);
  }
}

 // Calibrate thresholds for ambient light
void IRSensors::calibrate(byte sensor) {
  int ambient = averageValue(sensor); // Get ambient level

  irSensorAmbient[sensor] = ambient;
   
  // Precalculate the levels for object and edge detection
  irSensorReflect[sensor] = (ambient * (long)(100 - irReflectThreshold)) / 100;
  irSensorEdge[sensor]    = (ambient * (long)(100 + irEdgeThreshold)) / 100;
}

int IRSensors::averageValue(int sensor, byte numReadings) {
  int cumulativeValue = 0;
  for (byte reading = 0; reading < numReadings; reading++) {
    cumulativeValue += analogRead(IR_SENSORS[sensor]);
  }
  return cumulativeValue / numReadings;
}

boolean IRSensors::edgeDetect(int sensor) {
  boolean result = false; // default value
  int value = averageValue(sensor);

#ifdef IRSENSORS_DEBUG
  Serial.print(P("Sensor  = ")); Serial.println(sensor);
  Serial.print(P("Current = ")); Serial.println(value);
  Serial.print(P("Cal     = ")); Serial.println(irSensorEdge[sensor]);
#endif
  if (value >= irSensorEdge[sensor]) {
    result = true; // Edge detected (higher value means less reflected)
#ifdef IRSENSORS_DEBUG
    if (isDetected[sensor] == false) { // only print on initial detection
      Serial.print(LOCATION_NAME(sensor));
      Serial.println(P(" edge detected"));
    }
#endif
  }
  isDetected[sensor] = result;
  return result;
}
