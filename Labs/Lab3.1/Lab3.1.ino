/*******************************************  
  Lab 3.1:
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
  
#define  GRID_WIDTH  15.25
#define GRID_HEIGHT 11.75
#define PIXEL_WIDTH 31.8725
#define PIXEL_HEIGHT  15.98

unsigned long start_time; // start time recaculuated at the beginning of each loop
const float pi = 3.1415926535897932384626;
// 30 cm travelled in 10773 milliseconds = 30/10773 cm/milliseconds
    // 30/10.773 cm/s = 2.7847 cm/s
    // 0.3/10.773 m/s= 0.02785 m/s
double velocity = 0.02785; // m/s
double ax_length = 0.0851; // m or 8.51 cm
int servoDir = 1;     // 1 = moving right, -1 = moving left
int servoCur = 0;   // Current angle of servo
int servoSpd = 5;   // Speed of servo rotation (in degrees per 100ms)
int servoRange = 60;  // How much servo can rotate in either diretion (degrees)
int coordinates[2];
int obstacle_graph[16]; //if index == 0, obstacle; if index == 1, navigable
int distance[4][4];

float range = 0;      // Ping value (cm?)
  
// time it takes to rotate 360 = 37.4961 degrees/s
// time to move 360 left = 9601 ms
// time to move 360 right = 9599 ms
// time to move 360 = 9600 ms = 9.6 seconds
double speed_rotation = 37.4961; // degrees/second

float posX, posY = 0.0;
float theta = 0.0; // rotates around in degrees 

int threshold = 500; // white > 500, black < 500

bool lineLeft, lineCenter, lineRight;

//function 1 - "assigns a single number to each index of your 2D map"
int grid_to_graph(int x, int y){ //takes an x and y coordinate as input and returns a single number representing the graph node number
  int graph_i = (int) x/GRID_WIDTH; //grid_width and height defined above
  int graph_j = (int) y/GRID_HEIGHT;
  int graph_value = (4*graph_i)+graph_j; //multiply to find 0-15 value
  
  return graph_value;
}
//function 2 - "one that takes an index and returns the 2D coordinates"
int * graph_to_xyCoordinates(int graph_index){ //takes in the graph index and returns an array that acts as a tuple.
  float x_value = (graph_index/4)*GRID_WIDTH; //divide by four to get i value
  float y_value = (graph_index%4)*GRID_WIDTH; //modulo by four to get j
  coordinates[0] = x_value;
  coordinates[1] = y_value;

  return coordinates; //return array of [x,y] --> could do dictionary maybe?
}
//function 3 - "returns the cost to move from one cell to another given by its index"
int distance_to(int graph_index, int graph_go_to){
  //initialize distance array as a no-movement array
  int i, j = 0;
  while (i<4)
  {
    while (j<4)
    {
      distance[i][j]=99;
      j++;
    }
    i++;
  }
  //find i and j from the graph indices
  i = graph_index/4;
  j = graph_index%4;
  //set the current space to zero to ensure Sparki doesn't stop moving or have a fault
  distance[i][j]=0;

  if (obstacle_graph[graph_go_to] == 1)
  { //once we get object placing, this ensures that if there's an object we don't move there
    return 99;
  }
  else{
    int distance = abs(graph_index - graph_go_to); //if distance is 1 or 4, the spaces are adjacent
    if ((distance == 1) || (distance == 4)){ //check
      return 1; //return 1 for your distance as the grid is one space away
    }
    else{
      return 99; //not adjacent, large amount to ensure we don't accidentally take that route.
    }
  }
}

void setup()
{
  sparki.clearLCD();
  sparki.servo(0);
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
  sparki.println(" cm"); // print x displacement in meters
  
  sparki.print("Y: ");
  sparki.print(posY);
  sparki.println(" cm");

  sparki.print("Theta:");
  sparki.print(theta); 
  sparki.println(" degrees"); // print y displacement in meters
  sparki.print("Ping: ");
  sparki.println(range);
  
  sparki.updateLCD(); // display all of the information written to the screen
}

void loop() {
  start_time = millis();
  while (millis() < start_time + 100){
    // Wait for sparki to move for 100 milliseconds
  }
  
  // Sweep with servo while moving
  if ((servoCur + (servoDir*servoSpd) > servoRange) || (servoCur + (servoDir*servoSpd) < servoRange*(-1))) {   // If servo at either end, then flip direction
    if (servoCur > 0) {
      servoCur = servoRange - (servoSpd*2);
      servoDir = -1;
    }
    if (servoCur <= 0) {
      servoCur = (servoRange * (-1)) + (2*servoSpd);
      servoDir = 1;
    }
  }
  servoCur += (servoDir*servoSpd);
  sparki.servo(servoCur);
  range = sparki.ping();

  lineLeft   = sparki.lineLeft() < threshold;   // measure the left IR sensor (is black)
  lineCenter = sparki.lineCenter() < threshold; // measure the center IR sensor (is black)
  lineRight  = sparki.lineRight() < threshold;  // measure the right IR sensor (is black)

  if (lineCenter) // if the center line is black
  {  
    sparki.moveForward(); // move forward
    posX = posX + (cos(theta) * (velocity * 10));
    posY = posY + (sin(theta) * (velocity * 10));

    resetAtStart(lineRight, lineCenter, lineLeft); // if center line is black, checks to see if has returned to origin
  }
  else {
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










/*
61cm wide -> 128px wide
61/4=15.25cm between [0][0] and [0][1]
128/61=2.09px/cm
61/4=15.25cm*2.09px/cm=31.8725px
47cm tall -> 64px tall
47/4=11.75cm between [0][0] and [1][0]
47/64=1.36px/cm
47/4=11.75cm*1.36px/cm=15.98px
*/
/*
60cm wide -> 128px wide
60/4=15cm [0][0]->[0][1]
128/60=2.13px/cm
60/4=15cm*2.13px/cm=31.95px distance between [0][0]->[0][1]
48cm tall -> 64px tall
48/4=12cm [0][0]->[1][0]
64/48=1.33px/cm
48/4=12cm*1.33px/cm=15.96px distance between [0][0]->[1][0]
*/

