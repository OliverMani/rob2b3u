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

void drive(int speed, bool forwards) {
  
}

int main() {
  LeftMotor.setVelocity(60, vex::velocityUnits::pct);
  RightMotor.setVelocity(60, vex::velocityUnits::pct);
  
  while(true){
    Vision1.takeSnapshot(1);
    if(Vision1.largestObject.exists && Vision1.largestObject.width > 5){
      LeftMotor.spin(vex::directionType::fwd);
      RightMotor.spin(vex::directionType::fwd);
    } else {
      LeftMotor.stop();
      RightMotor.stop();
      /*LeftMotor.spin(vex::directionType::rev);
      RightMotor.spin(vex::directionType::rev);*/
    }
    
  }
}
