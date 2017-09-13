#define HARD_RIGHT 001
#define SOFT_RIGHT 011
#define HARD_LEFT 100
#define FORWARD_CENTER 010
#define SOFT_LEFT 110
#define ALL_BLACK 111
#define ALL_WHITE 000
  // left = 100 -> hard left (only left) = 100, soft left = left + center = 100 + 010 = 110
  // center = 010
  // right = 001 -> hard right (only right) = 001, or  soft right = right + center = 001 + 010 = 011
  // all black = 111, all white = 000
#include <Sparki.h> // include sparki library

const int threshold = 500; // line sensor threshold
const int objectDistance = 4; // distance to object before gripping
const int gripTime = 2000; // time to grip (2 second)
const int maxObjects = 1; // total objects to retrieve

int objectIndex = 0;
bool lineLeft, lineCenter, lineRight = false;

int move_state = 0; // defines HARD/SOFT/ALL states
int program_state = 0; // turning back, follow line, apporaching, finished
bool foundObject = false; // approaching object function

int ping = 0;

String state = "undefined";

void setup() {
  sparki.servo (SERVO_CENTER);
  sparki.clearLCD();
  //indicates to the user that the program started:
  //sparki.beep(440, 300);
  delay(300);
  //sparki.beep(880, 500);
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

void moveLeftGran(int dist) {
  sparki.moveLeft(300);
  sparki.moveRight(200);
}

int readRemote() {
  int code = sparki.readIR();
   if (code != -1)
   {
    sparki.print("Code received in function: ");
    sparki.println(code);
    return code;
   }
}
void moveLeft()
{
  //turn left at a lower speed:
  sparki.motorRotate(MOTOR_LEFT, DIR_CW, 30);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, 30); // 60 to slow down while turning
}

void moveLeftandForward()
{
  //turn left at a lower speed:
  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, 45);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, 145);
}
 
void moveRight()
{
  //turn right at a lower speed:
  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, 30);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CCW, 30);
}

void moveRightandForward()
{
  //turn right at a lower speed:
  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, 145);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, 45);
}

void moveForwardUpdate()
{
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
  // finsihed triggerd by start (cross start, release grip, turn around, finished)
  switch(program_state)
  {
    case 0: followLine(); break;
    case 1: approachObject(); break;
    case 2: turnGrab(); break;
    case 3: returnStart(); break;
    case 4: finished(); break;
    default: break;
  }
}

void followLine()
{
  movementStates();
  if (foundObject)
  {
    program_state = 1; // found object, approaches
  }
}

void approachObject()
{
  if (ping > objectDistance)
  {
    movementStates();
  }
  else
  {
    program_state = 2; // at the point to turn/grab
  }
}

void turnGrab()
{
  gripObject();
  turnBack();
  program_state = 3; // return to start  
}

void returnStart()
{
  movementStates();
  if (move_state == 111) // all black
  {
    program_state = 4; // finished
  }
}

void finished()
{
  // work is done
  state = "finished";
  sparki.gripperStop();
  sparki.moveStop();
  displaySensorsAndStates();
  delay(300);
  sparki.gripperOpen(5);
  delay(600);
}

void movementStates()
{
  readSensors();
  displaySensorsAndStates();
  
  switch(move_state) 
  {
    case 100: moveLeft(); break;
    case 110: moveLeftandForward(); break;
    case 10: moveForwardUpdate(); break; // moves forward quickly
    case 11: moveRightandForward(); break;
    case 1: moveRight(); break;
    case 111: break;
    case 000: break;
    default: break;
  }
}

void readSensors()
{
  // sensor is 1 if reading white and 0 if reading black
  lineLeft = sparki.lineLeft() > threshold;
  lineCenter = sparki.lineCenter() > threshold;
  lineRight = sparki.lineRight() > threshold;

  // determine move state
  // left = 100 -> hard left (only left) = 100, soft left = left + center = 100 + 010 = 110
  // center = 010
  // right = 001 -> hard right (only right) = 001, or  soft right = right + center = 001 + 010 = 011
  // all black = 111, all white = 000
  // false is 0, true is 1
  move_state = (!lineLeft*100)+(!lineCenter*10)+(!lineRight*1);
  
  ping = sparki.ping();
  if (ping == -1) // too far or too close to something
  {
    ping = 100;
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
  
  sparki.println(String("state = ") + program_state);
  
  sparki.updateLCD(); //display all information written to screen
}

void gripObject()
{
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
  //sparki.beep(); // line has been found on the way back
}

void loop() {
  // put your main code here, to run repeatedly: follows the line and only changes behavior is it sees an object
  readSensors();
  if (ping < (objectDistance * 3))
  {
    foundObject=true; // moves foward to approach the object that has been found
  }
  
  programStates(); // triggers code with foundObject value (all swtich states)
  
  displaySensorsAndStates();
  delay(10); // loops .01 a second (pings rate)
}
