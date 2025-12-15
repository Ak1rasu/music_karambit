#ifndef SOUNDS_H
#define SOUNDS_H

// Pin is defined in main sketch
// Example notes (frequency in Hz)
const int NOTE_C4  = 262;
const int NOTE_D4  = 294;
const int NOTE_E4  = 330;
const int NOTE_F4  = 349;
const int NOTE_G4  = 392;
const int NOTE_A4  = 440;
const int NOTE_B4  = 494;
const int NOTE_C5  = 523;

// Example melody (sequence of notes)
const int melody[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5
};

// Duration of each note (ms)
const int noteDurations[] = {
  200, 200, 200, 200,
  200, 200, 200, 400
};

#endif
