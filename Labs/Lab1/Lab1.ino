#define START=28
#define STOP=28
#include <Sparki.h> // include sparki library

const int threshold = 700; // line sensor threshold
const int objectDistance = 3; // distance to object before gripping
const int gripTime = 2000; // time to grip (2 second)
const int maxObjects = 1; // total objects to retrieve

int objectIndex = 0;
bool lineLeft, lineCenter, lineRight = false;

int ping = 0;

String state = "undefined";

void setup() {
  sparki.servo (SERVO_CENTER);
  sparki.clearLCD();
  //indicates to the user that the program started:
  sparki.beep(440, 300);
  delay(300);
  sparki.beep(880, 500);
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

void followLine(bool foundObject)
{
  state = "follow line";
  if (foundObject)
  {
    state = "approaching";
    if (ping <= objectDistance)
    {
      gripObject();
      turnBack();
      readSensors();
      while (lineLeft || lineCenter || lineRight) // START is when all sensors are black/false
      {
        readSensors();
        followLine(false);
        delay(100);
      }
      sparki.beep(440, 300); // beeps to indicate that it has reached the START mark
      sparki.moveForward(maxObjects - objectIndex); // passes the mark
      releaseObject();
      sparki.moveBackward(3); //// the constant number is a small security margin to avoid the object when turning
      turnBack();

      objectIndex++;
      if (objectIndex == maxObjects)
      {
        finished();
        while(true); // finished program
      }
    }
  }
  readSensors();
  displaySensorsAndStates();
  if (!lineLeft && lineCenter && lineRight) // if the black line is below left line sensor
  {
    moveLeft(); // turn left
  }
  else if (!lineRight && lineCenter && lineLeft) // if black line is below right line sensor
  {
    moveRight(); // turn right, very off course, larger correction
  }
  else if (lineLeft && !lineCenter && lineRight) // if line only found on center line sensor
  {
    moveForwardUpdate(); // move foward (FAST)
  }
  else if (!lineLeft && !lineCenter && lineRight) // both left and center see black
  {
    moveLeftandForward(); //slightly off course (small correct)
  }
  else if (!lineRight && !lineCenter && lineLeft) // both left and center see black
  {
    moveRightandForward();
  }
  else if (lineLeft && lineCenter && lineRight)
  {
    //sparki.moveForward(); 
  }
  else if (!lineLeft && !lineCenter && !lineRight)
  {
    //follow last command
  }
}

void readSensors()
{
  // sensor is 1 if reading white and 0 if reading black
  lineLeft = sparki.lineLeft() > threshold;
  lineCenter = sparki.lineCenter() > threshold;
  lineRight = sparki.lineRight() > threshold;
  ping = sparki.ping();
  if (ping == -1) // too far or too close to something
  {
    ping = 100;
  }
}

bool startMark()
{
  return !lineLeft && !lineCenter && !lineRight;
}

void displaySensorsAndStates()
{
  sparki.clearLCD(); // wipe screen clean each run  
  sparki.print("Line Left: "); // show left line sensor on screen
  sparki.println(!lineLeft);
 
  sparki.print("Line Center: "); // show center line sensor on screen
  sparki.println(!lineCenter);
 
  sparki.print("Line Right: "); // show right line sensor on screen
  sparki.println(!lineRight);
 /*
  sparki.print("Ping: "); // ultrasonic ping ranger on screen
  sparki.print(ping);
  sparki.println(" cm");

  sparki.print("Object dist * 3: ");
  sparki.print(objectDistance * 3);
  sparki.println(" cm");
 */
  sparki.println(String("state = ") + state);
  
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

void finished()
{
  // work is done
  state = "finished";
  sparki.gripperStop();
  sparki.moveStop();
  displaySensorsAndStates();
  sparki.beep(880, 300);
  delay(300);
  sparki.beep(440, 300);
  delay(300);
  sparki.beep(880, 600);
  delay(600);
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
  sparki.moveLeft(170); // turn left a fixed angle so sensor can no longer see line (overshoots)
  sparki.moveLeft(); // turn left until robt is centered
  centerRobot();
  sparki.beep(); // line has been found on the way back
}

void loop() {
  // put your main code here, to run repeatedly:
  readSensors();
  if (ping < (objectDistance * 3))
  {
    followLine(true); // follows the line while found object
  }
  else
  {
    followLine(false);
  }

  displaySensorsAndStates();
  delay(10); // loops .1 a second (pings rate)

   /*
   int code = sparki.readIR();

   if (code != -1){
    sparki.print("Received code: ");
    sparki.println(code);
   }

   switch(code){
    //Start program
    case 28:  sparki.moveLeft(); break;
    case 64:  sparki.moveStop(); break;
    //case 24:  moveLeftInfinite(); break;
   }
  sparki.updateLCD();*/
}
