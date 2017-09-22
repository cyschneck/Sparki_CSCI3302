/************************************************************
* CSCI3302 Introduction to Robotics
* (c) Nikolaus Correll
*
* Lab 2: Odometry
*
* The goal of this lab is to keep track of the robot's position
* by integrating all motion commands sent to the motors. In order
* to accurately track the physics of the robot, it is important
* that you control loop execution time.
*
* What do you learn:
*
* - Applying basic forward kinematics equations
* - Implementing timed execution
*
* Tricks:
*
* - Find out what the maximum motor speed is by letting the robot
*   move forward a known distance (MoveForward(30), e.g. to move 30cm)
*   and stop the time it takes using millis(). 
* - Functions like MoveRight() don't reveal what motor commands
*   are actually sent. Observe the robot carefully or replace them
*   with commands that explicitely set motorspeeds. 
***************************************************************/


#include <Sparki.h>       // include the sparki library

float maxspeed=0.0285;    // [m/s] speed of the robot that you measured
float alength=0.0851;     // [m] axle length  
float phildotr=0, phirdotr=0; // wheel speeds that you sent to the motors
float Xi=0, Yi=0, Thetai=0;
float Xrdot, Thetardot;

void setup() 
{
}

void loop() {
  long int time_start = millis();
  int threshold = 700;
  
  int lineLeft   = sparki.lineLeft();   // measure the left IR sensor
  int lineCenter = sparki.lineCenter(); // measure the center IR sensor
  int lineRight  = sparki.lineRight();  // measure the right IR sensor

  if ( lineCenter < threshold ) // if line is below left line sensor
  {  
    sparki.moveForward(); // move forward
    phildotr=maxspeed;
    phirdotr=maxspeed;
  }
  else{
    if ( lineLeft < threshold ) // if line is below left line sensor
    {  
      sparki.moveLeft(); // turn left
      phildotr=-maxspeed;
      phirdotr=maxspeed;
    }
  
    if ( lineRight < threshold ) // if line is below right line sensor
    {  
      sparki.moveRight(); // turn right
      phildotr=maxspeed;
      phirdotr=-maxspeed;
    }
  }


  sparki.clearLCD(); // wipe the screen
  
  sparki.print(Xi);
  sparki.print("/");
  sparki.print(Yi);
  sparki.print("/");
  sparki.print(Thetai);
  sparki.println();
    
  sparki.updateLCD(); // display all of the information written to the screen

  // perform odometry
  Xrdot=phildotr/2.0+phirdotr/2.0;
  Thetardot=phirdotr/alength-phildotr/alength;
  
  Xi=Xi+cos(Thetai)*Xrdot*0.1;
  Yi=Yi+sin(Thetai)*Xrdot*0.1;
  Thetai=Thetai+Thetardot*0.1;

  while(millis()<time_start+100); // wait until 100ms have elapsed
}
