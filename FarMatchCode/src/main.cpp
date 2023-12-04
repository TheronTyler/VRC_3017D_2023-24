/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       rhond                                                     */
/*    Created:      11/20/2023, 9:47:30 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
using namespace vex;

competition Competition;

//Devices
brain Brain;
controller Controller1;
motor fLDrive = motor(PORT20, true);
motor bLDrive = motor(PORT19, true);
motor fRDrive = motor(PORT11);
motor bRDrive = motor(PORT12);
motor intake = motor(PORT9);
motor launch = motor(PORT11);
motor lLift = motor(PORT10, true);
motor rLift = motor(PORT1);

//rotation cataRot = rotation(PORT16);
//digital_out flaps = digital_out(Brain.ThreeWirePort.G);
digital_out ratchet = digital_out(Brain.ThreeWirePort.A);

void pre_auton(void) {

//Config
motor_group(fLDrive, bLDrive, fRDrive, bRDrive).setStopping(coast);
intake.setStopping(brake);
motor_group(lLift, rLift).setStopping(brake);
bool open = false;
}

int intakeVelocity = 95; //pct
int launchVelocity = 95;
bool open = false;

void mind(char cmd,float delay,float revolutions) {
  switch (cmd) {
  case 'w': //forward motion
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).spinFor(fwd, revolutions, rev, false);
    wait(delay, sec);
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).stop();
    break;
  case 'a': //clockwise rotation
    motor_group(fLDrive, bLDrive ).spinFor(fwd, revolutions, rev, false);
    motor_group(fRDrive, bRDrive).spinFor(reverse, revolutions, rev, false);
    wait(delay, sec);
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).stop();
    break;
  case 'i': //clockwise rotation
    intake.spinFor(fwd, revolutions, rev, false);
    wait(delay, sec);
    intake.stop();
    break;
  }
}
void autonomous(void) {
  mind('w',1,-4);//'w' is calling the case, 1 is time in seconds, -4 is revolutions of the motor
  mind('w',3,2);//back off goal

  mind('a',.75,.75);//rotate to elevation bar

  mind('w',2,2.75);//hit elevation bar triball

  launch.spin(fwd,100,pct);
}

void usercontrol(void) {

  while (1) {
    //Drive
    int lateral = Controller1.Axis3.position(pct);
    int rotational = Controller1.Axis1.position(pct);
   
    motor_group(fLDrive, bLDrive).spin(fwd, lateral + rotational, pct);
    motor_group(fRDrive, bRDrive).spin(fwd, lateral - rotational, pct);

    //Intake
    if (Controller1.ButtonR1.pressing() || Controller1.ButtonR2.pressing()) {
      if (Controller1.ButtonR1.pressing()) {
        intake.spin(fwd, intakeVelocity, pct);
      }
      else if (Controller1.ButtonR2.pressing()) {
        intake.spin(reverse, intakeVelocity, pct);
      }

    }
    else {
      intake.stop();
    }
   
    //Lift
    if (Controller1.ButtonL1.pressing() || Controller1.ButtonDown.pressing()) {
      if (Controller1.ButtonL1.pressing() and Controller1.ButtonR1.pressing()) {
        motor_group(lLift, rLift).spin(fwd);
      }

      else if (Controller1.ButtonL2.pressing() and Controller1.ButtonR2.pressing()) {
        motor_group(lLift, rLift).spin(reverse);
      }
    }
      else {
        motor_group(lLift, rLift).stop();
      }

    //ratchet
    if (Controller1.ButtonLeft.pressing()) {
      if(open){
        ratchet.set(false);
        open = false;
      }

      else {
        ratchet.set(true);
        open = true;
      }
      }
     wait(2, msec); //needs to be tested and adjust the time
    }


    wait(20, msec);
  }

int main() {

  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
}