#ifndef COOLTUNE_H
#define COOLTUNE_H

#define AUDIO_PIN 9

// Notes (frequencies in Hz)
#define C4  262
#define D4  294
#define E4  330
#define F4  349
#define G4  392
#define A4  440
#define B4  494
#define C5  523

class CoolTuneClass {
  private:
    int melody[12]   = {C4,E4,G4,C5,B4,G4,E4,C4,D4,F4,A4,D4};
    int durations[12]= {200,200,300,300,200,200,300,300,200,200,300,300};
    int index = 0;

  public:
    void playStep() {
      if(index >= 12) index = 0; 

      tone(AUDIO_PIN, melody[index]);
      delay(durations[index]);
      noTone(AUDIO_PIN);
      delay(durations[index]/3); // short pause
      index++;
    }

    void reset() { index = 0; }
};

extern CoolTuneClass CoolTune;

#endif
