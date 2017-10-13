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

/*Lab 3.3
   1. Write code that calculates an X-Y coordinate in the real world based on the 
   I-J coordinates of your 2D map. This coordinate should be the center of each grid cell.
   
      float arrayToWorldX(int arrayIndex) {
        // Input: grid index on array (int)
        // Output: x world coordinate (float)
    int x = arrayIndex/4;
        float xWorld = (x*GRID_WIDTH) + (.5*GRID_WIDTH);
        return(xWorld);
        
      float arrayToWorldY(int arrayIndex) {
        // Input: grid index on array (int)
        // Output: y world coordinate (float)
        int y = arrayIndex%4;
        float yWorld = (y*GRID_HEIGHT) + (.5*GRID_HEIGHT);
        return(yWorld);
        
      float coordToWorldX(int i) {
        // Input: i coordinate on grid (int)
        // Output: x world coordinate (float)
        float xWorld = (i*GRID_WIDTH) + (.5*GRID_WIDTH);
        return(xWorld);
        
      float coordToWorldY(int j) {
        // Input: j coordinate on grid (int) 
        // Output: y world coordinate (float)
        float yWorld = (j*GRID_HEIGHT) + (.5*GRID_HEIGHT);
        return(yWorld);
      }
 
 
    2. Implement a state machine that pulls an entry from the route, calculate its X-Y coordinate 
    and then use feedback control to drive there. Pull the next coordinate of the stack once the 
    robot is close enough to the last waypoint.
      
      //global variable:
      int distance_size = 0;
      int grid_space = 999;
      stack goal = new Stack();
      
      void traversal_stack(int grid_space, int dist[]){
        // we are going to build a stack for traversal
        for (int x = 0; x < distance_size(dist[]), x++){
            goal.push(dist[x]);
        }
      }
      
      // This is used to find all available nodes we go to in the found path
      int distance_size(int dist[]){
        for (int x = 0; x < 16; x++){
          if (dist[x] != 0){ // if there is a 0 here, then there must be no more nodes to go to
              distance_size++;
            } else {
              return distance_size;
            }
        }
      }
      
*/

/*


*/

#include <Sparki.h>       // include the sparki library

#define INF 999

// Setting up lab 3.1
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

// Setting up lab 3.2
// map of obstalces on the map
int grid[4][4]={  
   {0, 0, 0, 0} , /*  initializers for row indexed by 0 */
   {1, 1, 0, 1} , /*  initializers for row indexed by 1 */
   {0, 0, 0, 0} , /*  initializers for row indexed by 2 */
   {1, 0, 1, 0}   /*  initializers for row indexed by 3 */
};

int dist[16]; // distance to each node (cost)
int go_to[16];

int pos = 0; // robot position given as index on the graph
int goal= 13;// desired position

  
void setup() 
{
  // Setting up lab 3.2
  dij(15,goal,dist); // calculate a map to go to node "13"
  Serial.println("Starting...");
  //displayMapSpace();   --> LOOK AT BOTTOM, IN COMMENT
}

void loop() {
  // for lab 3.1
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
  /*
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
  */
  // perform odometry
  Xrdot=phildotr/2.0+phirdotr/2.0;
  Thetardot=phirdotr/alength-phildotr/alength;
  
  Xi=Xi+cos(Thetai)*Xrdot*0.1;
  Yi=Yi+sin(Thetai)*Xrdot*0.1;
  Thetai=Thetai+Thetardot*0.1;

  while(millis()<time_start+100); // wait until 100ms have elapsed
  
  // for lab 3.2
  dij(15,goal,dist); // calculate a map to go to node "13"
  int i;

  for(i=0;i<16;i++){
         if(dist[i]!=99) 
          Serial.print(go_to[i]);
         else
          Serial.print("#");
        Serial.print(" ");
        if((i+1)%4==0) Serial.println();
  }
  Serial.println();
}


// functions from lab 3.2 path planning
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

/*
int worldToArray(float xWorld, float yWorld) {
// Converts Sparki's x and y world coordinates (floats) to integers of array
}




int worldToGrid(xWorld, yWorld)


*/





void dij(int n,int v,int dist[])
// n = number of nodes v = goal node int dist[] = array to go to?
{
 int i,u,count,w,flag[16],min;

 for(i=0;i<=n;i++)
  flag[i]=0, dist[i]=99; // initialize every node with cost-to-goal (infinity if not adjacent)
 dist[v]=0;
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


  
  
  
  
  /*
  void displayMapSpace(){
    sparki.clearLCD();
    int grid_width_pixel = 128/4
    int grid_height_pixel = 64/4;
    
    for(int i=0; i<4; i++){
      for (int j=0; j<4; j++){
          if(grid[i][j]==1){
              sparki.drawRectFilled(grid_width_pixel*4, grid_height_pixel*4, grid_width_pixel, grid_height_pixel);
            }
        }
    }
  }
  */
