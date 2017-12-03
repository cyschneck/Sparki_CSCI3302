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
#define HARD_RIGHT 1 //001
#define SOFT_RIGHT 11 //011
#define HARD_LEFT 100
#define FORWARD_CENTER 10 //010
#define SOFT_LEFT 110
#define ALL_LIGHT 111
#define NO_LIGHT 0 ///000

// Define states for what action the robot is taking (program states)
#define FIND_LIGHT 0 // attempting to find victims
#define FOUND_VICTIM 1 // sees light, follows
#define RETURN_HOME 2 // return victim to home, drop victim, return to goal node
#define END_GAME 3 // all victims have been captured, or lost, end of all movement

const int threshold = 500; // light sensor threshold
bool lightLeft, lightCenter, lightRight = false; // light senesors see light above threshold

int light_state = NO_LIGHT;
int program_state = FIND_LIGHT;

String state = "undefined"; // print program state on display

void setup() {
  sparki.servo (SERVO_CENTER);
  sparki.clearLCD();
  delay(300);
}

void displaySensorsAndStates()
{
  sparki.clearLCD(); // wipe screen clean each run  
  sparki.print("Light state: ");
  sparki.println(light_state);
  
  sparki.println(String("state = ") + program_state); // displays state of movement (follow line, approaching, etc...)
  
  sparki.updateLCD(); //display all information written to screen
}

void readSensors()
{
  // sensor is 1 if reading white and 0 if reading black
  lightLeft = sparki.lightLeft() > threshold; // considered to be on the path if greater than threshold
  lightCenter = sparki.lightCenter() > threshold;
  lightRight = sparki.lightRight() > threshold;

  // determine move state
  // left = 100 -> hard left (only left) = 100, soft left = left + center = 100 + 010 = 110
  // center = 010
  // right = 001 -> hard right (only right) = 001, or  soft right = right + center = 001 + 010 = 011
  // all light = 111, no light = 000
  // false is 0, true is 1
  light_state = (!lightLeft*100)+(!lightCenter*10)+(!lightRight*1); // creates unique 000 number combination to represent each possible sensor combintation (e.g. BLACKBLACKWHITE, etc...)
}

void loop() {
  // put your main code here, to run repeatedly: follows the line and only changes behavior is it sees an object
  if (program_state != END_GAME) // while the program isn't done
  {
    readSensors();
    displaySensorsAndStates();
    delay(10); // loops .01 a second (pings rate)
  }
}
