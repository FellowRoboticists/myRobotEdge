#ifndef _LOOK_H_
#define _LOOK_H_

// Obstacles constants
#define OBST_NONE        0 // No obstacle detected
#define OBST_LEFT_EDGE   1 // Left edge detected
#define OBST_RIGHT_EDGE  2 // Right edge detected
#define OBST_FRONT_EDGE  3 // Edge detected both right/left

class IRSensors;

class Look {
 public:

  Look(IRSensors *sensors);
  
  void begin();
  boolean lookForObstacle(int obstacle);
  boolean checkMovement();

 private:
  IRSensors *irSensors;
};

#endif
