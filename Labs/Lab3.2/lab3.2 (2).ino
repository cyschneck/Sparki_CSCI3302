/************************************************************
* CSCI3302 Introduction to Robotics
* (c) Nikolaus Correll
*
* Lab 3.2: Path Planning
*
***************************************************************/


#include <Sparki.h>       // include the sparki library


int grid[4][4]={  
   {0, 0, 0, 0} , /*  initializers for row indexed by 0 */
   {1, 1, 0, 1} , /*  initializers for row indexed by 1 */
   {0, 0, 0, 0} , /*  initializers for row indexed by 2 */
   {1, 0, 1, 0}   /*  initializers for row indexed by 3 */
};

int dist[16];
int go_to[16];

int pos = 0; // robot position given as index on the graph
int goal= 13;// desired position


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

void dij(int n,int v,int dist[])
{
 int i,u,count,w,flag[16],min;

 for(i=0;i<=n;i++)
  flag[i]=0, dist[i]=99; // initialize every node with cost-to-goal (infinity if not adjacent)
 dist[goal]=0;
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
    go_to[w]=u; // store where to go when at w in order to reach goal with lowest cost
     //Serial.print("Found: "); Serial.print(w); Serial.print("->"); Serial.println(u);
   }
 }
}



void setup() 
{
  dij(15,goal,dist); // calculate a map to go to node "13"
  Serial.println("Starting...");

}

void loop() {

    dij(15,goal,dist); // calculate a map to go to node "13"
 
    int i;
  /*for(i=0;i<16;i++){
      if(i%4==0) Serial.println();
    if(dist[i]!=99) 
     Serial.print(dist[i]);
    else
     Serial.print("#");
    Serial.print(" ");
  }
  Serial.println();*/

  for(i=0;i<16;i++){
         if(dist[i]!=99) 
          Serial.print(go_to[i]);
         else
          Serial.print("#");
        Serial.print(" ");
        if((i+1)%4==0) Serial.println();
  }
  Serial.println();

  
/*
   pos=go_to[pos];
   Serial.print("Driving to ");
   Serial.println(pos);
   //if(pos==goal) pos=0;
  */
}

/* 
 *  
*/

