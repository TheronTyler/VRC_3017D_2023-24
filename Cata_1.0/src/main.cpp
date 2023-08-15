/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
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
    
      if (Controller1.ButtonL1.pressing()){
    cata.spin(fwd,100,pct);
  }

  if(Controller1.ButtonL2.pressing()){
    cata.spin(reverse,100,pct);
  }
  
  else{
    cata.stop();
  }

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
