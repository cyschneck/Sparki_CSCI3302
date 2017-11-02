#define NO_LCD 
#include  <Sparki.h>; // include the sparki library 


// strcuture of numAcceptedValues from: https://github.com/asgaines/Robots/
//globals for valuse
int array_1[100]; //for object 1 (r_1)
int array_2[100]; //for object 2 (r_2)
int sum_1=0;
int sum_2=0;

float L = 0.1; // Distance between beacons (fixed)
int startAngle=-30;
int angle = startAngle;
int endAngle = 30;
int counter = 0;
int distances[60]; // 60 = end angle - start angle = one reading per degree
int numAcceptedValues;
int acceptedValues[60][2]; // Store the distance (degree) as well as the angle
float mean, variance;

// distances between two beacons (r1 and r2)
int numr1Readings, numr2Readings = 0;
int r1Readings[60];
int r2Readings[60];
int r1avg, r2avg = 0; // Distance from r1/r2 beacon 

// Sparki's position
float x, y = 0.0;

///////////////////////////////////////////////
int targetDistance = 10; 
////////////////////////////////////////////// change for 10, 20, 30, 40

void setup() {
} 

void loop() { 
  sparki.servo(angle);
  counter++;
  angle=angle+1; 
  
  // measures the distance with Sparki's eyes 
  int cm = sparki.ping();
  distances[counter] = cm;
  
  // Only accept the values within 10cm of target
  // Second reading lies......
  if (cm > targetDistance - 10 && cm < targetDistance + 10) {
    acceptedValues[numAcceptedValues][0] = cm;
    acceptedValues[numAcceptedValues][1] = angle;
    numAcceptedValues++;
  }

  if(angle>endAngle){ 
    angle=startAngle;
    counter = 0;

    for (int index = 0; index < 60; index++) {
      Serial.println(distances[index]);
    }

    // Scan through distance measurements, sort into r1, r2 arrays
    for (int index = 2; index < 60; index++) { // Ignore first 2 inaccurate values
      while (distances[index] == -1) {} // Ignore leading -1 values
      while (distances[index] != -1) {
        r1Readings[numr1Readings] = distances[index];
        numr1Readings++;
      }
      while (distances[index] == -1) {} // Ignore -1 values between r1, r2
      while (distances[index] != -1) {
        r2Readings[numr2Readings] = distances[index];
        numr2Readings++;
      }
      break; // No need to check any more values beyond r2
    }

    // r1 readings
    for (int index = 0; index < numr1Readings; index++) {
      r1avg += r1Readings[index];
    }
    r1avg = r1avg / numr1Readings;

    // r2 readings
    for (int index = 0; index < numr2Readings; index++) {
      r2avg += r2Readings[index];
    }
    r2avg = r2avg / numr2Readings;

    // Calculate Sparki's x, y coordinates
    x = ((L * L) + (r1avg * r1avg) - (r2avg * r2avg));
    x = x / (2 * L);
    y = (r1avg * r1avg) - (x * x);
    y = sqrt(y)

    sparki.clearLCD();
    sparki.print("x = ");
    sparki.println(x);
    sparki.print("y = ");
    sparki.println(y);
    sparki.updateLCD();
    
    for (int index = 0; index < 60; index++) {
      Serial.print(distances[index]);
      Serial.print(", ");
    }
    Serial.println();

    sparki.clearLCD();

    for (int reading = 0; reading < numAcceptedValues; reading++) {
      Serial.print("Distance: ");
      sparki.print(acceptedValues[reading][0]);
      sparki.print(" ");
      Serial.print(", angle: ");
      Serial.println(acceptedValues[reading][1]);
      mean = mean + acceptedValues[reading][0];
    }
    sparki.println();
    mean = mean / numAcceptedValues;
    sparki.print("Mean = ");
    sparki.println(mean);

    for(int reading = 0; reading < numAcceptedValues; reading++) {
      variance = variance + (acceptedValues[reading][0] - mean) * (acceptedValues[reading][0] - mean); 
    }
    variance = variance / numAcceptedValues;

    sparki.print("Variance = ");
    sparki.println(variance);
    sparki.updateLCD();

    numAcceptedValues = 0;    
  }  
} 
