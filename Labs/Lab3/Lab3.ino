<<<<<<< HEAD
/*******************************************  
  Lab 3.1:
  Sparki Odometry

  Summary: Track and print X, Y displacement from 
  the start line (0.0 m, 0.0m, 0.0 degrees) as
  well as heading
  
  Team: Spark-E
  Cora Schneck
  Kylee Bennett
  Max Messenger Bouricius
  Anthony Hauger
  Douglas Allen
********************************************/

#include <Sparki.h> // include the sparki library

unsigned long start_time; // start time recaculuated at the beginning of each loop
const float pi = 3.1415926535897932384626;
// 30 cm travelled in 10773 milliseconds = 30/10773 cm/milliseconds
    // 30/10.773 cm/s = 2.7847 cm/s
    // 0.3/10.773 m/s= 0.02785 m/s
double velocity = 0.02785; // m/s
double ax_length = 0.0851; // m or 8.51 cm

// time it takes to rotate 360 = 37.4961 degrees/s
// time to move 360 left = 9601 ms
// time to move 360 right = 9599 ms
// time to move 360 = 9600 ms = 9.6 seconds
double speed_rotation = 37.4961; // degrees/second

float posX, posY = 0.0;
float theta = 0.0; // rotates around in degrees 
=======
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
//LEFT WHEEL SPEED phi_l=(2((a*d)/r)-((b*alpha+c*nu)*d))/2r (le ->x_r'=controller_gain1*distance
//RIGHT WHEEL SPEED phi_r=(2((a*d)/r)+((b*alpha+c*nu)*d))/2r  ->
// sparki.motorrotate(right, cw, [2xp - theta d/2r]*r/maxspeed*100)
//a,b,c=0.1?
// conditions: big distance = just moving towwards goals, or, small distance = at goal, change heading to match goal

#include <Sparki.h>       // include the sparki library
  
//float a,b,c = 0.1;
float a=0.1;
float b=0.5; // Lab notes has these parameters
float c=0.01;
float wheelradius=0.025;  // radius in meters
float maxspeed=0.0285;    // [m/s] speed of the robot that you measured
float alength=0.0851;     // [m] axle length  
float phildotr=0, phirdotr=0; // wheel speeds that you sent to the motors
float leftWheelSpeed=0; // speed for the left wheel [m/s]
float rightWheelSpeed=0; // speed for right wheel [m/s]
float row=0; // row used for robot speed

float Xi=0, Yi=0, Thetai=0;    // current odometry readings
float Xg=1, Yg=1, Thetag=7;   // goal position + rotation

float maxDistFromGoal=0.1;    // maximum distance from goal point before forward movement state is triggered
>>>>>>> 54d4efa380e39123ff8d7bd0397b1deb329bffad

int threshold = 500; // white > 500, black < 500

bool lineLeft, lineCenter, lineRight;
//int map[4][4];

void setup()
{
  sparki.clearLCD();
}

void resetAtStart(bool lineRight, bool lineCenter, bool lineLeft)
{
    // robot has returned to origin, reset position vectors
    // all sensors are true = sensors < threshold = all sensors are black
    if (lineRight && lineCenter && lineLeft ) {
      // at start line, reset position values
      posX, posY = 0.0; 
      // does not reset theta since it could be 180 (opposite direction)
    }
}

<<<<<<< HEAD
void displayPositionAndHeading(float posX, float posY, float theta)
{
  sparki.clearLCD(); // wipe the screen
  
  sparki.print("X: ");
  sparki.print(posX);
  sparki.println(" m"); // print x displacement in meters
  
  sparki.print("Y: ");
  sparki.print(posY);
  sparki.println(" m");

  sparki.print("Theta:");
  sparki.print(theta); 
  sparki.println(" degrees"); // print y displacement in meters
  
  sparki.updateLCD(); // display all of the information written to the screen
}
void loop() {
  start_time = millis();
  int cm = sparki.ping(); // measures the distance with Sparki's eyes
    
  while (millis() < start_time + 100){
    // Wait for sparki to move for 100 milliseconds
  }
  lineLeft   = sparki.lineLeft() < threshold;   // measure the left IR sensor (is black)
  lineCenter = sparki.lineCenter() < threshold; // measure the center IR sensor (is black)
  lineRight  = sparki.lineRight() < threshold;  // measure the right IR sensor (is black)

  if (lineCenter) // if the center line is black
  {  
    sparki.moveForward(); // move forward
    posX = posX + (cos(theta) * (velocity * 0.1));
    posY = posY + (sin(theta) * (velocity * 0.1));

    resetAtStart(lineRight, lineCenter, lineLeft); // if center line is black, checks to see if has returned to origin
  }
  else{
    if (lineLeft) // if the left line is black
    {  
      sparki.moveLeft(); // course correct if left line is black (move left until it is white)
      theta = theta - 0.1*(speed_rotation);
      theta = fmod(theta, 360); // fmod (floating point module) 360 to keep theta between 0-360
    }
    if (lineRight) // if right line is black
    {  
      sparki.moveRight(); // course correct if right line is black (move right until it is white)
      theta = theta + 0.1*(speed_rotation);
      theta = fmod(theta, 360); // fmod (floating point module) 360 to keep theta between 0-360
    }
  }
  displayPositionAndHeading(posX, posY, theta);
=======
/* All equations from the lab notes */
float distToGoal() {
  // Return pythagorean distance between current position and goal position
  return sqrt((Xg - Xi)^2 + (Yg - Yi)^2)); // follows the hypotenuse for most direct path
}

//calculate robot speed
float calculateRobotSpeed() {
 return a * distToGoal();
}

//calculate alpha
float calculateAlpha() {
 return thetai - atan2((Yg - Yi)/(Xg - Xi));
}

//calculate n
float calculateN(){
 return thetag - thetai;
}

//calculate theta dot G
float calculateThetaDotG(){
 return b * calculateAlpha() + c * calculateN(); 
}

// code to calculate the current speed of left wheel
float leftWheelSpeed() {
  leftWheelSpeed = (2 * calculateRobotSpeed() - calculateThetaDotG() * alength)/2
  return  r * leftWheelSpeed;
}

// code to calculate the current speed of right wheel
float rightWheelSpeed() {
  rightWheelSpeed = (2 * calculateRobotSpeed() - calculateThetaDotG() * alength)/2
  return  r * rightWheelSpeed;
}
/* end equations */

float angleDiscrepTheta() {
  // Return difference in angle between Sparki and the direction towards the goal
  return
} 

void loop() {
  long int time_start = millis();
  int threshold = 700;
  
  int lineLeft   = sparki.lineLeft();   // measure the left IR sensor
  int lineCenter = sparki.lineCenter(); // measure the center IR sensor
  int lineRight  = sparki.lineRight();  // measure the right IR sensor
  
  // Wheel speed, updated every loop
  float RWS = rightWheelSpeed();
  float LWS = leftWheelSpeed();

  if (distToGoal() < maxDistToGoal) {
    // postive change in x and y means it is moving in the right direction
    // if negative in x or y (or both) needs to readjust heading
    // distance between current position and final position should be decreasing (otherwise, change course)
    // real time adjusts done by changing wheel while moving
    // If we should be moving forward 
    // if ( lineCenter < threshold ) // if line is below left line sensor
    
    
    /*
    MOVEMENT (when not near goal):
    left and right wheels need to move at different speeds according to which direction Sparki needs to move
    let diffTheta = difference between Sparki's current direction and the direction which faces the goal
    
    for range (0, pi): diffTheta will be positive (e.g. left turn)
    right wheel speed: maximum
    left wheel speed: cos(diffTheta)
    
    for range (pi, 2pi): diffTheta will be negative (e.g. right turn)
    right wheel speed: cos(diffTheta)
    left wheel speed: maximum
    
    
    */
    
    
    
    /*984
    if ( abs(angleDiscrepTheta()) < maxAngleFromTheta ) { // if we're pointed towards goal
      {  
        sparki.moveForward(); // move forward
        phildotr=maxspeed;
        phirdotr=maxspeed;
      }
    {
    else{
      if ( lineLeft < threshold ) // if line is below left line sensor
      {  
        sparki.moveLeft(); // turn left
        phildotr=-maxspeed;
        phirdotr=maxspeed;
        
        
//TO MOVE LEFT SMOOTHLY        
        if(phi_l*r>0){ // if phi left * radius is greater than max speed, set to max speed
          if(phi_l*r>0.0285)
            phi_l*r=0.0285;
        }
        if(phi_r*r>0){
          if(phi_r*r>0.0285)
            phi_r*r=0.0285;
        }
        sparki.motorrotate(left, cw, ((phi_l*r)/0.0285)*100); 
        // turns to sparki left to rotate left wheel at speed of left wheel * radius / maxspeed to rotate sparki left while moving forward
        sparki.motorrotate(right, cw, ((phi_r*r)/0.0285)*100);
      }

      if ( lineRight < threshold ) // if line is below right line sensor
      {  
        sparki.moveRight(); // turn right
        phildotr=maxspeed;
        phirdotr=-maxspeed;
        
        
//TO MOVE RIGHT SMOOTHLY        
        if(phi_l*r>0){
          if(phi_l*r>0.0285)
            phi_l*r=0.0285;
        }
        if(phi_r*r>0){
          if(phi_r*r>0.0285)
            phi_r*r=0.0285;
        }
        sparki.motorrotate(left, ccw, ((phi_l*r)/0.0285)*100);
        sparki.motorrotate(right, ccw, ((phi_r*r)/0.0285)*100);
      }
    }
    */
  }
  
  // If we're at goal
  else {
    // if we're not facing eta
      // then turn towards eta
    // else we're done
    return 0;
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
>>>>>>> 54d4efa380e39123ff8d7bd0397b1deb329bffad
}
