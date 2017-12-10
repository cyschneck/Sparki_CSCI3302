/*******************************************
Final Project: Murder-Bot

Summary: 

Team: Spark-E
Cora Schneck
Kylee Bennett
Anthony Hauger
Max Messenger Bouricius
Anthony Hauger
Douglas Allen

********************************************/
#include <Sparki.h> // include sparki library

// Define states for following light (sensor states)
// 1 (SEES LIGHT), 0 (DOES NOT SEE LIGHT)
#define ALL_LIGHT 111
#define NO_LIGHT 0 ///000
#define  ONLY_RIGHT 1 //001
#define  RIGHT_CENTER 11 //011
#define  ONLY_LEFT 100
#define  LEFT_CENTER 110
#define  ONLY_CENTER 10 //010

// Define states for what action the robot is taking (program states)
#define FIND_LIGHT 0 // attempting to find victims
#define FOUND_LIGHT 1 // found light, implements best path algorithm
#define FOLLOW_LIGHT 2 // following best path to light source
#define LOCAL_SEARCH 3 // gets to approx. locationa and searches for victim with ping
#define FOUND_VICTIM 4 // found objects, appraoching to grab
#define RETURN_HOME 5 // grabbed victim, returning home
#define END_GAME 6 // all victims have been captured, or lost, end of all movement

#define infinity 99
/*Dijkstras Variables*/
int start_node=1;
int current_node=1;
int goal_node=0;

int numNodes=16;
int distanceToNode[16];
int go_to[16];

int grid[4][4]={
  {0,0,0,0},
  {0,1,1,0},
  {0,1,0,0},
  {0,0,0,1}
};
/*End Dijkstra Variables*/

const int threshold = 500; // light sensor threshold
bool lightLeft, lightCenter, lightRight = false; // light senesors see light above threshold

int light_state = NO_LIGHT;
bool found_light = false;
int program_state = FIND_LIGHT;

int ping = 0;

String state = "undefined"; // print program state on display

void setup() {
  for(int i=0;i<numNodes;i++){
    distanceToNode[i]=infinity;
  }
  sparki.servo (SERVO_CENTER);
  sparki.clearLCD();
  delay(300);
}

/*******************************************
TAKES READINGS FROM SENSORS TO DISPLAY
********************************************/

void displaySensorsAndStates()
{
  sparki.clearLCD(); // wipe screen clean each run  
  if(program_state==FIND_LIGHT){
    sparki.print("Light state: ");
    sparki.println(light_state);
  } else if(program_state==FOUND_LIGHT){
    sparki.print("Goal node:");
    sparki.println(goal_node);
  }
  sparki.println(String("state = ") + program_state); // displays state of movement (follow line, approaching, etc...)
  sparki.println("state = " + state); // displays state of movement (follow line, approaching, etc...)
  
  sparki.updateLCD(); //display all information written to screen
}


void readSensors()
{
  // sensor is 1 if sees light and 0 if does not see light 
  lightLeft = sparki.lightLeft() > threshold;
  lightCenter = sparki.lightCenter() > threshold;
  lightRight = sparki.lightRight() > threshold;

  light_state = (lightLeft*100)+(lightCenter*10)+(lightRight*1); // creates unique 000 number combination to represent each possible sensor combintation (e.g. BLACKBLACKWHITE, etc...)

  if (light_state) 
  {
    state = "light found";
    program_state = FOUND_LIGHT;
  }
}

void rotateStationary()
{
  readSensors(); // continually grab new sensor data for left/center/right sensors
  displaySensorsAndStates(); // display sensor on display

  if (light_state == NO_LIGHT){
    sparki.moveLeft(100);
    sparki.moveRight(100);
  }
}

void findLight() {
  state = "find light";
  if (program_state == FIND_LIGHT) // while program hasn't found light
  {
    readSensors();
    displaySensorsAndStates();

    // turn in position, stops when it finds light
    rotateStationary();
  }
}

void lightStates()
{
  readSensors(); // continually grab new sensor data for left/center/right sensors
  displaySensorsAndStates(); // display sensor on display

  switch(light_state) 
  {
    //case HARD_LEFT: moveLeft(); break;
    case ONLY_RIGHT: break;
    case RIGHT_CENTER: break;
    case ONLY_LEFT: break;
    case LEFT_CENTER: break;
    case ONLY_CENTER: break;
    case ALL_LIGHT: break;
    case NO_LIGHT: break; // keeps doing previous action
    default: break; // keeps doing previous action
  }
}

void setGoal(){
  sparki.clearLCD();
  if(light_state==100){
    goal_node=2;
    program_state=FOLLOW_LIGHT;
    sparki.print("Goal node:");
    sparki.println(goal_node);
    sparki.println(String("state = ") + program_state); //should be 2
    sparki.println("state = " + state);
  } else if(light_state==110){
    goal_node=11;
    program_state=FOLLOW_LIGHT;
    sparki.print("Goal node:");
    sparki.println(goal_node);
    sparki.println(String("state = ") + program_state); //should be 2
    sparki.println("state = " + state);
  } else if(light_state==10){
    goal_node=12;
    program_state=FOLLOW_LIGHT;
    sparki.print("Goal node:");
    sparki.println(goal_node);
    sparki.println(String("state = ") + program_state); //should be 2
    sparki.println("state = " + state);
  } else if(light_state==11){
    goal_node=4;
    program_state=FOLLOW_LIGHT;
    sparki.print("Goal node:");
    sparki.println(goal_node);
    sparki.println(String("state = ") + program_state); //should be 2
    sparki.println("state = " + state);
  } else if(light_state==1){
    sparki.moveRight(100);
    program_state=LOCAL_SEARCH;
    sparki.print("Goal node:");
    sparki.println(goal_node);
    sparki.println(String("state = ") + program_state); //should be 3
    sparki.println("state = " + state);
  } else {
    program_state=FIND_LIGHT;
    sparki.print("Goal node:");
    sparki.println(goal_node);
    sparki.println(String("state = ") + program_state); //should be 1
    sparki.println("state = " + state);
  }
  sparki.updateLCD();
}


int cost(int i, int j){ //i=from node, j=to node
  int x1= i/4;
  int y1= i%4;
  int x2= j/4;
  int y2= j%4;

  if(i==j){
    return 0;
  }
  if(grid[x1][y1] || grid[x2][y2]){ //if obstacle
    return(infinity);
  }
  int cost = fabs(x1-x2)+fabs(y1-y2);
  if(cost>1){
    return(infinity);
  } else {
    return(cost);
  }
}

void dij(int n, int v, int dist[]){
  int i,u,count,w,flag[16],minimum;

  for(i=0;i<=n;i++){
    flag[i]=0;
    dist[i]=infinity;
  }
  dist[goal_node]=0;
  count=1;
  while(count<=n){
    minimum=infinity;
    for(w=0;w<=n;w++){
      if(dist[w]<minimum && !flag[w]){
        minimum=dist[w];
        u=w; 
      }
    }
    flag[u]=1;
    count++;
    for(w=0;w<=n;w++){
      if((dist[u]+cost(u,w)<dist[w]) && !flag[w]){
        dist[w]=dist[u]+cost(u,w);
        go_to[w]=u;
      }
    }
  }
}

void call_dijkstras(){
  dij(15, goal_node, distanceToNode);
  //displaySensorsAndStates();
  go_to[goal_node]=17; //this makes sure we don't just drive off into the sunset and so the user isn't confused

  sparki.clearLCD();
  for(int i=0;i<16;i++){
    sparki.print(" ");
         if(distanceToNode[i]!=infinity) 
          sparki.print(go_to[i]);
         else
          sparki.print("#");
        sparki.print(" ");
        if((i+1)%4==0) sparki.println();
    }
  sparki.println();
  sparki.updateLCD();
  program_state=LOCAL_SEARCH;

  delay(500);
}
void programStates()
{
/*
// Define states for what action the robot is taking (program states)
#define FIND_LIGHT 0 // attempting to find victims
#define FOUND_LIGHT 1 // found light, implements best path algorithm
#define FOLLOW_LIGHT 2 // following best path to light source
#define LOCAL_SEARCH 3 // gets to approx. locationa and searches for victim with ping
#define FOUND_VICTIM 4 // found objects, appraoching to grab
#define RETURN_HOME 5 // grabbed victim, returning home
#define END_GAME 
*/

  switch(program_state)
  {
    case FIND_LIGHT: findLight(); break;
    case FOUND_LIGHT: setGoal(); break;
    case FOLLOW_LIGHT: call_dijkstras(); break;
    case LOCAL_SEARCH: break;
    // case FOUND_VICTIM: break;
    // case RETURN_HOME: break;
    // case END_GAME: break;
    default: break;
  }
}

void loop() {
  // put your main code here, to run repeatedly: follows the line and only changes behavior is it sees an object
  if (program_state != END_GAME) // while the program isn't done
  {
    programStates();
    //displaySensorsAndStates();
    delay(10); // loops .01 a second (pings rate)
  }
}
