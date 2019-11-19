/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       Clawbot Template                                          */
/*    Author:       VEX Robotics                                              */
/*    Description:  Blank Pre-Configured V5 Clawbot Project                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "MyVision.h"
#include "sometasks.h"
#include <string>

#define abs(i) i < 0 ? -i : i

drivetrain   Drivetrain = drivetrain(LeftMotor, RightMotor, 319, 295, distanceUnits::mm);
controller   Controller1;

vex::sonar      Sonar1         = vex::sonar(Brain.ThreeWirePort.A);

int x, y;
const int deiling = 40;



// heldur utan um hvert vélmennið er að fara
struct Travel {
  float time; //millisecondss
  int left_motor; // velocity í prósentum
  int right_motor;
};

struct Travel list[1200]; // Til að geyma gögn um hreyfingar

// TODO: gera fallið
struct Travel getDriveData(){
  
  struct Travel result;
  result.time = (float)Brain.timer(vex::timeUnits::msec);
  result.left_motor = LeftMotor.velocity(vex::velocityUnits::pct);
  result.right_motor = RightMotor.velocity(vex::velocityUnits::pct);

  if(result.left_motor > 20 && result.right_motor > 20){
    result.left_motor = 30;
    result.right_motor = 30;
  }

  if(RightMotor.direction() == vex::directionType::rev){
    result.right_motor *= -1;
  }

  if(LeftMotor.direction() == vex::directionType::rev){
    result.left_motor *= -1;
  }

  Brain.resetTimer();
  return result;
}


void setSpeed(double percent){
  
  LeftMotor.setVelocity(percent, vex::velocityUnits::pct);
  RightMotor.setVelocity(percent, vex::velocityUnits::pct);
}


// Breiddin í myndunum er 314 (width)
int main() {
  int travelIndex = 0;
  int count = 0;
  bool searching = true;
  char going = 'N'; //N = Nowhere, F = Forwards, L = Left, R = Right

  setSpeed(30);
  ArmMotor.setVelocity(20, vex::velocityUnits::pct);
  ClawMotor.setVelocity(10 , vex::velocityUnits::pct);

  

  while(true){

    
    //Brain.Screen.printAt(10, 230, "Set velocity to 1!");
    if(searching){
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
            if(going != 'F')
            {
              struct Travel travel = getDriveData();
              list[travelIndex] = travel;
              travelIndex++;
            }
            going = 'F';
            Brain.Screen.printAt(10, 110, "Going: Forward");
            LeftMotor.spin(vex::directionType::fwd);
            RightMotor.spin(vex::directionType::fwd);
          } else if(x > deiling){
            if(going != 'R')
            {
              struct Travel travel = getDriveData();
              list[travelIndex] = travel;
              travelIndex++;
            }
            going = 'R';
            Brain.Screen.printAt(10, 110, "Going: Right  ");
            LeftMotor.spin(vex::directionType::fwd);
            //RightMotor.spin(vex::directionType::rev);
            RightMotor.stop();
          } else {
            if(going != 'L')
            {
              struct Travel travel = getDriveData();
              list[travelIndex] = travel;
              travelIndex++;
            }
            going = 'L';
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
          
          searching = false;

          
          
          ClawMotor.spin(vex::directionType::rev);

          while(count <= 2000){
            count++;
            Brain.Screen.printAt(10, 20, "Count to 2000: %d                     ", count);
          }

          
        }

        if(count > 2000){

          ClawMotor.setVelocity(3, vex::velocityUnits::pct);
          ClawMotor.spin(vex::directionType::rev);
          //Brain.Screen.printAt(10, 200, "Set velocity to 1!");
          while(count++ > 25000);

          
          searching = false;
          continue;
        }

      } else {
        // Þegar það er enginn hlutur
        if(going != 'N'){
          struct Travel travel = getDriveData();
          list[travelIndex] = travel;
          travelIndex++;
        }
        going = 'N';
        LeftMotor.stop();
        RightMotor.stop();
        Brain.Screen.printAt(10, 20, "I can't see a red object");
        /*LeftMotor.spin(vex::directionType::rev);
        RightMotor.spin(vex::directionType::rev);*/
      }
    }
    Brain.Screen.printAt(10, 140, "Movement index: %d    ", travelIndex);

    if(!searching){
      Brain.Screen.printAt(10, 20, "I must move back....");
      ClawMotor.spin(vex::directionType::rev);
      Brain.resetTimer();

      while(Brain.timer(vex::timeUnits::msec) < 5000);
      Brain.Screen.printAt(10, 20, "I must turn around");
      vex::task left(rotateLeft);
      vex::task right(rotateRight);

      count = 0;
      Brain.resetTimer();

      while(Brain.timer(vex::timeUnits::msec) < 6000);
      break; // Hérna þurfum við að komast út úr lykkjunni til að fara til baka
    }
  
    // Segja tímann
  
    Brain.Screen.printAt(10,230,"Timer: %.3f", (float)Brain.timer(vex::timeUnits::msec));

    
    
  } // WHILE TRUE LYKKJA ENDAR

  Brain.Screen.printAt(10, 20, "List length: %d           ", travelIndex);

  for(int i = travelIndex; i >= 0; i--){
    Brain.Screen.printAt(10, 50, "Index: %d         ", i);
    struct Travel travel = list[i];
    
    

    // Við þurfum að hafa þetta víxlað úr structinu
    int left_motor = travel.right_motor;
    int right_motor = travel.left_motor;

    Brain.Screen.printAt(10, 80, "Left motor velocity: %d         ", left_motor);
    Brain.Screen.printAt(10, 110, "Right motor velocity: %d         ", right_motor);



    LeftMotor.setVelocity(left_motor, vex::percentUnits::pct);
    RightMotor.setVelocity(right_motor, vex::percentUnits::pct);

    LeftMotor.spin(left_motor > 0 ? vex::directionType::fwd : vex::directionType::rev);
    RightMotor.spin(right_motor > 0 ? vex::directionType::fwd : vex::directionType::rev);
    Brain.resetTimer();
    while(Brain.timer(vex::timeUnits::msec) < travel.time);
    LeftMotor.stop();
    RightMotor.stop();
  }

}
