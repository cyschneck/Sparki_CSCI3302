#include <Sparki.h>
float freq[] = {0.0, 32.703, 34.648, 36.708, 38.891, 41.203, 43.654, 46.249, 48.999, 51.913, 55.0, 58.270, 61.735};
float  octave[] = {2, 4, 8, 16, 32, 64, 128};
//               16th,  8th,  quarter,  half,  whole
float duration[] = {125,  250,  500,      1000,  2000};



float tempo = 200.0;
float JawsNotes[4] = {freq[5]*octave[0], freq[6]*octave[0], 0, -1};
int JawsDurations[4] = {duration[3]*(120/tempo), duration[2]*(120/tempo), duration[3]*(120/tempo), 0};

float JawsNotesFast[5] = {freq[5]*octave[0], 0, freq[6]*octave[0], 0, -1};
int JawsDurationsFast[5] = {duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo), 0};


int CSharp4 = int(freq[2]*octave[4]);
int FSharp3 = int(freq[7]*octave[3]);
int D4 = int(freq[3]*octave[4]);
int C4 = int(freq[1]*octave[4]);
int F3 = int(freq[6]*octave[3]);

/*
float HalloweenNotes[41] = {freq[2]*octave[4], freq[7]*octave[3], freq[7]*octave[3],
                          freq[2]*octave[4], freq[7]*octave[3], freq[7]*octave[3],
                          freq[2]*octave[4], freq[7]*octave[3], freq[3]*octave[4], freq[7]*octave[3],
                          freq[2]*octave[4], freq[7]*octave[3], freq[7]*octave[3],
                          freq[2]*octave[4], freq[7]*octave[3], freq[7]*octave[3],
                          freq[2]*octave[4], freq[7]*octave[3], freq[3]*octave[4], freq[7]*octave[3],
                          freq[1]*octave[4], freq[6]*octave[3], freq[6]*octave[4],
                          freq[1]*octave[4], freq[6]*octave[3], freq[6]*octave[4],
                          freq[1]*octave[4], freq[6]*octave[3], freq[2]*octave[4], freq[6]*octave[3],
                          freq[1]*octave[4], freq[6]*octave[3], freq[6]*octave[4],
                          freq[1]*octave[4], freq[6]*octave[3], freq[6]*octave[4],
                          freq[1]*octave[4], freq[6]*octave[3], freq[2]*octave[4], freq[6]*octave[3], -1};
*/

int HalloweenNotes[41] = {CSharp4, FSharp3, FSharp3, CSharp4, FSharp3, FSharp3, CSharp4, FSharp3, D4, FSharp3,
                            CSharp4, FSharp3, FSharp3, CSharp4, FSharp3, FSharp3, CSharp4, FSharp3, D4, FSharp3,
                            C4, F3, F3, C4, F3, F3, C4, F3, CSharp4, F3,
                            C4, F3, F3, C4, F3, F3, C4, F3, CSharp4, F3, -1};

int EighthNote = int(duration[0]*(120/tempo));

int HalloweenDurations[40] = {EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote,
                              EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote,
                              EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote,
                              EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote, EighthNote};

/*
int HalloweenDurations[40] = {duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo),
                          duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo),
                          duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo),
                          duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo),
                          duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo),
                          duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo),
                          duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo),
                          duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo),
                          duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo),
                          duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo),
                          duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo),
                          duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo), duration[1]*(120/tempo)};
*/

int nextTimer = int(HalloweenDurations[0]);    // The amount of time (ms) until state machine goes to next note
// Initialize at first index of array
int i = 0;


void setup() {

 
}

void loop() {
  sparki.clearLCD();
  /*
  if (sparki.ping() < 5) {
    sparki.noBeep();
    sparki.beep(note[10]*octave[3]);
  }
  */
  // STATE MACHINE: get to next note!  
  
  //sparki.print(nextTimer);
  // Updates every 100ms
  if (millis() % 100 == 0) {
  // If it's time for a new note, load it in
    if (nextTimer < 0) {
      // If at end of song, loop around
      sparki.noBeep();
      if (HalloweenNotes[i] < 0){
        i = 0;
      } else {
        i += 1;
      }
      nextTimer = HalloweenDurations[i];
    } else {
      nextTimer -= 100;
    }
    if ((HalloweenNotes[i] < 1)) {
      sparki.noBeep();
    } else {
      sparki.noBeep();
      //sparki.beep(HalloweenNotes[i]);  
    }
    
    //sparki.noBeep();
    //sparki.beep((int(freq[sparki.ping()/5])*octave[2]));
    //printf("note[%d]", sparki.ping()/10);
    
    sparki.updateLCD();
  }
}
