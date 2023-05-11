/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\lukec                                            */
/*    Created:      Wed May 10 2023                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftmotor            motor         1               
// rightmotor           motor         2               
// inert                inertial      3               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

struct tune {
  float kp;
  float kd;
  float dt;
} drivePid={1, 0, 0}, turnPid={1, 0, 0};

float preverror = 0;
float PD(struct tune tune, float error) {
  float p = error * tune.kp;
  float d = (error - preverror) / tune.dt;
  d *= tune.kd;

  preverror = error;
  wait(tune.kd, msec);
  return p + d;
}

void move(int dist) {
  rightmotor.setPosition(0, degrees);

  while(abs(dist - (int)rightmotor.position(degrees)) > 3) {
    float speed = PD(drivePid, dist - rightmotor.position(degrees));

    rightmotor.setVelocity(speed, percent);
    leftmotor.setVelocity(speed, percent);

    rightmotor.spin(forward);
    leftmotor.spin(forward);
  }

  rightmotor.stop();
  leftmotor.stop();
}

void turn_pd(int rot) {
  inert.setRotation(0, degrees);

  while(abs((int)inert.rotation(degrees)) < abs(rot)) {
    float speed = PD(turnPid, rot - inert.rotation(degrees));

    rightmotor.setVelocity(-speed, percent);
    leftmotor.setVelocity(speed, percent);

    rightmotor.spin(forward);
    leftmotor.spin(forward);
  }

  rightmotor.stop();
  leftmotor.stop();
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  inert.calibrate();
  wait(3, sec);
  inert.setRotation(0, degrees);
}
