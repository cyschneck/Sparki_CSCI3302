/*******************************************
Lab 2:
Sparki Odometry

Summary: <INSERT SUMMARY HERE>

Team: Spark-E
Cora Schneck
Kylee Bennett
Max Messenger Bouricius
Anthony Hauger
Douglas Allen



TODO:
- We've got our updatePosition function working; simply pass in 

Additional sources: http://arcbotics.com/lessons/object-retrieval/
********************************************/
// Defines states for following a path (sensor states)
#define HARD_RIGHT 1 //001
#define SOFT_RIGHT 11 //011
#define HARD_LEFT 100
#define FORWARD_CENTER 10 //010
#define SOFT_LEFT 110
#define ALL_BLACK 111
#define ALL_WHITE 0 ///000
  // left = 100 -> hard left (only left) = 100, soft left = left + center = 100 + 010 = 110
  // center = 010
  // right = 001 -> hard right (only right) = 001, or  soft right = right + center = 001 + 010 = 011
  // all black = 111, all white = 000

// Define states for what action the robot is taking (program states)
#define FOLLOW_LINE 0
#define APPROACHING 1
#define TURNING_BACK 2
#define RETURN_TO_START 3
#define FINISHED 4
#define KILL_MOVEMENT 5



#include <Sparki.h> // include sparki library

const int threshold = 500; // line sensor threshold
const int objectDistance = 4; // distance to object before gripping
const int gripTime = 2000; // time to grip (2 second)

bool lineLeft, lineCenter, lineRight = false; // light senesors see black or white

int move_state = ALL_WHITE; // defines HARD/SOFT/ALL states
int program_state = FOLLOW_LINE; // turning back, follow line, apporaching, return to start, finished
bool foundObject = false; // approaching object function

// Initialize position values
float xPos = 0;
float yPos = 0;
float rotPos = 0;

// Define movement amounts (units: meters and degrees per 100ms)
float moveUnit = 0.000002784;
float rotUnit = 0.00374961;

int ping = 0; 

String state = "undefined"; // print program state on display

void setup() {
  sparki.servo (SERVO_CENTER);
  sparki.clearLCD();
  delay(500);
  //timeMovement(30);
  //moveForwardTime(2000);
  //timeRotation(90);
  displayPosition(0.5, 0.7, 0.9);
  displayPosition(0.5, 0.7, 0.9);
  
}

// Remote mappings cheat sheet:
// /------^-----\
// |            |
// | 69  70  71 |
// | 68  64  67 |
// |  7  21   9 |
// | 22  25  13 |
// | 12  24  94 |
// |  8  28  90 |
// | 66  82  74 |
// \____________/

void moveForwardTime(int milliTime) {
  sparki.print("moveForwardTime: ");
  sparki.println(milliTime);
  int startTime = millis();
  while (millis() <= startTime + milliTime) {
    sparki.moveForward();
  }
  sparki.moveStop();
}

void displayPosition (float deltX, float deltY, float deltRot) {
  sparki.print("current X, Y, Rot: ");
  sparki.println(xPos);
  sparki.println(yPos);
  sparki.println(rotPos);
  xPos += deltX;
  yPos += deltY;
  rotPos += deltRot;
  sparki.print("updated X, Y, Rot: ");
  sparki.println(xPos);
  sparki.println(yPos);
  sparki.println(rotPos);
}


int timeMovement(int dist) {
   // Return the amount of time it takes to travel a passed in distance (in cm)
  int startTime = 0;
  int endTime = 0;
  int travelTime = 0;
  startTime = millis();
  sparki.print("startTime: ");
  sparki.println(startTime);
  sparki.moveForward(dist);
  endTime = millis();
  sparki.print("  endTime: ");
  sparki.println(endTime);
  travelTime = endTime - startTime;
  sparki.print("    travelTime: ");
  sparki.println(travelTime);
}

void timeRotation(int degrees) {
  // Returns the time (ms) required to rotate Sparki a passed in angle (degrees)
  int startTime = 0;
  int endTime = 0;
  int travelTime = 0;
  startTime = millis();
  sparki.print("startTime: ");
  sparki.println(startTime);
  sparki.moveLeft(degrees);
  endTime = millis();
  sparki.print("  endTime: ");
  sparki.println(endTime);
  travelTime = endTime - startTime;
  sparki.print("    travelTime: ");
  sparki.println(travelTime);
}

void loop() {
  if ((millis() % 100) == 0) {
    //sparki.println("Milliseconds: ");
    //sparki.print(millis());
    // Put all your movement code here!

    
    sparki.updateLCD();
  }
  
}



//void loop() {

