#define START 0
#define SEARCH 1
#define DONE 2

int state = START;

void loop(){
switch(state)

case START :
    movefoward;
    if (sensor > 20) state=SEARCH;
    break;

case SEARCH:
    if(sensor2 < 10)
    state=DONE;
    else
    state=START;
    break;
case DONE:
   stopwheels;
}

delay(100);
}
