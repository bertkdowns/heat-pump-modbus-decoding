// This file defines which arduino pins are used for what.


// DOT MATRIX DISPLAY
#define SIN1 2
#define SIN2 3
#define SIN3 4
#define CLK 5 // note: changed clock pin
#define LATCH 6

// new button layout using shift registers
#define BUTTON_CLK 8
#define BUTTON_DATA 9
#define BUTTON_IN 10


// Which index on the shift register the button is
// so you can use button_state[BTN_LEFT_IDX] to get the state of that button.
#define BTN_LEFT_IDX 0
#define BTN_RIGHT_IDX 1
#define BTN_ROTATE_LEFT_IDX 2
#define BTN_ROTATE_RIGHT_IDX 3