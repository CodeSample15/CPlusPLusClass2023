#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor leftmotor = motor(PORT1, ratio18_1, false);
motor rightmotor = motor(PORT2, ratio18_1, true);
inertial inert = inertial(PORT3);
encoder EncoderX = encoder(Brain.ThreeWirePort.A);
encoder EncoderY = encoder(Brain.ThreeWirePort.C);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}