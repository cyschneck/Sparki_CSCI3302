
#include <Sparki.h>       // include the sparki library

// map of obstalces on the map
int grid[4][4]={  
   {0, 0, 0, 0} , /*  initializers for row indexed by 0 */
   {1, 1, 0, 1} , /*  initializers for row indexed by 1 */
   {0, 0, 0, 0} , /*  initializers for row indexed by 2 */
   {1, 0, 1, 0}   /*  initializers for row indexed by 3 */
};

void displayMapSpace(){
  sparki.clearLCD();
  int grid_width_pixel = 128/4;
  int grid_height_pixel = 64/4;
  //vertical lines
  for(int i=1; i<4; i++){
    sparki.drawLine((i*grid_width_pixel), 0, (i*grid_width_pixel), 63);
  }
  //horizontal lines
  for(int i=1; i<4; i++){
    sparki.drawLine(0, (i*grid_height_pixel), 127, (i*grid_height_pixel));
  }
  //rectangles for objects
  for(int i=0; i<4; i++){
    for (int j=0; j<4; j++){
      if(grid[i][j]==1){
        int start_width=(grid_width_pixel*j)+(((grid_width_pixel*j)/2)-1);
        int end_width=(grid_width_pixel*j)+(grid_width_pixel-1);
        int start_height=(grid_height_pixel*i)+(((grid_height_pixel*i)/2)-1);
        int end_height=(grid_height_pixel*i)+(grid_height_pixel-1);
        sparki.drawRectFilled(start_width, start_height, end_width, end_height);
      }
    }
  }
  sparki.updateLCD();
}

void setup() {
  // put your setup code here, to run once:
  displayMapSpace();
}

void loop() {
  // put your main code here, to run repeatedly:

}
