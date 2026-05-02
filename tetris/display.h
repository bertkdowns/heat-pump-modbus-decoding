#include "piece.h"
#include "pinout.h"

#define TETRIS_BOARD_X_START 1
#define TETRIS_BOARD_WIDTH 10

int update_time = 300;

void clearDisplay() {
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 32; y++) {
      display_data[x][y] = false;
    }
  }
}

// Shifts values to display_data and latches
void updateDisplay() {
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
      digitalWrite(SIN3, display_data[col_to_show][row_n]); // Write the appropriate data for this column
      digitalWrite(SIN2, display_data[col_to_show][row_n+16]); // Write the appropriate data for this column
      // pulse the clock
      digitalWrite(CLK, HIGH);
      // cpu delay is long enough
      digitalWrite(CLK, LOW);
    }
    digitalWrite(LATCH, HIGH);
    delayMicroseconds(10);
  }
}


void shiftRowsDown(int cleared_row) {
  for (int y = cleared_row; y > 0; y--) {
    for (int x = TETRIS_BOARD_X_START; x < TETRIS_BOARD_X_START + TETRIS_BOARD_WIDTH; x++) {
      display_data[x][y] = display_data[x][y - 1];
    }
  }
}


void clearRows() {
  for (int y = 0; y < 32; y++) {
    bool full_row = true;
    for (int x = TETRIS_BOARD_X_START; x < TETRIS_BOARD_X_START + TETRIS_BOARD_WIDTH; x++) {
      if (!display_data[x][y]) {
        full_row = false;
        break;
      }
    }
    if (full_row) {
      // Clear the row
      for (int x = TETRIS_BOARD_X_START; x < TETRIS_BOARD_X_START + TETRIS_BOARD_WIDTH; x++) {
        display_data[x][y] = false;
      }
      update_time = update_time * 0.9;
      shiftRowsDown(y);
    }
  }
}

