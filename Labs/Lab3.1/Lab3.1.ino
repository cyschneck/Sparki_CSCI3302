/*******************************************  
  Lab 2:
  Sparki Odometry

  Summary: Track and print X, Y displacement from 
  the start line (0.0 m, 0.0m, 0.0 degrees) as
  well as heading
  
  Team: Spark-E
  Cora Schneck
  Kylee Bennett
  Max Messenger Bouricius
  Anthony Hauger
  Douglas Allen
********************************************/

#include <Sparki.h> // include the sparki library

unsigned long start_time; // start time recaculuated at the beginning of each loop
const float pi = 3.1415926535897932384626;
// 30 cm travelled in 10773 milliseconds = 30/10773 cm/milliseconds
    // 30/10.773 cm/s = 2.7847 cm/s
    // 0.3/10.773 m/s= 0.02785 m/s
double velocity = 0.02785; // m/s
double ax_length = 0.0851; // m or 8.51 cm

// time it takes to rotate 360 = 37.4961 degrees/s
// time to move 360 left = 9601 ms
// time to move 360 right = 9599 ms
// time to move 360 = 9600 ms = 9.6 seconds
double speed_rotation = 37.4961; // degrees/second

float posX, posY = 0.0;
float theta = 0.0; // rotates around in degrees 

int threshold = 500; // white > 500, black < 500

bool lineLeft, lineCenter, lineRight;

void setup()
{
  sparki.clearLCD();
}

void resetAtStart(bool lineRight, bool lineCenter, bool lineLeft)
{
    // robot has returned to origin, reset position vectors
    // all sensors are true = sensors < threshold = all sensors are black
    if (lineRight && lineCenter && lineLeft ) {
      // at start line, reset position values
      posX, posY = 0.0; 
      // does not reset theta since it could be 180 (opposite direction)
    }
}

void displayPositionAndHeading(float posX, float posY, float theta)
{
  sparki.clearLCD(); // wipe the screen
  
  sparki.print("X: ");
  sparki.print(posX);
  sparki.println(" m"); // print x displacement in meters
  
  sparki.print("Y: ");
  sparki.print(posY);
  sparki.println(" m");

  sparki.print("Theta:");
  sparki.print(theta); 
  sparki.println(" degrees"); // print y displacement in meters
  
  sparki.updateLCD(); // display all of the information written to the screen
}
void loop() {
  start_time = millis();
  while (millis() < start_time + 100){
    // Wait for sparki to move for 100 milliseconds
  }
  lineLeft   = sparki.lineLeft() < threshold;   // measure the left IR sensor (is black)
  lineCenter = sparki.lineCenter() < threshold; // measure the center IR sensor (is black)
  lineRight  = sparki.lineRight() < threshold;  // measure the right IR sensor (is black)

  if (lineCenter) // if the center line is black
  {  
    sparki.moveForward(); // move forward
    posX = posX + (cos(theta) * (velocity * 0.1));
    posY = posY + (sin(theta) * (velocity * 0.1));

    resetAtStart(lineRight, lineCenter, lineLeft); // if center line is black, checks to see if has returned to origin
  }
  else{
    if (lineLeft) // if the left line is black
    {  
      sparki.moveLeft(); // course correct if left line is black (move left until it is white)
      theta = theta - 0.1*(speed_rotation);
      theta = fmod(theta, 360); // fmod (floating point module) 360 to keep theta between 0-360
    }
    if (lineRight) // if right line is black
    {  
      sparki.moveRight(); // course correct if right line is black (move right until it is white)
      theta = theta + 0.1*(speed_rotation);
      theta = fmod(theta, 360); // fmod (floating point module) 360 to keep theta between 0-360
    }
  }
  displayPositionAndHeading(posX, posY, theta);
}
