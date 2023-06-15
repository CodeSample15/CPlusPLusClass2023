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
// EncoderX             encoder       A, B            
// EncoderY             encoder       C, D            
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

#define PI 3.14159

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

float PD(struct tune t, float error, float& preverror, bool w) {
  float p = error * t.kp;
  float d = (error - preverror) / t.dt;
  d *= t.kd;

  preverror = error;
  if(w)
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

float PD(struct tune t, float error, float& preverror, float& slew, float slewRate, bool w) {
  float p = error * t.kp;
  float d = (error - preverror) / t.dt;
  d *= t.kd;

  preverror = error;

  if(w)
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

void move(int leftDist, int rightDist, float s) {
  rightmotor.setPosition(0, degrees);
  leftmotor.setPosition(0, degrees);
  inert.setRotation(0, degrees);

  float preverror1 = 0;
  float preverror2 = 0;
  float slew1 = 0;
  float slew2 = 0;
  while(abs(leftDist - (int)leftmotor.position(degrees)) > 2 || abs(rightDist - (int)rightmotor.position(degrees)) > 2) {
    float speedL = PD(drivePid, leftDist - leftmotor.position(degrees), preverror1, slew1, 5, false) * s;
    float speedR = PD(drivePid, rightDist - rightmotor.position(degrees), preverror2, slew2, 5, true) * s;

    rightmotor.setVelocity(speedR, percent);
    leftmotor.setVelocity(speedL, percent);

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

void moveToPos(float x, float y) {
  float a = atan2(x, y)*180 / PI;
  a = 90-a;

  turn_pd(a);
  move(sqrt((x*x)+(y*y)), 1);
}

float x = 0;
float y = 0;

void positionTracking() {
  while(true) {
    float rad = inert.rotation(degrees) * PI / 180;
    x += (sin(rad) * EncoderY.position(degrees)) + (cos(rad) * EncoderX.position(degrees));
    y += (cos(rad) * EncoderY.position(degrees)) + (sin(rad) * EncoderX.position(degrees));

    wait(20, msec);

    EncoderX.setPosition(0, degrees);
    EncoderY.setPosition(0, degrees);
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  thread t(display);

  EncoderX.setPosition(0, degrees);
  EncoderY.setPosition(0, degrees);
  
  inert.calibrate();
  while(inert.isCalibrating())
    wait(50, msec);
  inert.setRotation(0, degrees);
  
  //move(1800, 1);
  //move(-1800, 1);
}
