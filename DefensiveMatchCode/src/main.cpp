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

inertial sense = inertial(PORT2);
digital_out flaps = digital_out(Brain.ThreeWirePort.A);

bool DB = false;
int intakeVelocity = 85; //pct
int flywheelVelocity = 95;
bool open = false;

void calibrateInertialSensor(){
  sense.calibrate();

  while(sense.isCalibrating()){
    Controller1.Screen.clearLine(3);
    Controller1.Screen.print("calibrating");
  }
    Controller1.Screen.clearLine(3);
    Controller1.Screen.print("done");
    Controller1.rumble("...");
    wait(5,msec);
}

void pre_auton(void) {

calibrateInertialSensor();

//Config
motor_group(fLDrive, bLDrive, fRDrive, bRDrive).setStopping(hold);
intake.setStopping(brake);
motor_group(lLift, rLift).setStopping(brake);
flywheel.setStopping(coast);
motor_group(lLift, rLift).setVelocity(85, pct);
bool open = false;
}

void mind(char cmd,float delay,float revolutions) {
  switch (cmd) {
  case 'w': //forward motion
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).setVelocity(80, pct);
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).spinFor(fwd, revolutions, rev, false);
    wait(delay, sec);
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).stop();
    break;
  case 'a': //clockwise turn
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).setVelocity(80, pct);
    motor_group(fLDrive, bLDrive ).spinFor(fwd, revolutions, rev, false);
    motor_group(fRDrive, bRDrive).spinFor(reverse, revolutions, rev, false);
    wait(delay, sec);
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).stop();
    break;
  case 'i': //intake
    intake.setVelocity(100,pct);
    intake.spinFor(fwd, revolutions, rev, false);
    wait(delay, sec);
    intake.stop();
    break;
  } 
}


void inertialTurnHeading(double targetHeading){
  double currentError = targetHeading - sense.heading();
  double Kp = 0.5;
  double motorSpeed;

  while(fabs(currentError)<4){
    motorSpeed = Kp*currentError;

    motor_group(fLDrive,bLDrive).spin(fwd,-motorSpeed,pct);
    motor_group(fRDrive,bRDrive).spin(fwd,motorSpeed,pct);

    currentError = targetHeading - sense.heading();
  }
  motor_group(fLDrive,bLDrive,fRDrive,bRDrive).stop();
}

void autonomous(void) {

  //(Case,delay,revolutions) 
  mind('w',2,-3);//Push preload into goal
  sense.setHeading(0,degrees);
  mind('w',.5,.5);//back up

  //mind('a',.5,-.25);
  inertialTurnHeading(315);

  flaps.set(true);//open flaps

  mind('w',.5,.75);
  //mind('a',.5,-.5);//Remove triball from match load zone
  inertialTurnHeading(270);

  flaps.set(false);//close flaps
 
  //mind('a',.5,.25);
  intake.spinFor(reverse,25,rev, false);//reach elevation bar
  mind('w',1.5,2.75);

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
  if (Controller1.ButtonDown.pressing()){
    flywheel.spin(fwd, flywheelVelocity, pct);
  }
  else {
    flywheel.stop();
  }
  
  //Flaps   *NOT COMPLETE*
  if(!DB && Controller1.ButtonB.pressing()){
    DB = true;
    flaps.set(false);
  }

  else if(DB && Controller1.ButtonB.pressing()){
    DB = false;
    flaps.set(true);
  }
}
  wait(30, msec);
} 

int main() {

Competition.autonomous(autonomous);
Competition.drivercontrol(usercontrol);

pre_auton();

while (true) {
  wait(100, msec);
}
}