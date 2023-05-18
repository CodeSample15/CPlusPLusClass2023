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
} drivePid={0.4, 0, 20}, turnPid={0.3, 0, 20};

float preverror = 0;
float PD(struct tune t, float error) {
  float p = error * t.kp;
  float d = (error - preverror) / t.dt;
  d *= t.kd;

  preverror = error;
  wait(t.dt, msec);
  return p + d;
}

float PD(struct tune t, float error, float& slew, float slewRate) {
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

void move(int dist) {
  rightmotor.setPosition(0, degrees);

  float slew = 0;
  while(abs(dist - (int)rightmotor.position(degrees)) > 3) {
    float speed = PD(drivePid, dist - rightmotor.position(degrees), slew, 5);

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

  while(abs(rot - (int)inert.rotation(degrees)) > 2) {
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

  move(1000);
  turn_pd(180);
  move(1000);
  turn_pd(180);
}
