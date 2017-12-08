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

#define infinity 999

/* BEGIN STATE DEFINITION FOR LIGHT FOLLOWING */

// Define states for following light (sensor states)
// 1 (SEES LIGHT), 0 (DOES NOT SEE LIGHT)
#define ALL_LIGHT 111
#define NO_LIGHT 0 ///000
#define  ONLY_RIGHT 1 //001
#define  RIGHT_CENTER 11 //011
#define  ONLY_LEFT 100
#define  LEFT_CENTER 110
#define  ONLY_CENTER 10 //010
/* END LIGHT STATE DEFINITION */

/* BEGIN ROBOT ACTION DEFINITION */
// Define states for what action the robot is taking (program states)
#define FIND_LIGHT 0 // attempting to find victims
#define FOUND_LIGHT 1 // found light, implements best path algorithm
#define FOLLOW_LIGHT 2 // following best path to light source
#define LOCAL_SEARCH 3 // gets to approx. locationa and searches for victim with ping
#define FOUND_VICTIM 4 // found objects, appraoching to grab
#define RETURN_HOME 5 // grabbed victim, returning home
#define END_GAME 6 // all victims have been captured, or lost, end of all movement
/* END ROBOT ACTION DEFINITION */

/* BOARD SIZE FOR ODOMETRY */
#define  GRID_WIDTH  15.25 //GRID_WIDTHxGRID_HEIGHT is the size of the Sparki box
#define GRID_HEIGHT 11.75

#define HALF_WIDTH 7.625 //midpoint of each grid is H_W*jxH_H*i
#define HALF_HEIGHT 5.875
/* END BOARD SIZE */

/* INITILIZE LIGHT VARIABLES */
const int left_threshold = 70; // left light sensor threshold
const int right_threshold = 70; //right light sensor threshold
const int center_threshold = 20; //center light sensor threshold
bool lightLeft, lightCenter, lightRight = false; // light senesors see light above threshold
int left=sparki.lightLeft();
int center=sparki.lightCenter();
int right=sparki.lightRight();
int light_state = NO_LIGHT;
bool found_light = false;
int program_state = FIND_LIGHT;
/* END INITIALIZE LIGHT VARS */

int ping = 0;


/* START DIJKSTRA SETUP */
int numRows=5;
int numColumns=5;

int distanceToNode[numRows*numColumns];
int go_to[numRows*numColumns];

bool map_space[5][5]={ //1's are obstacles!
  {0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 1, 1, 0, 1},
  {0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0}
};

byte startPosition=1;
byte currentPosition=1;
byte goalPosition=24;
/* END DIJKSTRA SETUP */

String state = "undefined"; // print program state on display

/* DIJSTRAS RELATED FUNCTIONS */

int cost(int start, int goal){
  if (start == goal){
    return 0;
  }
  
  int dist=0;
  int x1=start/numRows;
  int x2=goal/numRows;
  int y1=start%numColumns;
  int y2=goal%numColumns;

  if(map_space[x1][y1]==1 || map_space[x2][y2]==1){
    return infinity;
  }
  
  dist=abs(x2-x1)+abs(y2-y1);

  if(dist>1){
    return infinity;
  } else {
    return 1;
  }
}

void dijkstra(int n, int v, int distance[]){ //n=number of nodes, v=goal node
  int i,u,count,w,flag[numColumns*numRows],minimum;
  for(i=0; i<n; i++){
    flag[i]=0;
    distance[i]=cost(v, i);
  }
  distance[goal]=0; //copied from Correll's file, maybe gives better sense for sparki to want to go there?
  count=1;
  while(count<=n){
    minimum=infinity;
    for(w=0;w<n;w++){
      if(distance[w]<minimum && !flag[w]){
        minimum=distance[w];
        u=w;
      }
      flag[u]=1;
      count++;
      for(w=0;w<n;w++){
        if((distance[u]+cost(u,w)<distance[w]) && !flag[w]){
          distance[w]=distance[u]+cost(u,w);
          go_to[w]=u;
        }
      }
    }
  }
}

byte positionToNode(byte positionRow, byte positionColumn){
  return((positionRow*numRows)+positionColumn);
}

/* END DIJKSTRAS RELATED FUNCTIONS */

void setup() {
  for (int node=0; node<(numRows*numColumns); node++){
    distanceToNode[node]=infinity;
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

  sparki.print("Left=");
  sparki.println(left);
  sparki.print("Center=");
  sparki.println(center);
  sparki.print("\nRight=");
  sparki.println(right);
  if(program_state==FIND_LIGHT){
    sparki.print("Light state: ");
    sparki.println(light_state);
  } else if(program_state==FOUND_LIGHT){
    sparki.print("Goal node:\n");
    sparki.println(goalPosition);
  }
  sparki.println(String("\nstate = ") + program_state); // displays state of movement (follow line, approaching, etc...)
  sparki.println("\nstate = " + state); // displays state of movement (follow line, approaching, etc...)
  
  sparki.updateLCD(); //display all information written to screen
}

void setGoal(){ //branches are not in numerical order, but in direction order
  if(light_state==1){ //directly to Sparki's left
    goalPosition=2;
  } else if(light_state==11){ //front left of Sparki
    goalPosition=13;
  } else if(light_state==10){ //directly in front of Sparki
    goalPosition=15;
  } else if(light_state==110){ //front right of Sparki
    goalPosition=5;
  } else if(light_state==100){ //directly right of Sparki
    sparki.moveRight(100);
    program_state=LOCAL_SEARCH;
    return;
  } else { //either 000 or 111
    program_state=FIND_LIGHT;
  }

  dijkstra((numRows*numColumns), goalPosition, distanceToNode);
  displaySensorsAndStates();

  program_state=FOLLOW_LIGHT;
}

void readSensors()
{
  // sensor is 1 if sees light and 0 if does not see light 
  lightLeft = sparki.lightLeft() > left_threshold;
  lightCenter = sparki.lightCenter() > center_threshold;
  lightRight = sparki.lightRight() > right_threshold;

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
    case FOUND_LIGHT: setGoal; break;
    case FOLLOW_LIGHT: break;
    // case LOCAL_SEARCH: break;
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
    readSensors();
    programStates();
    displaySensorsAndStates();
    delay(10); // loops .01 a second (pings rate)
  }
}
