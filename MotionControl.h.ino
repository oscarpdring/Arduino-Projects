// MotionControl.h

#ifndef MotionControl_h
#define MotionControl_h

#include "Arduino.h"
#include <AccelStepper.h>

enum MotionState { IDLE,
                   MOVING_IN_RUNSPEED,
                   MOVING_TO_ORIGIN_X,
                   MOVING_TO_ORIGIN_Y,
                   MOVING_TO_POSITION,
                   AT_POSITION,
                   MOVING_TO_HOME};

struct Position {
  long x;
  long y;
};

class MotionControl {
private:  
  const float STEPS_PER_MM = 80.0;  // Number of steps per millimeter
  AccelStepper stepperX;
  AccelStepper stepperY;
  Stream& serial;                   // Referencia al objeto Serial
  short directionTowardHomeX = -1;
  short directionTowardHomeY = -1;
  Position homePos;
  short endStopStatusX = 0;
  short endStopStatusY = 0;
  bool endStopFlagX = false;
  bool endStopFlagY = false;

public:
  MotionState state = IDLE;
  MotionControl(Stream& serial);
  void goToOrigin();
  void moveToMechanicalOriginX();
  void moveToMechanicalOriginY();
  void setOrigin();
  void setHome();
  void goToHome();
  void moveX(float distanceInMillimeters);
  void moveY(float distanceInMillimeters);
  void returnErrorCode();
  void runMotors();
  bool isMovementFinished();
  float getX();
  float getY();
  void printCurrentPosition(Stream& serial);
  void update();
  void disconnectMotors();
  void emergencyStop();
  void activateMotors();
  void setSpeedX(int dir);
  void setSpeedZeroX();
  void setSpeedY(int dir);
  void setSpeedZeroY();
  void endStops();
  bool checkDistanceX(float distanceInMillimeters);
  bool checkDistanceY(float distanceInMillimeters);
};

#endif
