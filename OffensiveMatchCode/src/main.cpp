/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Theron                                                    */
/*    Created:      11/15/2023, 4:47:29 PM                                    */
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
motor intake = motor(PORT9, true);
motor flywheel = motor(PORT8, true);
motor lLift = motor(PORT10, true);
motor rLift = motor(PORT1);

digital_out flaps = digital_out(Brain.ThreeWirePort.A);

void pre_auton(void) {

//Config
motor_group(fLDrive, bLDrive, fRDrive, bRDrive).setStopping(coast);
intake.setStopping(brake);
motor_group(lLift, rLift).setStopping(brake);
flywheel.setStopping(coast);
bool open = false;
}

int intakeVelocity = 85; //pct
int flywheelVelocity = 95;
bool open = false;

void mind(char cmd,float delay,float revolutions) {
  switch (cmd) {
  case 'w': //forward motion
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).spinFor(fwd, revolutions, rev, false);
    wait(delay, sec);
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).stop();
    break;
  case 'a': //clockwise turn
    motor_group(fLDrive, bLDrive ).spinFor(fwd, revolutions, rev, false);
    motor_group(fRDrive, bRDrive).spinFor(reverse, revolutions, rev, false);
    wait(delay, sec);
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).stop();
    break;
  case 'i': //intake
    intake.spinFor(fwd, revolutions, rev, false);
    wait(delay, sec);
    intake.stop();
    break;
  }
}

void autonomous(void) {
  mind('w',2.5,7);//'w' is calling the case, 1 is time in seconds, -4 is revolutions of the motor //Drive to front of goal
  
  mind('a',1.5,1);//Turn to goal

  mind('i',1,-2.5);//Deposite alliance triball to the goal

  mind('w',.5,1.25);//Push triball into goal
  mind('w',1,-2.125);//back off goal

  mind('a',1.5,-1);//turn to triball

  motor_group(fLDrive, bLDrive, fRDrive, bRDrive).spinFor(fwd,2,rev, false);
  intake.spinFor(fwd,10,rev,false);
  wait(3,sec);
  motor_group(fLDrive, bLDrive, fRDrive, bRDrive, intake).stop();

  mind('w',1,-2);//re-align with goal

  mind('a',1.125,1);//turn to goal

  mind('w',1,2.125);//drive to goal

  mind('i',1,-2.5);//Deposite alliance triball to the goal

  mind('w',.5,1.25);//Push triball into goal
}

void usercontrol(void) {
while (1) {
  //Drive
  int lateral = Controller1.Axis3.position(pct);
  int rotational = Controller1.Axis1.position(pct);
   
  motor_group(fLDrive, bLDrive).spin(fwd, lateral + rotational, pct);
  motor_group(fRDrive, bRDrive).spin(fwd, lateral - rotational, pct);

  //Intake
  if (Controller1.ButtonR1.pressing()) {
  intake.spin(fwd, intakeVelocity, pct);
  }
  else if (Controller1.ButtonR2.pressing()) {
  intake.spin(reverse, intakeVelocity, pct);
  }
  else {
    intake.stop();
  }

  //Lift
  if (Controller1.ButtonL1.pressing() || Controller1.ButtonL2.pressing()) {
    if (Controller1.ButtonL1.pressing()) {
      motor_group(lLift, rLift).spin(fwd);
    }
    else if (Controller1.ButtonL2.pressing()) {
      motor_group(lLift, rLift).spin(reverse);
    }
  }
    else {
      motor_group(lLift, rLift).stop();
    }

  //Flywheel
  if (Controller1.ButtonUp.pressing()){
    flywheel.spin(fwd, flywheelVelocity, pct);
  }
  else {
    flywheel.stop();
  }
  
  //flaps   *NOT COMPLETE*
  if (Controller1.ButtonLeft.pressing()) {
    if(open){
      flaps.set(false);
      open = false;
    }
    else {
      flaps.set(true);
      open = true;
    }
  }
  wait(2, msec); //needs to be tested and adjust the time for debounce
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