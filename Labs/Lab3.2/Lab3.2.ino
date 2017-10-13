

#include "stdio.h"
#include <Sparki.h> // include sparki library
//#include "conio.h"
#define infinity 999

/* Notes from Correll:
 for 4x4 graph:
 12  3 4
 
 x coord = i % 4
 y coord = i / 4
 cost from index i to index j:
*/


/* Pseudo:
 - we need:
     - grid[16] filled with 0 (free) or 1 (obstacle) --> have an array obstacle, currently filled for the test case.
    - index to [x, y] coordinates (int) function: i%4 = x, i/4 = y --> function1 = x,y to 0-15, function2 = 0-15 to x,y
    - goto[16] array contains lowest cost route to go towards source
      - value = which node to go to next (e.g. 2 = 'go to 2 next')
        - e.g. for:
          0   0   0   0
          1   1   0   1
          0   0   0   0
          0   1   0g  1
        - goto would look like: 
          1   2   5   2
          inf inf 10  inf
          9   10  14  10  
          8   inf 14  inf
    - check for adjacents:
      - up: i - 4 > 0
      - down: i + 4 < 15
      - left: i%4 != 0 (equivalent to i%4 > 0, in this case)
      - right: i%4 != 3 (equivalent to i%4 < 3, in this case)
    - if any of these are valid, 
    - update costs to each of these, I think?
    
      
      [0  01 02 03]
      [04 05 06 07]
      [08 09 10 11]
      [12 13 14 15]
*/

/*
cost(i, j) {
    x1 = i % 4;
    x2 = j % 4;
    y1 = i / 4;
    y2 = j / 4;
    if (grid[x1][y1] || grid[x2][y2]) { return infinity; }
    i -> x,y
    else { return abs(x1-x2) + abs(y1-y2) }
}
*/

// v = vertex (node2)
// n = node
// dist = distance
// flag = whether that node has been visited already (i.e. frozen)

/* From online Dijkstra to our code:
  - graph reference name?
  - update min=99 to use #defined infinity (999)? (DONE)
  - 
*/

/* Test graph with obstacles (o):
  0   1o  2o  3
  4   5   6   7
  8o  9   10o 11o
  12  13  14  15
  
  Shortest path from 0 to 15 should be [0, 4, 5, 9, 13, 14, 15]
*/

//defining grid constants
#define GRID_WIDTH  15.25
#define GRID_HEIGHT 11.75
//array and matrix initialization
int distance[16]; //distance to get from start node to -a- node -> should be filled with 1's and INF's
int cost[4][4]; //distance to get from start node to end node -> should look something like (if going from 0 to 4) [0, 1, 2, 3][INF, INF, INF, INF]...
short int grid = [0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0]; //graph of 0's and 1's -> 0==no obstacle, 1==obstacle --> made for test case currently
short int flags[16];
//initializing start and end nodes
int start = 0;
int goal = 15;
int node = 15;

/**************************************
************START 10/2 AH**************
**************************************/

int check_adjacency(int point_1, int point_2){
    //obtains an absolute value (0-15) of the distance between two nodes
    //take that distance and divides by four+mods by four
         //ex: from 0 to 7 
	 //    "distance" is 7, but that's one down and three to right (4 spaces)
	 //    (d/4)+(d%4)=1+3=4

    int grid_distance = math.abs(point_1 - point2);
    int neighbor = ((int)grid_distance/4)+(grid_distance%4);
      if (grid[*point 2] == 1){
        return infinity;
      }
      else if (neighbor > 1){ //not a neighbor
        return infinity;
      } else { //space is "empty" and the space is a neighbor
        return 1;
      }
}

//POSSIBLY UNNEEDED 19:37
int space_distance(int point_1, int point_2){
    //obtains an absolute value (0-15) of the distance between two nodes
    //take that distance and divides by four+mods by four
         //ex: from 0 to 7 
	 //    "distance" is 7, but that's one down and three to right (4 spaces)
	 //    (d/4)+(d%4)=1+3=4

    int grid_distance = math.abs(point_1 - point2);
    int num_spaces = ((int)grid_distance/4)+(grid_distance%4);
    return num_spaces;
}

void dijkstra_algorithm(int num_node, int start_node, int cost[4][4], int distance[16])
{
    int i,u,x,y,z,count,w,flag[16],min; //flag is if node has been looked at.

    //resets flag to all 0, returns distance with 1 (adjacent and no object) or 99 (not adjacent or no object)
    for(i=0;i<=node;i++) 
    {
        flag[i]=0;
	distance[i]=check_adjacency(start_node, i);
    }

    count=2;

    //1.check if the space is a neighbor and has NOT been checked
    //2.if true, then we keep track of that grid (0-15) number
    //3.mark that node as checked
    //4.increases count -- count = 2 as we've "checked" the space we're on and we've checked a neighbor
    //5.generates a cost matrix from the nearest-unobstructed-unchecked neighbor to any node w
    //6.if (distance[u]=)1+the cost to get from u to any node w<distance to node w AND w is unchecked, sets distance[w]=number of spaces to get to w from the start node.

    while(count<=node)
    {
        min=infinity;
        for(w=0;w<=node;w++)
        {
            if(distance[w]<min && !flag[w]){
                min=distance[w];
		x= (int) w/4;
		y= w%4; //x,y are the 4x4 coordinates of w
		u=w;
		}
        }
        flag[u]=1;
        count++;
        for(w=0;w<=node;w++)
        {
	    z=space_distance(u, w);
            if((distance[u]+cost[w]<distance[w]) && !flag[w]){
                distance[w]=distance[u]+cost[u][w];
        }
    }
    if (___==goal){
	return True;
    }
    else{
	//call Dijkstra's again

	if (something == True){
	    //Append node to the path array here
    }
}

/********************************
************END 10/12************
********************************/

int checkAdjacent(int i, int j) {
  // Checks if node with index j is adjacent to i
  // Not sure if this works yet! I basically just vomited my thought process into codeshare. Somebody verify please?
  // j = up
  if ((i - 4) == j) {
    return 1;
  }
  // j = down
  if ((i + 4) == j) {
    return 1;
  }
  // j = left
  if ((i % 4) == (j % 4) - 1) {
    return 1;
  }
  // j = right
  if ((i % 4) == (j % 4) + 1) {
    return 1;
  }
  else {
    return 0;
  }
  
  int up = (i - 4 >= 0);
  int down = (i + 4 <= 15);
  int left = (i % 4 != 0);
  int right = (i % 4 != 3);
  if (up || down || left || right) {
    return 1;
  } else {
    return 0;
}

int main()
{
    int node,v,i,j,cost[10][10],distance[10];
    sparki.clearLCD();
    
    
//    printf("n Enter the number of nodes:");
//    scanf(&node);
    
    /* 
    printf("n Enter the cost matrix:n");
    for(i=0;i<=node;i++)
        for(j=1;j<=node;j++)
            {
                scanf(&cost[i][j]);       // Gutting this sytem in favor of check_adjacency function
                if(cost[i][j]==0)
                    cost[i][j]=infinity;
            }
    */
//    printf("n Enter the source matrix:");
//    scanf(&v);
  
    node = start;
    //v = grid[];
  
    dijkstra_algorithm(node,v,cost,distance);
    sparki.println("n Shortest path:n");
    
    for(i=0;i<=node;i++)
        if(i!=v)
    printf("%d->%d,cost=%dn",v,i,distance[i]);
    
    //getch();
}
  
/*
//function 1 - "assigns a single number to each index of your 2D map"
int grid_to_graph(int x, int y){ //takes an x and y coordinate as input and returns a single number representing the graph node number
  int graph_j = (int) x/GRID_WIDTH; //grid_width and height defined above
  int graph_i = (int) y/GRID_HEIGHT;
  int graph_value = (4*graph_i)+graph_j; //multiply to find 0-15 value
  
  return graph_value;
}

//function 2 - "one that takes an index and returns the 2D coordinates"
int * graph_to_xyCoordinates(int graph_index){ //takes in the graph index and returns an array that acts as a tuple.
  float x_value = (graph_index%4)*GRID_WIDTH; //divide by four to get i value
  float y_value = (graph_index/4)*GRID_WIDTH; //modulo by four to get j
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
      distance[i][j]=infinity;
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
    return infinity;
  }
  else{
    int distance = abs(graph_index - graph_go_to); //if distance is 1 or 4, the spaces are adjacent
    if ((distance == 1) || (distance == 4)){ //check
      return 1; //return 1 for your distance as the grid is one space away
    }
    else{
      return infinity; //not adjacent, large amount to ensure we don't accidentally take that route.
    }
  }
}
*/



/* LAB 2: RETURNS THE POSITION OF THE SPARKI ROBOT
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
}
*/

