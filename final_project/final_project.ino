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

// Define states for following lighLt (sensor states)
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
#define SET_GOAL 1 // found light, implements best path algorithm
#define CALL_DIJKSTRA 2 // following best path to light source
#define RETURN_HOME 3 // grabbed victim, returning home
#define END_GAME 4 // all victims have been captured, or lost, end of all movement
#define LOCAL_SEARCH 5 // all victims have been captured, or lost, end of all movement

const int threshold = 800; // light sensor threshold
bool lightLeft, lightCenter, lightRight = false; // light senesors see light above threshold

int light_state = NO_LIGHT;
int found_light = 0;
int program_state = FIND_LIGHT;
int ping = 0;
String state = "undefined"; // print program state on display
bool finishedMoving = false; 
bool pathCalcuated = false;
////////////////////////////////////////////////////////////////////////////
#define infinity 999

unsigned long startOfLoop; // Time since start of program, calculated at each loop beginning
const float pi = 3.1415926;

const float speedMotor = 0.3 / (10773.0 / 1000.0); // m / s
const float lengthAxle = 0.084; // meters

// Pixels on Sparki display
int displayRows = 64;
int displayCols = 128;

// Dimensions of line-following map
float mapWidth = 0.45; // meters
float mapHeight = 0.45; // meters

// Map of obstacle course
const int numRows = 5;
const int numCols = 5;
// 0 are obstalces and 1 is clear path
bool envMap[numRows][numCols] = {
  {1, 1, 1, 1, 0}, // starts on first node
  {1, 0, 0, 1, 1},
  {1, 0, 0, 1, 0},
  {1, 1, 1, 1, 1},
  {1, 0, 0, 1, 1},
};
float theta = 0; // Initially facing right ("East" or 0 radians)

// Distance to each node
int distanceToNode[numRows * numCols];

byte startPos[2] = {0, 0};
byte currentPos[2] = {startPos[0], startPos[1]};
byte goalPos[2] = {0, 3};
////////////////////////////////////////////////////////////////////////////


void setup() {
  //for(int i=0;i<numNodes;i++){
  //  distanceToNode[i]=infinity;
  //}
  sparki.servo(SERVO_CENTER);
  sparki.clearLCD();
  for (int node = 0; node < numCols * numRows; node++) {
    distanceToNode[node] = infinity;
  }
}

void programStates()
{
/*
// Define states for what action the robot is taking (program states)
#define FIND_LIGHT 0 // attempting to find victims
#define SET_GOAL 1 // found light, implements best path algorithm
#define CALL_DIJKSTRA 2 // following best path to light source
#define RETURN_HOME 3 // grabbed victim, returning home
#define END_GAME 4
*/

  switch(program_state)
  {
    case FIND_LIGHT: findLight(); break;
    case SET_GOAL: setGoal(); break;
    case LOCAL_SEARCH: localSearchandGrab(); break;
    case CALL_DIJKSTRA: call_dijkstras(); break;
    case RETURN_HOME: atHome(); break;
    case END_GAME: break;
    default: break;
  }
}


void displaySensorsAndStates()
{
  sparki.clearLCD(); // wipe screen clean each run 
  /*sparki.print("PING: "); 
   * sparki.print(ping);
   * sparki.println(" cm");
   */
  if(program_state==FIND_LIGHT){
    sparki.print("Light state: ");
    sparki.println(light_state);
  } else if(program_state==SET_GOAL ){
    //sparki.print("Goal node:");
    //sparki.println(goal_node);
  }
  sparki.println(String("state = ") + program_state); // displays state of movement (follow line, approaching, etc...)
  sparki.println("state = " + state); // displays state of movement (follow line, approaching, etc...)
      
  sparki.print("PING: "); 
  sparki.print(ping);
  sparki.println(" cm");
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
    found_light = light_state;
  }
}


void findLight() {
  state = "find light";
  if (program_state == FIND_LIGHT) // while program hasn't found light
  {
    readSensors();
    displaySensorsAndStates();

    // turn in position, stops when it finds light
    sparki.moveRight(100);
    sparki.moveLeft(100);
  }
  program_state = SET_GOAL;
}

void setGoal(){
  state = "set goal";
  sparki.clearLCD();
  displaySensorsAndStates();
  // INSERT GOAL CODE
  
  program_state =CALL_DIJKSTRA;
}


void call_dijkstras(){
  state = "call dijkstras";
  displaySensorsAndStates();
  // INSERT DIJKSTRA CODE
  if (! pathCalcuated) {
    dij(numRows * numCols, posToNode(goalPos[0], goalPos[1]), distanceToNode);
  }
  dijMove();
  if (finishedMoving)
  {
  //displayMap(); 
    program_state = LOCAL_SEARCH;
  }
}

void localSearchandGrab() {
  state = "search for object";
  finishedMoving = false;
  pathCalcuated=false;
  displaySensorsAndStates();
  ping = sparki.ping();
  if (ping == -1) {
    ping = 100; // too large or too small
  }
  if (ping < 10){
    state = "grabbing object";
    displaySensorsAndStates();
    ping = sparki.ping();
    sparki.moveStop();
    sparki.gripperClose();
    delay(3000);
    sparki.gripperStop();
    program_state = RETURN_HOME;
  }
}

void atHome()
{
  state = "return home";
  displaySensorsAndStates();
 // return to start
  displaySensorsAndStates();
  goalPos[0] = startPos[0];
  goalPos[1] = startPos[1];
  if (! pathCalcuated) {
      dij(numRows * numCols, posToNode(goalPos[0], goalPos[1]), distanceToNode); // return to start
  }
  dijMove();
  displaySensorsAndStates();

  //displayMap(); 

  // INSERT 'RETURN TO HOME' CODE
  //sparki.moveStop();
  //delay(3000);
  //state = "releasing object1";
  //displaySensorsAndStates();
  if (finishedMoving)
  {

    sparki.gripperOpen();
    delay(3000);
    //sparki.gripperStop();
    state = "ALL DONE2";
    sparki.gripperStop();
    displaySensorsAndStates();
    program_state = END_GAME;
    displaySensorsAndStates();
  }
}

void displayMap() {
  sparki.clearLCD();
  int widthRow = displayRows / numRows;
  int widthCol = displayCols / numCols;
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
      if (!envMap[row][col]){
        sparki.drawRectFilled(widthCol * col, widthRow * row, widthCol, widthRow);
      }
    }
  }
  sparki.updateLCD();
}

int cost(int start, int goal){
  if (start == goal) {
    return 0;
  }
  int dist = 0;
  int x1 = start / numRows;
  int x2 = goal / numRows;
  int y1 = start % numCols;
  int y2 = goal % numCols;
  
  if (envMap[x1][y1] == 0 || envMap[x2][y2] == 0) {
    return infinity;
  }
  
  dist = abs(x2-x1)+abs(y2-y1);
  
  if (dist > 1) {
    return infinity;
  } else {
    return 1;
  }
}

void dij(int n,int v,int distance[])
{
  int i,u,count,w,flag[numCols * numRows],min;
  for(i=0;i<n;i++)
    flag[i]=0,distance[i]=cost(v,i);
  count=1;
  while(count<n)
  {
    min=infinity;
    for(w=0;w<n;w++)
      if(distance[w]<min && !flag[w])
      min=distance[w],u=w;
    flag[u]=1;
    count++;
    for(w=0;w<n;w++)
      if((distance[u]+cost(u,w)<distance[w]) && !flag[w])
        distance[w]=distance[u]+cost(u,w);
  }
}

void dijMove(){
  //displayMap(); 
  state = "dij moving";
  
    if (currentPos[0] != goalPos[0] || currentPos[1] != goalPos[1]){
    int currentNode = posToNode(currentPos[0], currentPos[1]);
    int moveToNode;
    
    // above, right, below, left
    // {distance, node}
    int possibleNodes[5][2] = {
      {infinity, 99},
      {infinity, 99},
      {infinity, 99},
      {infinity, 99},
      {infinity, 99},
    };

    // Check node above
    if (currentPos[0] != 0) {
      possibleNodes[0][0] = distanceToNode[currentNode - numCols];
      possibleNodes[0][1] = currentNode - numCols;
    }
    // Check node to right
    if (currentPos[1] != numCols - 1) {
      possibleNodes[1][0] = distanceToNode[currentNode + 1];
      possibleNodes[1][1] = currentNode + 1;
    }
    // Check node below
    if (currentPos[0] != numRows - 1) {
      possibleNodes[2][0] = distanceToNode[currentNode + numRows];
      possibleNodes[2][1] = currentNode + numRows;
    }
    // Check node to left
    if (currentPos[1] != 0) {
      possibleNodes[3][0] = distanceToNode[currentNode - 1];
      possibleNodes[3][1] = currentNode - 1;
    }

    int minStep1 = min(possibleNodes[0][0], possibleNodes[1][0]);
    int minStep2 = min(possibleNodes[2][0], possibleNodes[3][0]);
    int minStep = min(minStep1, minStep2);

    for (int directionNode = 0; directionNode < 4; directionNode++) {
      if (possibleNodes[directionNode][0] == minStep) {
        moveToNode = possibleNodes[directionNode][1];
        break;
      }
    }
    
    if (currentNode < numCols && moveToNode == currentNode - numRows) {
      // We should move up
      if (theta == 0) {
        sparki.moveLeft(90);
      } else if (theta == 180) {
        sparki.moveRight(90);
      } else if (theta == 270) {
        sparki.moveRight(180);
      }
      theta = 90;
      sparki.moveForward(mapHeight / numCols * 100.0);
      currentPos[0] -= 1;
    } else if (currentNode % 5 != numCols - 1 && moveToNode == currentNode + 1) {
      // We should move right
      if (theta == 90) {
        sparki.moveRight(90);
      } else if (theta == 180) {
        sparki.moveRight(180);
      } else if (theta == 270) {
        sparki.moveLeft(90);
      }
      theta = 0;
      sparki.moveForward(mapWidth / numRows * 100.0);
      currentPos[1] += 1;
    } else if (currentNode > numRows * (numCols - 1) - 1 < numCols && moveToNode == currentNode + numRows) {
      // We should move down
      if (theta == 0) {
        sparki.moveRight(90);
      } else if (theta == 180) {
        sparki.moveLeft(90);
      } else if (theta == 90) {
        sparki.moveRight(180);
      }
      theta = 270;
      sparki.moveForward(mapHeight / numCols * 100.0);
      currentPos[0] += 1;
    } else if (currentNode % numRows != 0 && moveToNode == currentNode - 1) {
      // We should move left
      if (theta == 0) {
        sparki.moveLeft(180);
      } else if (theta == 90) {
        sparki.moveLeft(90);
      } else if (theta == 270) {
        sparki.moveRight(90);
      }
      theta = 180;
      sparki.moveForward(mapWidth / numRows * 100.0);
      currentPos[1] -= 1;
    }
   } 
  else {
    finishedMoving = true;
    }
}

// Convert x, y position to node
byte posToNode(byte posX, byte posY) {
  return posX * numRows + posY;
}

void loop() {
  // put your main code here, to run repeatedly: follows the line and only changes behavior is it sees an object
  if (program_state != END_GAME) // while the program isn't done
  {
    ping = sparki.ping();
    programStates();
    //displaySensorsAndStates();
    delay(10); // loops .01 a second (pings rate)
}
}
