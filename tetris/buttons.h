#define BUTTON_CLK 8
#define BUTTON_DATA 9
#define BUTTON_IN 10

bool button_state[8] = {false};

void get_button_state(){
    // we read the button states by shifting a 1
    // through the button data line to enable one at a time,
    // and then read from the BUTTON_IN input.

    // Add the clock pulse with high
    digitalWrite(BUTTON_DATA,HIGH);
    digitalWrite(BUTTON_CLK, HIGH);
    digitalWrite(BUTTON_CLK,LOW);
    // Only add low now
    digitalWrite(BUTTON_DATA,LOW);
    for(int i = 0; i < 8; i++){
        digitalWrite(BUTTON_CLK,HIGH);
        button_state[i] = digitalRead(BUTTON_IN);
        digitalWrite(BUTTON_CLK,LOW);
    }
}

void print_button_state(){
    for(int i = 0; i < 8; i++){
        Serial.print("Button ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(button_state[i]);
    }
}