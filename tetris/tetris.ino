// #include <array>
#include "shuffle.h"
#include "display.h"
#include "buttons.h"
#include "buzzer.h"
int prev_time = 0;
int current_time = 0;

int time_since[4] = { 0, 0, 0, 0 };


TetrisPiece current_piece = TetrisPiece();
TetrisPiece hold_piece = TetrisPiece();
TetrisPiece next_piece = TetrisPiece();

void handleButtons(){
  current_time = millis();
  getButtonState();
  printButtonState();
  if (button_state[BTN_LEFT_IDX] && current_time - time_since[0] > BTN_DEBOUNCE_TIME) {
    time_since[0] = current_time;
    current_piece.moveLeft();
  }
  if (button_state[BTN_RIGHT_IDX] && current_time - time_since[1] > BTN_DEBOUNCE_TIME) {
    time_since[1] = current_time;
    current_piece.moveRight();
  }
  if (button_state[BTN_ROTATE_LEFT_IDX] && current_time - time_since[2] > BTN_DEBOUNCE_TIME * 3) {
    time_since[2] = current_time;
    current_piece.tryRotate();
  }
  if (button_state[BTN_SPEED_DOWN_IDX] && current_time - time_since[3] > BTN_DEBOUNCE_TIME) {
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
  current_piece.x = 5;
  current_piece.y = 8;

  current_piece.switchPiece(T);
  hold_piece.x = 11;
  hold_piece.y = 6;
  hold_piece.switchPiece(T);
  hold_piece.hide();
  next_piece.switchPiece(chooseRandomPiece());
  next_piece.x = 11;
  next_piece.y = 1;
  next_piece.hide(); // display sillhouette
}

void loop() {
  int current_time = millis();
  if (current_time - prev_time >= update_time) {
    updateGame();
    prev_time = current_time;
  }
  handleButtons();
  buzzerLoop();
  
  updateDisplay();
}

void switchHeldPiece(){
  hold_piece.show();
  current_piece.hide();
  int tmp = hold_piece.type;
  hold_piece.switchPiece(current_piece.type);
  current_piece.switchPiece(tmp);
  hold_piece.hide(); // displays the sillhouette
  current_piece.show(); // displays the new current piece
}

void getNewPiece(){
  current_piece.switchPiece(next_piece.type);
  // update the display of the next piece
  next_piece.show();
  next_piece.switchPiece(chooseRandomPiece());
  next_piece.hide(); // displays the sillhouette  
}

void updateGame() {
  //printButtonState();
  current_piece.hide(); // hide so it doesn't collide with itself
  if(current_piece.isColliding(current_piece.x, current_piece.y + 1)){
    // We leave the piece there and don't get rid of it.
    // Start with a new piece.
    current_piece.show();
    clearRows();
    getNewPiece();
    updateScoreCounter(1);
    current_piece.y = 0;
    current_piece.x = 7;
  }
 current_piece.y = current_piece.y + 1;
 current_piece.show();
}

