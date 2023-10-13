/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Theron                                                    */
/*    Created:      10/4/2023, 6:36:20 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
using namespace vex;
competition Competition;

//-------------------------------[DEVICES]
brain Brain;
controller Controller1;
motor fLDrive = motor(PORT13);
motor bLDrive = motor(PORT12);
motor fRDrive = motor(PORT18);
motor bRDrive = motor(PORT17);
motor intake = motor(PORT19);
motor cata1 = motor(PORT11);
motor cata2 = motor(PORT15);
rotation cataRot = rotation(PORT16);
digital_out flaps = digital_out(Brain.ThreeWirePort.G);
//-------------------------------[]

void pre_auton(void) {

  motor_group(fLDrive, bLDrive, fRDrive, bRDrive).setStopping(coast);
  intake.setStopping(brake);
  motor_group(cata1, cata2).setStopping(coast);
}

//--------------------------------[CONFIG]
int intakeVelocity = 95; //percentage
int catapultVelocity = 95;
bool open = false;
void mind(char cmd,float delay,float revolution) {
  switch (cmd) {
  case 'w':
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).spinFor(fwd, revolution, rev, false);
    wait(delay, sec);
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).stop();
    break;
  case 'a':
    motor_group(fLDrive, bLDrive ).spinFor(fwd, revolution, rev, false);
    motor_group(fRDrive, bRDrive).spinFor(reverse, revolution, rev, false);
    wait(delay, sec);
    motor_group(fLDrive, bLDrive, fRDrive, bRDrive).stop();
    break;
  case 'q':  
    if (cataRot.angle() < 78 || cataRot.angle() > 90) {
      motor_group(cata1, cata2).spin(fwd, 100, pct);

    } else if (true){//needs variable{
      motor_group(cata1, cata2).spin(fwd, 100, pct);

    } else {
      motor_group(cata1, cata2).stop(brakeType::hold);
    }    break;
  default:
    break;
  }
} //w=fwd/back -=back +=fwd

void autonomous(void) {
  mind('w',2.5, -3.13); //s=command 2=time in sec 3.13=revolutions
  mind('w',2, 2.55);//push under

  mind('a',1, 0.5);//turn 

  mind('w',4, 3.88);//towards center
  motor_group(cata1, cata2).spin(fwd, 100, pct);
}

void usercontrol(void) {

  while (1) {
   //----------------------------------------------------------[DRIVE]
    int lateral = Controller1.Axis3.position(pct);
    int rotational = Controller1.Axis1.position(pct);

    motor_group(fLDrive, bLDrive).spin(fwd, lateral + rotational, pct);
    motor_group(fRDrive, bRDrive).spin(fwd, lateral - rotational, pct);

    //----------------------------------------------------------[Catapult]
    if (Controller1.ButtonL1.pressing()) {
      motor_group(cata1, cata2).spin(fwd, 100, pct);
    }

    if (cataRot.angle() < 78 || cataRot.angle() > 90) {
      motor_group(cata1, cata2).spin(fwd, 100, pct);
    } 

    else if (Controller1.ButtonL1.pressing()) {
      motor_group(cata1, cata2).spin(fwd, 100, pct);
    }
     else {
      motor_group(cata1, cata2).stop(brakeType::hold);
    }

    //----------------------------------------------------------[Intake]
    if (Controller1.ButtonR1.pressing() || Controller1.ButtonR2.pressing()) {
      if (Controller1.ButtonR1.pressing()) {
        intake.spin(fwd, intakeVelocity, pct);
      }
      if (Controller1.ButtonR2.pressing()) {
        intake.spin(reverse, intakeVelocity, pct);
      }

    } 
    else {
      intake.stop();
    }

    //----------------------------------------------------------[flaps]
    if (Controller1.ButtonL2.pressing()) {
      if(open){
        flaps.set(false);
        open = false;
      }

      else {
        flaps.set(true);
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
