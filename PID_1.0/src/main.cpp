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
 //Settings
double kP = 0.0;
double kI = 0.0;
double kD = 0.0;
double turnkP = 0.0;
double turnkI = 0.0;
double turnkD = 0.0;

//Autonomus Settings
int desiredValue = 0;
int desiredTurnValue = 0;

int error; //SensorValue - DesiredValue | Position  -> speed -> acelleration -> jerk
int prevError = 0;; //Postion 20 milliseconds ago
int derivative; //error - prevError | Speed
int totalError = 0; //totalError = totalError + error

int turnError; //SensorValue - DesiredValue | Position  -> speed -> acelleration -> jerk
int turnPrevError = 0;; //Postion 20 milliseconds ago
int turnDerivative; //error - prevError | Speed
int turnTotalError = 0; //totalError = totalError + error

bool resetDriveSensors = false;

//variables modified for use
bool enableDrivePID = true;

int drivePID(){

  while(enableDrivePID){

    if (resetDriveSensors){
      resetDriveSensors = false;
      backLeft.setPosition(0,degrees);
      backRight.setPosition(0,degrees);

    }

    //Get position of both motors
    int backLeftPosition = backLeft.position(degrees);
    int backRightPosition = backLeft.position(degrees);
////////////////////////
//Lateral movement PID//
////////////////////////
    //Get average of two motors
    int averagePosition = (backLeftPosition + backRightPosition)/2;

    //Potential
    error = averagePosition - desiredValue;

    //Derivative
    derivative = error - prevError;

    //Integral
    totalError += error;

    double lateralMotorPower = (error * kP + derivative * kD + totalError * kI);
///////////////////////////////////////////////////////


////////////////////////
//Turning movement PID//
////////////////////////
//Get average of two motors
    int turnDifference = backLeftPosition - backRightPosition;

    //Potential
    turnError = turnDifference - desiredValue;

    //Derivative
    turnDerivative = turnError - turnPrevError;

    //Integral
    turnTotalError += turnError; //Can cause osolation

    double turnMotorPower = (turnError * turnkP + turnDerivative * turnkD + turnTotalError * turnkI);
//////////////////////////////////////////////////////////////////

    backLeft.spin(forward, lateralMotorPower + turnMotorPower, voltageUnits::volt);
    backRight.spin(forward, lateralMotorPower - turnMotorPower, voltageUnits::volt);

    //code
    prevError = error;
    turnPrevError = turnError;
    vex::task::sleep(20);

  }

 return 1;
}

void autonomous(void) {
 
 vex::task vroomVroom(drivePID);

 resetDriveSensors = true;
 desiredValue = 300;
 desiredTurnValue = 600;
/*
 vex::task::sleep(1000);

 resetDriveSensors = true;
 desiredValue = 300;
 desiredTurnValue = 300;
*/
void usercontrol(void) {
  
  while (1) {
   
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
