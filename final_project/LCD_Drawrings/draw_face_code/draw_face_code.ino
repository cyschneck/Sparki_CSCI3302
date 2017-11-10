#include <Sparki.h>

void draw_face(){
  sparki.clearLCD();
  //mask outline
  sparki.drawLine(51, 7, 75, 7); //top
  sparki.drawLine(51, 7, 39, 15); //diagonals to side
  sparki.drawLine(75, 7, 87, 15);
  sparki.drawLine(39, 15, 39, 35); //sides
  sparki.drawLine(87, 15, 87, 35);
  sparki.drawLine(39, 35, 59, 55); //diagonals to chin
  sparki.drawLine(87, 35, 67, 55);
  sparki.drawLine(59, 55, 67, 55); //chin
  //mask featrues
  //mouth
  sparki.drawLine(59, 43, 67, 43);
  sparki.drawLine(59, 43, 55, 47);
  sparki.drawLine(55, 47, 71, 47);
  sparki.drawLine(67, 43, 71, 47);
  //nose
  sparki.drawLine(61, 31, 65, 31);
  sparki.drawLine(61, 31, 59, 35);
  sparki.drawLine(63, 31, 61, 35);
  sparki.drawLine(63, 31, 65, 35);
  sparki.drawLine(59, 35, 61, 35);
  sparki.drawLine(65, 35, 67, 35);
  sparki.drawLine(65, 31, 67, 35);
  //eyes
  sparki.drawLine(47, 19, 59, 19);
  sparki.drawLine(47, 23, 59, 23);
  sparki.drawLine(59, 19, 59, 23);
  sparki.drawLine(47, 19, 47, 23);
  sparki.drawLine(67, 19, 67, 23);
  sparki.drawLine(67, 19, 79, 19);
  sparki.drawLine(67, 23, 79, 23);
  sparki.drawLine(79, 19, 79, 23);
  //draw
  sparki.updateLCD();
}

void setup() {
  // put your setup code here, to run once:
  draw_face();
}

void loop() {
  // put your main code here, to run repeatedly:

}
