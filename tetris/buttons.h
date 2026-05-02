#define BUTTON_CLK 8
#define BUTTON_DATA 9
#define BUTTON_IN 10

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
        button_state[i] = digitalRead(BUTTON_IN);
        digitalWrite(BUTTON_CLK,LOW);
    };
    digitalWrite(BUTTON_CLK,HIGH);
    digitalWrite(BUTTON_CLK,LOW);
    digitalWrite(BUTTON_CLK,HIGH);
    digitalWrite(BUTTON_CLK,LOW);
    digitalWrite(BUTTON_CLK,HIGH);
    digitalWrite(BUTTON_CLK,LOW);
}

void printButtonState(){
    for(int i = 0; i < 8; i++){
      if( button_state[i]){
                Serial.print("Button ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(button_state[i]);
    }
    }
}
