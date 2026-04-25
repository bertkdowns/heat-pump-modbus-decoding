// #include <array>
#include "piece.h"
#include "shuffle.h"
#define CLK 3
#define SIN1 4
#define SIN2 5
#define SIN3 6
#define LATCH 7

#define BTN_LEFT 8
#define BTN_RIGHT 9
#define BTN_ROTATE_LEFT 10
#define BTN_ROTATE_RIGHT 11
#define BTN_DEBOUNCE_TIME 100

int update_time = 100;
int prev_time = 0;
int current_time = 0;

#define T 0
#define I 1
#define O 2
#define L 3
#define J 4
#define S 5
#define Z 6

int time_since[4] = { 0, 0, 0, 0 };


TetrisPiece current_piece = TetrisPiece();

void handle_buttons(){
  current_time = millis();
  if (digitalRead(BTN_LEFT) == LOW && current_time - time_since[0] > BTN_DEBOUNCE_TIME) {
    time_since[0] = current_time;
    current_piece.moveLeft();
  }
  if (digitalRead(BTN_RIGHT) == LOW && current_time - time_since[1] > BTN_DEBOUNCE_TIME) {
    time_since[1] = current_time;
    current_piece.moveRight();
  }
  if (digitalRead(BTN_ROTATE_LEFT) == LOW && current_time - time_since[2] > BTN_DEBOUNCE_TIME) {
    time_since[2] = current_time;
    current_piece.tryRotate();
  }
  if (digitalRead(BTN_ROTATE_RIGHT) == LOW && current_time - time_since[3] > BTN_DEBOUNCE_TIME) {
    time_since[3] = current_time;
    current_piece.tryRotate();
  }
}



void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(SIN1, OUTPUT);
  pinMode(SIN2, OUTPUT);
  pinMode(SIN3, OUTPUT);
  pinMode(LATCH,OUTPUT);
  digitalWrite(LATCH,HIGH);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_ROTATE_LEFT, INPUT_PULLUP);
  pinMode(BTN_ROTATE_RIGHT, INPUT_PULLUP);
  current_piece.x = 5;
  current_piece.y = 8;

  current_piece.switchPiece(T);
}

void loop() {
  int current_time = millis();
  if (current_time - prev_time >= update_time) {
    update_game();
    prev_time = current_time;
  }
  handle_buttons();
  
  update_display();
}

void update_game() {
  current_piece.hide(); // hide so it doesn't collide with itself
  if(current_piece.is_colliding(current_piece.x, current_piece.y + 1)){
    // We leave the piece there and don't get rid of it.
    // Start with a new piece.
    current_piece.show();
    current_piece.switchPiece(chooseRandomPiece());
    current_piece.y = 0;
  }
 current_piece.y = current_piece.y + 1;
 current_piece.show();
}

void clear_display() {
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 32; y++) {
      display_data[x][y] = false;
    }
  }
}

// Shifts values to display_data and latches
void update_display() {
  // SIN1 controls the columns (x)
  // SIN2 controls the top rows (y)
  // SIN3 controls the bottom rows;
  // We have to print one row at a time becuase it's a dot matrix display_data
  
  for (int col_to_show = 0; col_to_show < 16; col_to_show++) {
    digitalWrite(LATCH, LOW);
    for(int row_n = 0; row_n < 16; row_n ++){
      if (row_n == col_to_show){
        // enable this row
        digitalWrite(SIN1, HIGH);
      } else {
        digitalWrite(SIN1, LOW);
      }
      digitalWrite(SIN2, display_data[col_to_show][row_n]); // Write the appropriate data for this column
      digitalWrite(SIN3, display_data[col_to_show][row_n+16]); // Write the appropriate data for this column
      // pulse the clock
      digitalWrite(CLK, HIGH);
      // cpu delay is long enough
      digitalWrite(CLK, LOW);
    }
    digitalWrite(LATCH, HIGH);
    delayMicroseconds(10);
  }
}






// Teris grid:
/*
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX

XXXXXXXXXXXXXXXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
XXX          XXX
*/