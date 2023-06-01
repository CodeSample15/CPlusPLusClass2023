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
} drivePid={0.25, 0.7, 20}, turnPid={0.6, 0.6, 20};

float PD(struct tune t, float error, float& preverror) {
  float p = error * t.kp;
  float d = (error - preverror) / t.dt;
  d *= t.kd;

  preverror = error;
  wait(t.dt, msec);
  return p + d;
}

void display() {
  while(true) {
    Brain.Screen.setCursor(1,1);
    Brain.Screen.print("inertial: %f", inert.rotation(degrees));

    wait(10, msec);

    Brain.Screen.clearScreen();
  }
}

float PD(struct tune t, float error, float& preverror, float& slew, float slewRate) {
  float p = error * t.kp;
  float d = (error - preverror) / t.dt;
  d *= t.kd;

  preverror = error;
  wait(t.dt, msec);

  float pd = p + d;

  slew += slewRate * (pd > 0 ? 1 : -1);
  if(abs((int)slew) < abs((int)pd))
    return slew;
  else
    return pd;
}

void move(int dist, float s) {
  rightmotor.setPosition(0, degrees);
  inert.setRotation(0, degrees);

  float preverrorD = 0;
  float preverrorT = 0;
  float slew = 0;
  while(abs(dist - (int)rightmotor.position(degrees)) > 3) {
    float speed = PD(drivePid, dist - rightmotor.position(degrees), preverrorD, slew, 5) * s;
    float turnSpeed = PD(turnPid, inert.rotation(degrees), preverrorT);

    rightmotor.setVelocity(speed + turnSpeed, percent);
    leftmotor.setVelocity(speed - turnSpeed, percent);

    rightmotor.spin(forward);
    leftmotor.spin(forward);
  }

  rightmotor.stop();
  leftmotor.stop();
}

void turn_pd(int rot) {
  inert.setRotation(0, degrees);
  float preverror = 0;

  while(abs(rot - (int)inert.rotation(degrees)) > 2) {
    float speed = PD(turnPid, rot - inert.rotation(degrees), preverror);

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
  thread t(display);
  
  inert.calibrate();
  while(inert.isCalibrating())
    wait(50, msec);
  inert.setRotation(0, degrees);

  move(1000, 1);
  turn_pd(90);
  move(1100, 1);
  turn_pd(-90);
  move(1800, 1);
  move(-1800, 1);
}
