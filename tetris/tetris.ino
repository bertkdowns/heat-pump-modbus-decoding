// #include <array>
#include "shuffle.h"
#include "buttons.h"
#include "display.h"

int prev_time = 0;
int current_time = 0;

// Tetris pieces
#define T 0
#define I 1
#define O 2
#define L 3
#define J 4
#define S 5
#define Z 6

int time_since[4] = { 0, 0, 0, 0 };


TetrisPiece current_piece = TetrisPiece();
TetrisPiece hold_piece = TetrisPiece();
// store the hold piece number
void handleButtons(){
  current_time = millis();
  if (digitalRead(BTN_LEFT) == LOW && current_time - time_since[0] > BTN_DEBOUNCE_TIME) {
    time_since[0] = current_time;
    current_piece.moveLeft();
  }
  if (digitalRead(BTN_RIGHT) == LOW && current_time - time_since[1] > BTN_DEBOUNCE_TIME) {
    time_since[1] = current_time;
    current_piece.moveRight();
  }
  if (digitalRead(BTN_ROTATE_LEFT) == LOW && current_time - time_since[2] > BTN_DEBOUNCE_TIME * 3) {
    time_since[2] = current_time;
    current_piece.tryRotate();
  }
  if (digitalRead(BTN_ROTATE_RIGHT) == LOW && current_time - time_since[3] > BTN_DEBOUNCE_TIME) {
    time_since[3] = current_time;
    updateGame();
  }
}



void setup() {
  setupButtons();
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
  hold_piece.x = 0;
  hold_piece.y = 0;
  hold_piece.hide();
}

void loop() {
  int current_time = millis();
  if (current_time - prev_time >= update_time) {
    updateGame();
    prev_time = current_time;
  }
  handleButtons();
  
  updateDisplay();
}

void switchHeldPiece(){
  TetrisPiece tmp = current_piece;
}

void updateGame() {
  getButtonState();
  //printButtonState();
  current_piece.hide(); // hide so it doesn't collide with itself
  if(current_piece.isColliding(current_piece.x, current_piece.y + 1)){
    // We leave the piece there and don't get rid of it.
    // Start with a new piece.
    current_piece.show();
    clearRows();
    current_piece.switchPiece(chooseRandomPiece());
    current_piece.y = 0;
    current_piece.x = 7;
  }
 current_piece.y = current_piece.y + 1;
 current_piece.show();
}

