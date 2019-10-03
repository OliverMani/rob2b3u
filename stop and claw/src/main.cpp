/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       Clawbot Template                                          */
/*    Author:       VEX Robotics                                              */
/*    Description:  Blank Pre-Configured V5 Clawbot Project                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "myVision.h"

using namespace vex;

brain        Brain;
motor        LeftMotor(PORT1, gearSetting::ratio18_1, false);
motor        RightMotor(PORT10, gearSetting::ratio18_1, true);
motor        ArmMotor(PORT8, gearSetting::ratio18_1, false);
motor        ClawMotor(PORT3, gearSetting::ratio18_1, false);
drivetrain   Drivetrain = drivetrain(LeftMotor, RightMotor, 319, 295, distanceUnits::mm);
controller   Controller1;

vex::sonar      Sonar1         = vex::sonar(Brain.ThreeWirePort.A);

void setSpeed(double percent){
  LeftMotor.setVelocity(percent, vex::velocityUnits::pct);
  RightMotor.setVelocity(percent, vex::velocityUnits::pct);
}

int main() {
  setSpeed(30);
  ClawMotor.setVelocity(10, vex::velocityUnits::pct);
  int count = 0;
  while(true){
    Vision1.takeSnapshot(1);
    float cm = Sonar1.distance(vex::distanceUnits::cm);
    Brain.Screen.printAt(50, 50, "%1f cm       ", cm);

    if(Vision1.largestObject.exists && Vision1.largestObject.width > 5){
      if(cm > 15 || cm < 0){
        setSpeed(30);
      } else {
        setSpeed(10);
      }

      if(cm < 3.8){
        count++;
        
        ClawMotor.spin(vex::directionType::rev);
      }
      //LeftMotor.spin(vex::directionType::fwd);
      //RightMotor.spin(vex::directionType::fwd);
    } else {
      LeftMotor.stop();
      RightMotor.stop();

      

      /*LeftMotor.spin(vex::directionType::rev);
      RightMotor.spin(vex::directionType::rev);*/
    }

    Brain.Screen.printAt(50, 100, "%d         ", count);
    
  }
}
