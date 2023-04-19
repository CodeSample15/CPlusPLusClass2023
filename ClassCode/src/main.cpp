/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\lukec                                            */
/*    Created:      Wed Apr 19 2023                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftMotor            motor         1               
// RightMotor           motor         2               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

void move(int distance, int speed) {
  RightMotor.setPosition(0, degrees);
  RightMotor.setVelocity(speed, percent);
  LeftMotor.setVelocity(speed, percent);

  while(RightMotor.position(degrees) < distance) {
    RightMotor.spin(forward);
    LeftMotor.spin(forward);
  }

  RightMotor.stop();
  LeftMotor.stop();
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  move(1000, 50);
}
