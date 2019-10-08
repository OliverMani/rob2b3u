/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       Clawbot Template                                          */
/*    Author:       VEX Robotics                                              */
/*    Description:  Blank Pre-Configured V5 Clawbot Project                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "MyVision.h"
#include <string>

using namespace vex;

brain        Brain;
motor        LeftMotor(PORT1, gearSetting::ratio18_1, false);
motor        RightMotor(PORT10, gearSetting::ratio18_1, true);
motor        ArmMotor(PORT8, gearSetting::ratio18_1, false);
motor        ClawMotor(PORT3, gearSetting::ratio18_1, false);
drivetrain   Drivetrain = drivetrain(LeftMotor, RightMotor, 319, 295, distanceUnits::mm);
controller   Controller1;

vex::sonar      Sonar1         = vex::sonar(Brain.ThreeWirePort.A);

int x, y;
const int deiling = 40;

// heldur utan um hvert vélmennið er að fara
struct Travel {
  int time; //millisecondss
  int left_motor;
  int right_motor;
  
};

// TODO: gera fallið
struct Travel getDriveData(){
  struct Travel result;
  result.time = 0;
  result.left_motor = 0;
  result.right_motor = 0;
  return result;
}


void setSpeed(double percent){
  LeftMotor.setVelocity(percent, vex::velocityUnits::pct);
  RightMotor.setVelocity(percent, vex::velocityUnits::pct);
}




// Breiddin í myndunum er 314 (width)
int main() {

  setSpeed(30);
  ArmMotor.setVelocity(30, vex::velocityUnits::pct);
  ClawMotor.setVelocity(10, vex::velocityUnits::pct);
  int count = 0;
  int armCount = 0;
  
  while(true){
    Vision1.takeSnapshot(1);
    //Brain.Screen.printAt(10, 20, "                                  ");
    if(Vision1.largestObject.exists && Vision1.largestObject.width > 5){
      //LeftMotor.spin(vex::directionType::fwd);
      //RightMotor.spin(vex::directionType::fwd);
      Brain.Screen.printAt(10, 20, "I can see a red object  ");
      x = Vision1.largestObject.centerX - 157;
      y = Vision1.largestObject.centerY - 157;


      Brain.Screen.printAt(10, 50, "x: %d                   ", x);
      Brain.Screen.printAt(10, 80, "y: %d                   ", y);

      
      float cm = Sonar1.distance(vex::distanceUnits::cm);
      Brain.Screen.printAt(10, 170, "Distance: %.2f cm     ", cm);
      if(cm > 15 || cm < 0){
        setSpeed(30);
        if(x / deiling == 0){
          Brain.Screen.printAt(10, 110, "Going: Forward");
          LeftMotor.spin(vex::directionType::fwd);
          RightMotor.spin(vex::directionType::fwd);
        } else if(x > deiling){
          Brain.Screen.printAt(10, 110, "Going: Right  ");
          LeftMotor.spin(vex::directionType::fwd);
          //RightMotor.spin(vex::directionType::rev);
          RightMotor.stop();
        } else {
          Brain.Screen.printAt(10, 110, "Going: Left   ");
          //LeftMotor.spin(vex::directionType::rev);
          LeftMotor.stop();
          RightMotor.spin(vex::directionType::fwd);
        }
      } else if (cm > 3.8){
        setSpeed(10);
        LeftMotor.spin(vex::directionType::fwd);
        RightMotor.spin(vex::directionType::fwd);
        //ClawMotor.stop();
      } else {
        LeftMotor.stop();
        RightMotor.stop();
        Brain.Screen.printAt(10, 140, "Count: %d    ", count);
        count++;
        
        ClawMotor.spin(vex::directionType::rev);
      }

      

    } else {
      LeftMotor.stop();
      RightMotor.stop();
      Brain.Screen.printAt(10, 20, "I can't see a red object");
      /*LeftMotor.spin(vex::directionType::rev);
      RightMotor.spin(vex::directionType::rev);*/
    }

    if(count > 2000){
      ArmMotor.spin(vex::directionType::fwd);
      armCount++;
    } else {
      
    }

    if(armCount > 2500){
      armCount = 0;
      count = 0;
      //ArmMotor.setVelocity(0, vex::velocityUnits::pct);
    }

    
    
  }
}
