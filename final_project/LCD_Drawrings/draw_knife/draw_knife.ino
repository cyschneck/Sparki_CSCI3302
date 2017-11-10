#include <Sparki.h>

void draw_knife(){
  sparki.clearLCD();
  //spine
  sparki.drawLine(27, 47, 79, 7);
  //point
  sparki.drawLine(27, 47, 35, 47);
  //blade
  sparki.drawLine(35, 47, 81, 19);
  //butt
  sparki.drawLine(81, 19, 73, 12);
  //hilt
  sparki.drawLine(79, 7, 81, 9);
  sparki.drawLine(81, 9, 75, 14);

  sparki.updateLCD();
}

void setup() {
  // put your setup code here, to run once:
  draw_knife();

}

void loop() {
  // put your main code here, to run repeatedly:

}
