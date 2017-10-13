/************************************************************
* CSCI3302 Introduction to Robotics
* (c) Nikolaus Correll
*
* Lab 3.1: Position Control
*
* The goal of this lab is to drive the robot to a desired position
* using Lab 2's odometry code.
*
* What do you learn:
*
* - Basic feedback control
*
* Tricks:
*
* - Use atan2() to get solutions in all four quadrants (0..360 degrees),
*   not just -90..+90 as with the standard inverse tangent.
* - Understand what the coordinate system is that atan2() uses, it might
*   be rotated from what you are using. 
***************************************************************/


#include <Sparki.h>       // include the sparki library

float maxspeed=0.0285;    // [m/s] speed of the robot that you measured
float alength=0.0851;     // [m] axle length  
float phildotr=0, phirdotr=0; // wheel speeds that you sent to the motors
float Xi=0, Yi=0, Thetai=0; // where the robot is in the world
float Xrdot, Thetardot;    // how fast the robot moves in its coordinate system

float Xg=0.1;     // Where the robot should go
float Yg=-0.1;
float Thetag=0;

float alpha, rho, eta; // error between positions in terms of angle to the goal, distance to the goal, and final angle
float a=0.1, b=1, c=0.1; // controller gains

void setup() 
{
}

void loop() {
  long int time_start = millis();
  int threshold = 700;

  // CALCULATE ERROR 
  rho   = sqrt((Xi-Xg)*(Xi-Xg)+(Yi-Yg)*(Yi-Yg));
  //alpha = Thetai-atan2(Yi-Yg,Xi-Xg)-PI/2.0;
  alpha = atan2(Yg-Yi,Xg-Xi)-Thetai;  
  eta   = Thetai-Thetag;

  // CALCULATE SPEED IN ROBOT COORDINATE SYSTEM
  Xrdot = a*rho;
  //Xrdot=0;
  Thetardot = b*alpha+c*eta;
  
  // CALCULATE WHEEL SPEED
  phildotr = (2*Xrdot - Thetardot*alength)/(2.0);
  phirdotr = (2*Xrdot + Thetardot*alength)/(2.0);
  
  // SET WHEELSPEED

  float leftspeed = abs(phildotr);
  float rightspeed = abs(phirdotr);

  if(leftspeed > maxspeed)
  {
    leftspeed = maxspeed;
  }
  if(rightspeed > maxspeed)
  {
    rightspeed = maxspeed;
  }
  leftspeed = (leftspeed/maxspeed)*100;//100
  rightspeed = (rightspeed/maxspeed)*100;//100

  if(rho > 0.01)  // if farther away than 1cm
  {
    if(phildotr > 0)
    {
      sparki.motorRotate(MOTOR_LEFT, DIR_CCW,leftspeed);
    }
    else
    {
      sparki.motorRotate(MOTOR_LEFT, DIR_CW,leftspeed);
    }
    if(phirdotr > 0)
    {
      sparki.motorRotate(MOTOR_RIGHT, DIR_CW,rightspeed);
    }
    else
    {
      sparki.motorRotate(MOTOR_RIGHT, DIR_CCW,rightspeed);
    }
  }
  else
  {
    sparki.moveStop();
  }
  
  sparki.clearLCD(); // wipe the screen
  
  sparki.print(Xi);
  sparki.print("/");
  sparki.print(Yi);
  sparki.print("/");
  sparki.print(Thetai);
  sparki.println();
  sparki.print(alpha/PI*180);
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

/* 
 *  
*/

