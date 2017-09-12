#include <Sparki.h> // include sparki library

void setup() {
  sparki.servo (SERVO_CENTER);
  sparki.clearLCD();
}

// /------^-----\
// |            |
// | 69  70  71 |
// | 68  64  67 |
// |  7  21   9 |
// | 22  25  13 |
// | 12  24  94 |
// |  8  28  90 |
// | 66  82  74 |
// \____________/

void defaultLineFollowing() {
  // line following from examples
  int threshold = 700;
  
  int lineLeft   = sparki.lineLeft();   // measure the left IR sensor
  int lineCenter = sparki.lineCenter(); // measure the center IR sensor
  int lineRight  = sparki.lineRight();  // measure the right IR sensor

  if ( lineCenter < threshold ) // if line is below left line sensor
  {  
    sparki.moveForward(); // move forward
  }
  else{
    if ( lineLeft < threshold ) // if line is below left line sensor
    {  
      sparki.moveLeft(); // turn left
    }
  
    if ( lineRight < threshold ) // if line is below right line sensor
    {  
      sparki.moveRight(); // turn right
    }
  }

  sparki.clearLCD(); // wipe the screen
  
  sparki.print("Line Left: "); // show left line sensor on screen
  sparki.println(lineLeft);
  
  sparki.print("Line Center: "); // show center line sensor on screen
  sparki.println(lineCenter);
  
  sparki.print("Line Right: "); // show right line sensor on screen
  sparki.println(lineRight);
  
  sparki.updateLCD(); // display all of the information written to the screen

  delay(100); // wait 0.1 seconds
}
void moveLeftGran(int dist) {
  sparki.moveLeft(300);
  sparki.moveRight(200);
}

int readRemote() {
  int code = sparki.readIR();
   if (code != -1)
   {
    sparki.print("Code received in function: ");
    sparki.println(code);
    return code;
   }
}

void loop() {
  // put your main code here, to run repeatedly:
   defaultLineFollowing();
   int code = sparki.readIR();

   if (code != -1){
    sparki.print("Received code: ");
    sparki.println(code);
   }

   switch(code){
    //Start program
    case 28:  sparki.moveLeft(); break;
    case 64:  sparki.moveStop(); break;
    //case 24:  moveLeftInfinite(); break;
   }
  sparki.updateLCD();
}
