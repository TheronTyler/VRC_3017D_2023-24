/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Theron                                                    */
/*    Created:      10/4/2023, 6:36:20 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

//2.35 rotations = 2ft/1 field tile
//Arc from corner to corner of circle = 3.14ft
//90 degrees is appx. 3.6895 case 'a' rotations
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
    motor_group(cata1, cata2).spinFor(fwd, revolution, rev, false);
    wait(0.5, sec);
        break;
  default:
    break;
  }
}
 //w=fwd/back -=back +=fwd

void autonomous(void) {
  mind('w',.5,-2.4); //push preload into goal
  mind('w',1,1.75);  //back off goal

  mind('a',1,-0.35); //aim cata toward red goal

  mind('w',1,-1); //back onto match load bar

  mind('q',8,100);//catapult fire fifteen (15) times w/ 0.5sec delay in between and 0.5 sec extra time
  
  mind('w',0.75,-2.35); //drive away from match load

  mind('a',1,-2); //turn to face opposite wall

  mind('w',2.5,-9.4); //drive to wall
  mind('w',1.5,0.5875); //back off wall

  mind('a',2.5,-1.84475); //turn ~45deg to other side

  mind('w',1.5,1.175); //drive foreward

  mind('a',2.5,-1.84475); //turn ~45deg to other side (totaling 90deg)
}

void usercontrol(void) {

  while (1) {
   //----------------------------------------------------------[DRIVE]
    int lateral = Controller1.Axis3.position(pct);
    int rotational = Controller1.Axis1.position(pct);

    motor_group(fLDrive, bLDrive).spin(fwd, lateral + rotational, pct);
    motor_group(fRDrive, bRDrive).spin(fwd, lateral - rotational, pct);

    //----------------------------------------------------------[Catapult]
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
