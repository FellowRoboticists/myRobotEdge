#include <Arduino.h>
#include "IRSensors.h"

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
  int ambient = analogRead(IR_SENSORS[sensor]); // Get ambient level
  irSensorAmbient[sensor] = ambient;
   
  // Precalculate the levels for object and edge detection
  irSensorReflect[sensor] = (ambient * (long)(100 - irReflectThreshold)) / 100;
  irSensorEdge[sensor]    = (ambient * (long)(100 + irEdgeThreshold)) / 100;
}

boolean IRSensors::edgeDetect(int sensor) {
  boolean result = false; // default value
  int value = analogRead(IR_SENSORS[sensor]); // Get the light level
  if (value >= irSensorEdge[sensor]) {
    result = true; // Edge detected (higher value means less reflected)
    if (isDetected[sensor] == false) { // only print on initial detection
      Serial.print(locationString[sensor]);
      Serial.println(" edge detected");
    }
  }
  isDetected[sensor] = result;
  return result;
}
