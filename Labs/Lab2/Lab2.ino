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
  TODO:/theta doesn't work (TODO: fix)
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
float theta = pi; // Initially facing left down the start line (pi radians and 180 degress) 

int threshold = 500;

bool lineLeft, lineCenter, lineRight;

void setup()
{
  sparki.clearLCD();
}

void resetAtStart(bool lineRight, bool lineCenter, bool lineLeft)
{
    if (lineRight && lineCenter && lineLeft ) {
      // at start line, reset position values
      posX, posY = 0.0;
    }
}

void loop() {
  start_time = millis();
  while (millis() < start_time + 100){
    // Wait for sparki to move for 100 milliseconds to get on line
  }
  
  lineLeft   = sparki.lineLeft() < threshold;   // measure the left IR sensor
  lineCenter = sparki.lineCenter() < threshold; // measure the center IR sensor
  lineRight  = sparki.lineRight() < threshold;  // measure the right IR sensor

  if (lineCenter) // if line is below left line sensor
  {  
    sparki.moveForward(); // move forward
    posX += cos(theta) * (velocity * 0.1);
    posY += sin(theta) * (velocity * 0.1);

    resetAtStart(lineRight, lineCenter, lineLeft);
  }
  else{ //theta doesn't work (TODO: fix)
    if (lineLeft) // if line is below left line sensor
    {  
      sparki.moveLeft(); // turn left
      theta -= (speed_rotation * 0.1);// / ax_length;
    }
  
    if (lineRight) // if line is below right line sensor
    {  
      sparki.moveRight(); // turn right
      theta += (speed_rotation * 0.1);// / ax_length;
    }
  }

  sparki.clearLCD(); // wipe the screen
  
  sparki.print("X: "); // show left line sensor on screen
  sparki.print(posX);
  sparki.println(" m");
  
  sparki.print("Y: "); // show center line sensor on screen
  sparki.print(posY);
  sparki.println(" m");

  sparki.print("Theta: "); // show right line sensor on screen
  theta = (theta/pi)*180.0; //convert radians to degrees to print
  sparki.print(theta); 
  sparki.println(" degrees");
  
  sparki.updateLCD(); // display all of the information written to the screen
}
