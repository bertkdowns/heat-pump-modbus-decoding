// This scorecounter counts in four digits from zero to nine,
// by turning off the dots on the dot matrix display.
// We index it by it's bottom right corner and it counts upwards from there.

#define SCORECOUNTER_X 15
#define SCORECOUNTER_Y 31

int score = 0;



void displayScore(){
    int temp_score = score;
    for (int i = 0; i < 4; i++) {
        int digit = temp_score % 10;
        temp_score /= 10;
        // Update the display for each digit
        for (int j = 0; j < 9; j++) {
            display_data[SCORECOUNTER_X - i][SCORECOUNTER_Y - j] = digit > j;
        }
    }
}

void updateScoreCounter(int s) {
    score += s;
    displayScore();
}