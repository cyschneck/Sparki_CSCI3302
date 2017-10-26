/*
	Hey guys,
	Here's my code for flipping coordinates from Dijkstra's (bottom-left origin) to our odometry (top-left origin)
	Everything is ALMOST working (flippedGoTo is the array in our desired odometry coordinates), except for one strange bug with flippedGoTo[0] = 1 and flippedGoTo[1] = 0
	I'll try to take a look later this weekend or next week and update you all if I make any breakthroughs.
	--Max
*/

#include <stdio.h>
#include <math.h>


float maxspeed=0.0285;    // [m/s] speed of the robot that you measured
float alength=0.0851;     // [m] axle length  
float phildotr=0, phirdotr=0; // wheel speeds that you sent to the motors
float Xi=0, Yi=0, Thetai=0; // where the robot is in the world
float Xrdot, Thetardot;    // how fast the robot moves in its coordinate system

float Xg=0.1;     // Where the robot should go
float Yg=-0.1;
float Thetag=0;

float alpha, rho, eta; // error between positions in terms of angle to the goal, distance to the goal, and final angle
float a=0.1, b=1, c=0.1; // controller gains

// Setting up lab 3.2
// map of obstalces on the map
int grid[4][4]={  
   {0, 0, 0, 0} , /*  initializers for row indexed by 0 */
   {1, 1, 0, 1} , /*  initializers for row indexed by 1 */
   {0, 0, 0, 0} , /*  initializers for row indexed by 2 */
   {1, 0, 1, 0}   /*  initializers for row indexed by 3 */
};

int dist[16]; // distance to each node (cost)
int go_to[16];
int flippedGoTo[16]; // go_to[] flipped from Dijkstra's coordinates (bottom-left origin) to odometry coordinates (top-left origin)

int pos = 0; // robot position given as index on the graph
int goal= 13;// desired position


void flipCoordGrid() {
	// Flips the coordinate grid's rows from odometry (top-left origin) to Dijkstra's coordinates (bottom-left origin)
	// Assumes grid size of 4x4
	int tmp[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp[i][j] = grid[i][j];
			printf("tmp[%d][%d] = %d\n", i, j, tmp[i][j]);
		}
	}
	// Now, flip relevant rows
	for (int i = 0; i < 4; i++) {
		grid[0][i] = tmp[3][i];
		grid[3][i] = tmp[0][i];
		grid[1][i] = tmp[2][i];
		grid[2][i] = tmp[1][i];
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("grid[%d][%d] = %d\n", i, j, grid[i][j]);
		}
	}
}

int cost(int i,int j){
  int x1 = i / 4;
  int y1 = i % 4;
  int x2 = j / 4;
  int y2 = j % 4;

  if(i==j) return 0;
  
  if(grid[x1][y1] || grid[x2][y2]) // if obstacle
   return 99;

  int cost = fabs(x1-x2)+fabs(y1-y2);

  if(cost>1) 
   return 99;
  else
   return cost;
  
}

int flipCoordinates(int n) {
	// Changes from Dijkstra algorithm coordinates (origin = bottom-left) to odometry coordinates (origin = top-left)
	//printf("input n = %d\n", n);
	int row = n/4;
	//printf("%d/4 = %d\n", n, row);
	int newN = n + (12 - 8*(row));
	//printf("  flipped index: %d\n", newN);
	//return n + (12 - 8*(row));
	return newN;
}


void dij(int n,int v,int dist[])
// n = number of nodes v = goal node int dist[] = array to go to?
{
 int i,u,count,w,flag[16],min, wFlipped, uFlipped;
 printf("dij(%d, %d, dist[])\n", n, v);

 for(i=0;i<=n;i++)
  flag[i]=0, dist[i]=99; // initialize every node with cost-to-goal (infinity if not adjacent)
 dist[v]=0;
 count=1;
 while(count<=n)
 {
  min=99;
  for(w=0;w<=n;w++)                       // find node u with the current lowest score
   if(dist[w]<min && !flag[w])
    min=dist[w],u=w;
  flag[u]=1; // u is the node with the lowest score
  //Serial.print(count); Serial.print(": "); Serial.println(u);
    count++;
  for(w=0;w<=n;w++)
   if((dist[u]+cost(u,w)<dist[w]) && !flag[w]){
    dist[w]=dist[u]+cost(u,w);
    //go_to[wFlipped]=uFlipped; // store where to go when at w in order to reach goal with lowest cost
    go_to[w]=u; // store where to go when at w in order to reach goal with lowest cost
		flippedGoTo[flipCoordinates(w)] = flipCoordinates(u);
		
		printf("go_to[%d]: %d\n", w, go_to[w]);
		printf(" 'flipped': go_to[%d]: %d\n", flipCoordinates(w), flipCoordinates(go_to[w]));
   }
 }
}



void main() {
	//printf("Hello world\n");
	flipCoordGrid();
	dij(15,goal,dist); // calculate a map to go to node "13"
	for (int i = 0; i < 16; i++) {
		//flipCoordinates(i);
		printf("flippedGoTo[%d] = %d\n", i, flippedGoTo[i]);
	}
}
  
  
  
  
  /*
  void displayMapSpace(){
    sparki.clearLCD();
    int grid_width_pixel = 128/4
    int grid_height_pixel = 64/4;
    
    for(int i=0; i<4; i++){
      for (int j=0; j<4; j++){
          if(grid[i][j]==1){
              sparki.drawRectFilled(grid_width_pixel*4, grid_height_pixel*4, grid_width_pixel, grid_height_pixel);
            }
        }
    }
  }
  */
