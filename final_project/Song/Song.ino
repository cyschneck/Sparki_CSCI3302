struct Song {
  int notes[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12];
  int octave[0, 1, 2, 3, 4, 5, 6];
  string duration = ["sixteenth", "eighth", "quarter", "half", "whole"];
  float tempo = 120.0;
}

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:
  sparki.beep(10);
}
