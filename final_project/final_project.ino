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

const int threshold = 500; // light sensor threshold
bool lightLeft, lightCenter, lightRight = false; // light senesors see light above threshold

int light_state = NO_LIGHT;
bool found_light = false;
int program_state = FIND_LIGHT;

int ping = 0;

String state = "undefined"; // print program state on display

void setup() {
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
  sparki.print("Light state: ");
  sparki.println(light_state);
  
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
    program_state = FOUND_LIGHT;
    state = "light found";
  }
}

void findLight() {
  state = "find light";
  if (program_state == FIND_LIGHT) // while program hasn't found light
  {
    readSensors();
    displaySensorsAndStates();
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
    case FOUND_LIGHT: break;
    // case FOLLOW_LIGHT: break;
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
