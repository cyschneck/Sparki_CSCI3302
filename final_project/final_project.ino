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

const int threshold = 800; // light sensor threshold
bool lightLeft, lightCenter, lightRight = false; // light senesors see light above threshold

int light_state = NO_LIGHT;
bool found_light = false;
int program_state = FIND_LIGHT;
int ping = 0;
String state = "undefined"; // print program state on display


void setup() {
  //for(int i=0;i<numNodes;i++){
  //  distanceToNode[i]=infinity;
  //}
  sparki.servo(SERVO_CENTER);
  sparki.clearLCD();
  delay(300);
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
    case CALL_DIJKSTRA: call_dijkstras(); break;
    case RETURN_HOME: returnHome(); break;
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
    program_state = SET_GOAL;
  }
}


void findLight() {
  state = "find light";
  if (program_state == FIND_LIGHT) // while program hasn't found light
  {
    readSensors();
    displaySensorsAndStates();

    // turn in position, stops when it finds light
  if (light_state == NO_LIGHT){
    sparki.moveLeft(100);
    sparki.moveRight(100);
   }
  }
}

void setGoal(){
  state = "set goal";
  sparki.clearLCD();
  displaySensorsAndStates();
  sparki.moveForward(10); // REMOVE (THIS IS FOR TESTING)

  // INSERT GOAL CODE
  
  program_state =CALL_DIJKSTRA;
}


void call_dijkstras(){
  state = "call dijkstras";
  displaySensorsAndStates();

  // INSERT DIJKSTRA CODE
  
  while (program_state != RETURN_HOME) { // LOCAL SEARCH SHOULD BE RUNNING WHILE SPARKI IS MOVING
      localSearchandGrab();
  }
}

void localSearchandGrab() {
  state = "search for object";
  displaySensorsAndStates();
  ping = sparki.ping();
  if (ping == -1) {
    ping = 100; // too large or too small
  }
  if (ping < 5){
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

void returnHome()
{
  displaySensorsAndStates();
  sparki.moveLeft(100); // TO REMOVE (TESTING CODE)

  // INSERT 'RETURN TO HOME' CODE
  
  sparki.moveStop();
  state = "releasing object";
  sparki.gripperOpen();
  delay(3000);
  sparki.gripperStop();
  state = "done";
  program_state = END_GAME;
  displaySensorsAndStates();
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
