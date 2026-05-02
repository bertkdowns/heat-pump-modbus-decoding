#include "pinout.h"

#define BTN_DEBOUNCE_TIME 100


bool button_state[8] = {false};

void setupButtons(){
    Serial.begin(9600);
    pinMode(BUTTON_CLK, OUTPUT);
    pinMode(BUTTON_DATA, OUTPUT);
    pinMode(BUTTON_IN, INPUT); // Listen for shift register signal
    Serial.println("System Ready: Scanning 8 Buttons...");
}

void getButtonState(){
    // We read the button states by shifting a 1
    // through the button data line to enable one at a time,
    // and then read from the BUTTON_IN input.

    // Shift in a 1 and then set data low
    digitalWrite(BUTTON_DATA, HIGH);
    digitalWrite(BUTTON_CLK, HIGH);
    digitalWrite(BUTTON_CLK, LOW);
    digitalWrite(BUTTON_DATA, LOW);

    for(int i = 0; i < 8; i++){
        digitalWrite(BUTTON_CLK,HIGH);
        bool data = digitalRead(BUTTON_IN);
        button_state[i] = data;
        digitalWrite(BUTTON_CLK,LOW);
    };
    digitalWrite(BUTTON_CLK,HIGH);
    digitalWrite(BUTTON_CLK,LOW);
    digitalWrite(BUTTON_CLK,HIGH);
    digitalWrite(BUTTON_CLK,LOW);
    digitalWrite(BUTTON_CLK,HIGH);
    digitalWrite(BUTTON_CLK,LOW);
}

// Button debugging logic: uses a small part of the display to tell us what buttons are being pressed.
#define BUTTONX 13
#define BUTTONY 14

void printButtonState(){
    for(int i = 0; i < 8; i++){
       display_data[BUTTONX][BUTTONY + i] = button_state[i];
    }
}
