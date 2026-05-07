
int last_note_played = 0; // time the last note was played
int current_note = 0;


/*
  Melody with Struct (note + duration together)

  Plays a melody

  circuit:
  - 8 ohm speaker on digital pin 13
*/

#include "pitches.h"

// Define a struct to hold both pitch and duration
struct Note {
  int pitch;
  float duration; // 1 = whole, 2 = half, 4 = quarter, etc.
};

// Melody (pitch + duration together)
Note melody[] = {
  //////// BARS 1-4 ////////
  {NOTE_E4, 1},

  {NOTE_B3, 0.5},
  {NOTE_C4, 0.5},

  {NOTE_D4, 0.5},
  {NOTE_E4, 0.25},
  {NOTE_D4, 0.25},

  {NOTE_C4, 0.5},
  {NOTE_B3, 0.5},

  // ----- //

  {NOTE_A3, 1},

  {NOTE_A3, 0.5},
  {NOTE_B3, 0.5},

  {NOTE_E4, 1},

  {NOTE_D4, 0.5},
  {NOTE_C4, 0.5},

  // ----- //

  {NOTE_B3, 1},

  {NOTE_B3, 0.5},
  {NOTE_C4, 0.5},
  
  {NOTE_D4, 1},

  {NOTE_E4, 1},
  
  // ----- //
  {NOTE_C4, 1},
  {NOTE_A3, 1},

  {NOTE_A3, 2},

  //////// BARS 5-8 ////////
  {REST, 0.5},
  {NOTE_D4, 0.5},
  {NOTE_D4, 0.5},
  {NOTE_F4, 0.5},

  {NOTE_A4, 1},

  {NOTE_G4, 0.5},
  {NOTE_F4, 0.5},

  // ----- //

  {NOTE_E4, 1.5},

  {NOTE_B3, 0.5},

  {NOTE_C4, 1},

  {NOTE_D4, 0.5},
  {NOTE_C4, 0.5},

  // ----- //

  {NOTE_B3, 1},

  {NOTE_B3, 0.5},
  {NOTE_C4, 0.5},

  {NOTE_D4, 1},

  {NOTE_E4, 1},

  // ----- //

  {NOTE_C4, 1},
  {NOTE_A3, 1},

  {NOTE_A3, 2}
};

// Calculate melody length
float BPM = 148;
int melody_length = sizeof(melody) / sizeof(melody[0]);
float beatDuration = 60000.0 / BPM;

void buzzerLoop() {
    float noteDuration = melody[current_note].duration * beatDuration;
    int time = millis();
    if (time - last_note_played >= noteDuration  || last_note_played > time) { // second case is for reset clock
        // on to the next note
        current_note = (current_note + 1) % melody_length;
        last_note_played = millis();
        noTone(BUZZER_PIN);
        tone(BUZZER_PIN, melody[current_note].pitch, noteDuration * 0.9);
    }
}