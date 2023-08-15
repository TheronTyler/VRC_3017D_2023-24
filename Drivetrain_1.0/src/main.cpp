/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// frontLeft            motor         1               
// frontRight           motor         2               
// backLeft             motor         3               
// backRight            motor         4               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

competition Competition;

void pre_auton(void) {
 
  vexcodeInit();

}

void autonomous(void) {
 
}

void usercontrol(void) {
  
  while (1) {
    
  enableDrivePID = false;

  while (1) {
   int _fwd = Controller1.Axis3.position(pct);       /*------------------------*/
   int lateral = Controller1.Axis1.position(pct);    /*                        */
                                                     /*                        */
   frontLeft.spin(fwd, _fwd + lateral, pct);         /*     Drive Controll     */
   frontRight.spin(fwd, _fwd - lateral, pct);        /*                        */
   backLeft.spin(fwd, _fwd + lateral, pct);          /*                        */
   backRight.spin(fwd, _fwd - lateral, pct);         /*------------------------*/

    wait(20, msec); 
  }
}


int main() {
 
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  
  pre_auton();

  
  while (true) {
    wait(100, msec);
  }
}
