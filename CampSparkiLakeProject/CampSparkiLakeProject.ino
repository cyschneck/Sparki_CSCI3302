#include <Sparki.h>

#define infinity 999

const int numRows=5;
const int numColumns=5;
bool campSparkiLake[numRows][numColumns]={
  {0, 0, 0, 0, 0}
  {0, 0, 1, 0, 0}
  {0, 1, 1, 0, 1}
  {0, 0, 0, 0, 0}
  {0, 0, 1, 0, 0}
};

byte startingPosition[2]={0, 1};
const byte homePosition[2]={startingPosition[0], startingPosition[1]};
byte currentPosition[2]={startingPosition[0], startingPosition[1]};
byte goalPosition[2]={0, 0};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  /*
   * State 1:
   *    Scan for light
   *    when light is found:
   *        orient sparki on light position (straight line from sparki front to light)
   *        determine light grid "general area" based on obstacles, sparki theta
   *        set goal node to closest grid space it could possibly be
   *        set state to state 2
   * State 2:
   *    If dijkstras hasn't been run:
   *        Run dijkstras to get path to goal node
   *    Else:
   *        Move towards goal node
   *        If at goal node:
   *            set state to state 3
   *        Else:
   *            move towards goal node
   * State 3:
   *    Preconditions -- You have reached the "goal" node
   *    Scan using ultrasound scanner for "victims"
   *    If victims found within x seconds:
   *        move towards victims
   *        target one victim in particular
   *        grab victim
   *        set state to state 4
   *    Else:
   *        "victims" have escaped
   *        set state to state 4
   * State 4:
   *     Return to home base
   *     set state to state 1
   */
}
