/*******************************************
Lab 1:
Sparki Line Following and Object Retrieval Program

Summary: Spark follows a black line on a white paper
untl it encounters an object which it picks up
and returns to the start mark

Team: Spark-E
Cora Schneck
Kylee Bennett
Max Messenger Bouricius
Anthony Hauger

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

int ping = 0; 

String state = "undefined"; // print program state on display

void setup() {
  sparki.servo (SERVO_CENTER);
  sparki.clearLCD();
  delay(300);
}

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

void moveLeft()
{
  //turn left at a faster speed
  //hard left turn to stay on path
  sparki.motorRotate(MOTOR_LEFT, DIR_CW, 30);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, 30); // 60 to slow down while turning
}

void moveLeftandForward()
{
  //turn left at a lower speed
  //avoids overshooting when making turns
  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, 45);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, 145);
}
 
void moveRight()
{
  //turn right at a faster speed
  //prevents losing path by making a hard turn back to path
  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, 30);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CCW, 30);
}

void moveRightandForward()
{
  //turn right at a lower speed
  //avoids overshooting when making turns
  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, 145);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, 45);
}

void moveForwardUpdate()
{
  // increases the turn rotation of wheels (moves faster when going straight)
  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, 150);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, 150);
}

void programStates()
{
  // follow line = 0
  // approaching = 1
  // turning back = 2
  // returning to start = 3
  // finished = 4

  // approaching triggered by foundObject
  // turning back trigged by ping <= objectDistance (grab then turn back)
  // finished triggerd by start (cross start, release grip, turn around, finished)

  switch(program_state)
  {
    case FOLLOW_LINE: followLine(); break;
    case APPROACHING: approachObject(); break;
    case TURNING_BACK: turnGrab(); break;
    case RETURN_TO_START: returnStart(); break;
    case FINISHED: finished(); break;
    case KILL_MOVEMENT: break; // stop all movement, end program
    default: break;
  }
}

void followLine()
{
  // robot follows the line on the graph with sensors
  movementStates(); // follows line (course correction to stay on path)
  if (foundObject)
  {
    program_state = APPROACHING; // found object, approaches
  }
}

void approachObject()
{
  // if the robot sees an object within x distance, it appraoches
  if (ping > objectDistance)
  {
    movementStates(); // conintue to move foward until within x distance
  }
  else
  {
    program_state = TURNING_BACK; // at the point to turn/grab
  }
}

void turnGrab()
{
  gripObject(); // grabs object for gripTime time
  turnBack(); // turn around on the graph, to return to start
  program_state = RETURN_TO_START; // return to start  
}

void returnStart()
{
  movementStates(); // follow path (course correction to stay on path back to start)
  if (move_state == ALL_BLACK) // all black: start line determined when all sensors see black
  {
    program_state = FINISHED; // finished
  }
}

void finished()
{
  state = "finished";
  sparki.moveForward(objectDistance); // move passed start mark to put down object
  releaseObject();
  sparki.moveBackward(objectDistance); // move back to avoid hitting object
  turnBack(); // turn around and re-center on line
  sparki.moveStop(); // stop (ALL ACTION IS DONE)
  displaySensorsAndStates();
  stopAllMovement(); // stop ALL ACTION IS DONE (END OF CODE)
  //sparki.gripperOpen(); // opens gripper all 5 cm (resets to default gripper open)
}

void stopAllMovement()
{
  state = "done";
  program_state = KILL_MOVEMENT;
  displaySensorsAndStates();
}

void movementStates()
{
  readSensors(); // continually grab new sensor data for left/center/right sensors
  displaySensorsAndStates(); // display sensor on display
  
  switch(move_state) 
  {
    case HARD_LEFT: moveLeft(); break;
    case SOFT_LEFT: moveLeftandForward(); break;
    case FORWARD_CENTER: moveForwardUpdate(); break; // moves forward quickly
    case SOFT_RIGHT: moveRightandForward(); break;
    case HARD_RIGHT: moveRight(); break;
    case ALL_BLACK: break; // keeps doing previous action
    case ALL_WHITE: break; // keeps doing previous action
    default: break; // keeps doing previous action
  }
}

void readSensors()
{
  // sensor is 1 if reading white and 0 if reading black
  lineLeft = sparki.lineLeft() > threshold; // considered to be on the path if greater than threshold
  lineCenter = sparki.lineCenter() > threshold;
  lineRight = sparki.lineRight() > threshold;

  // determine move state
  // left = 100 -> hard left (only left) = 100, soft left = left + center = 100 + 010 = 110
  // center = 010
  // right = 001 -> hard right (only right) = 001, or  soft right = right + center = 001 + 010 = 011
  // all black = 111, all white = 000
  // false is 0, true is 1
  move_state = (!lineLeft*100)+(!lineCenter*10)+(!lineRight*1); // creates unique 000 number combination to represent each possible sensor combintation (e.g. BLACKBLACKWHITE, etc...)
  
  ping = sparki.ping();
  if (ping == -1) // too far or too close to something
  {
    ping = 100; // if sensor doesn't see anything, set 100 cm (default ping value)
  }
}

void displaySensorsAndStates()
{
  sparki.clearLCD(); // wipe screen clean each run  
  sparki.print("Line state: ");
  sparki.println(move_state);
  
  sparki.print("Ping: "); // ultrasonic ping ranger on screen
  sparki.print(ping);
  sparki.println(" cm");
  
  sparki.println(String("state = ") + program_state); // displays state of movement (follow line, approaching, etc...)
  
  sparki.updateLCD(); //display all information written to screen
}

void gripObject()
{
  // grab object by closing grippers for x amount of time
  sparki.moveStop();
  state = "gripping object";
  sparki.gripperClose();
  delay(gripTime);
  sparki.gripperStop();
}

void releaseObject()
{
  sparki.moveStop();
  state = "releasing object";
  sparki.gripperOpen();
  delay(gripTime);
  sparki.gripperStop();
}

void centerRobot()
{
  readSensors(); // read's sensors again, so it can look for the line (false from moving left)
  while(lineCenter) // false until the center sensor detects the line
  {
    readSensors();
    displaySensorsAndStates();
  }
}

void turnBack()
{
  state = "turn back";
  sparki.moveLeft(170); // turn left a fixed angle so sensor overshoots the lines and can no longer see it
  sparki.moveLeft(); // turn left until robt is centered
  centerRobot();
}

void loop() {
  // put your main code here, to run repeatedly: follows the line and only changes behavior is it sees an object
  if (program_state != KILL_MOVEMENT) // while the program isn't done
  {
    readSensors();
    if (ping < (objectDistance * 3))
    {
      foundObject=true; // moves foward to approach the object that has been found
    }
    
    programStates(); // triggers code with foundObject value (all swtich states)
    
    displaySensorsAndStates();
    delay(10); // loops .01 a second (pings rate)
  }
}
