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
// inert                inertial      3               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

float kp = 5;
float ki = 20;
float kd = 5;

float dt = 20;

float prevError = 0;
float integral = 0;

float pid(float error) {
  float p = error * kp;

  if(error < 60 && error > 1)
    integral += error * (dt/1000);
  else
    integral = 0;

  float i = integral * ki;

  float d = (error - prevError) * kd;
  d /= dt;

  wait(dt, msec);

  return p + i + d;
}

void move(int distance) {
  RightMotor.setPosition(0, degrees);

  while(abs((int)(distance - RightMotor.position(degrees))) > 3) {
    float speed = pid(distance - RightMotor.position(degrees)) / 10;

    RightMotor.setVelocity(speed, percent);
    LeftMotor.setVelocity(speed, percent);

    RightMotor.spin(forward);
    LeftMotor.spin(forward);
  }

  RightMotor.stop();
  LeftMotor.stop();
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  inert.calibrate();
  while(inert.isCalibrating())
    wait(5, msec);
  inert.setRotation(0, degrees);



  move(1000);
}
