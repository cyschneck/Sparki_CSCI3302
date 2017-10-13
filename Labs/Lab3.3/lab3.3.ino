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

/************************************************************
* CSCI3302 Introduction to Robotics
* (c) Nikolaus Correll
*
* Lab 3.2: Path Planning
*
***************************************************************/


#include <Sparki.h>       // include the sparki library


int grid[4][4]={  
   {0, 0, 0, 0} , /*  initializers for row indexed by 0 */
   {1, 1, 0, 1} , /*  initializers for row indexed by 1 */
   {0, 0, 0, 0} , /*  initializers for row indexed by 2 */
   {1, 0, 1, 0}   /*  initializers for row indexed by 3 */
};

int dist[16];
int go_to[16];

int pos = 0; // robot position given as index on the graph
int goal= 13;// desired position


int cost(int i,int j){
  int x1 = i / 4;
  int y1 = i % 4;
  int x2 = j / 4;
  int y2 = j % 4;

  if(i==j) return 0;
  
  if(grid[x1][y1] || grid[x2][y2]) // if obstacle
   return 99;

  int cost = fabs(x1-x2)+fabs(y1-y2);

  if(cost>1) 
   return 99;
  else
   return cost;
  
}

void dij(int n,int v,int dist[])
{
 int i,u,count,w,flag[16],min;

 for(i=0;i<=n;i++)
  flag[i]=0, dist[i]=99; // initialize every node with cost-to-goal (infinity if not adjacent)
 dist[goal]=0;
 count=1;
 while(count<=n)
 {
  min=99;
  for(w=0;w<=n;w++)                       // find node u with the current lowest score
   if(dist[w]<min && !flag[w])
    min=dist[w],u=w;
  flag[u]=1; // u is the node with the lowest score
  //Serial.print(count); Serial.print(": "); Serial.println(u);
    count++;
  for(w=0;w<=n;w++)
   if((dist[u]+cost(u,w)<dist[w]) && !flag[w]){
    dist[w]=dist[u]+cost(u,w);
    go_to[w]=u; // store where to go when at w in order to reach goal with lowest cost
     //Serial.print("Found: "); Serial.print(w); Serial.print("->"); Serial.println(u);
   }
 }
}



void setup() 
{
  dij(15,goal,dist); // calculate a map to go to node "13"
  Serial.println("Starting...");

}

void loop() {

    dij(15,goal,dist); // calculate a map to go to node "13"
 
    int i;
  /*for(i=0;i<16;i++){
      if(i%4==0) Serial.println();
    if(dist[i]!=99) 
     Serial.print(dist[i]);
    else
     Serial.print("#");
    Serial.print(" ");
  }
  Serial.println();*/

  for(i=0;i<16;i++){
         if(dist[i]!=99) 
          Serial.print(go_to[i]);
         else
          Serial.print("#");
        Serial.print(" ");
        if((i+1)%4==0) Serial.println();
  }
  Serial.println();

  
/*
   pos=go_to[pos];
   Serial.print("Driving to ");
   Serial.println(pos);
   //if(pos==goal) pos=0;
  */
}

/* 
 *  
*/

