#define CLK 3
#define SIN1 4
#define SIN2 5
#define SIN3 6
#define LATCH 7

bool display_data[16][32] = {false}; // x;y or col;row


void setup() {
  Serial.begin(4800);   // make sure this matches the sender
  pinMode(CLK, OUTPUT);
  pinMode(SIN1, OUTPUT);
  pinMode(SIN2, OUTPUT);
  pinMode(SIN3, OUTPUT);
  pinMode(LATCH,OUTPUT);
  digitalWrite(LATCH,HIGH);
  display_data[0][0] = true;
  display_data[4][1] = true;
  display_data[8][2] = true;
  display_data[12][24] = true;
}

void loop() {
  
  update_display();
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