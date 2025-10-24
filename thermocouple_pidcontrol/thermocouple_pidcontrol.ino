 // Setpoint Trial with Real Thermocouple and Servo Control

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Adafruit_MAX31855.h>

// --- PI Constants ---
const int kp = 8;
const int ki = 9;
float PI_p = 0;
float PI_i = 0;
float PI_value = 0;

// --- Encoder Pins ---
#define outputA 2
#define outputB 3

// --- LCD Setup ---
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define LCD_REFRESH_TIME 1000 // ms

// --- VALVE SETUP
#define VALVE_MAX 255
#define VALVE_MIN 80
#define VALVE_PIN D6 // This needs an analog pwm output value which controls the valve position
#define PID_UPDATE_TIME 1000 // ms

// --- Thermocouple Pins ---
#define thermoDO   D12
#define thermoCS   D11
#define thermoCLK  D13
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);

// --- Variables ---
float setPoint = 25;
float realTemp = 25;
int aState;
int aLastState;
float error = 0;
unsigned long prevLCDRefreshTime = 0;
unsigned long prevPIDUpdateTime = 0;
// NOTE THIS WILL HAVE PROBLEMS IF YOU LEAVE IT ON FOR MORE THAN 50 DAYS AS IT WILL GO BACK ROUND TO ZERO!

void setup() {
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);

  // Rotary encoder initial state
  aLastState = digitalRead(outputA);

  // Attach the servo to pin 9
  pinMode(VALVE_PIN,OUTPUT);

  // Initialize thermocouple
  if (!thermocouple.begin()) {
    Serial.println("Thermocouple not found!");
    while (1);
  }
}



void loop() {
  // --- Rotary Encoder Handling ---
  aState = digitalRead(outputA);
  if (aState != aLastState) {
    //lcd.clear();
    if (digitalRead(outputB) != aState) {
      setPoint++;
    } else {
      setPoint--;
    }
    Serial.print("Setpoint: ");
    Serial.println(setPoint);
  }

  // --- Read actual temperature ---
  realTemp = thermocouple.readCelsius();

  unsigned long time = millis();

  if (prevPIDUpdateTime < time){
    prevPIDUpdateTime = PID_UPDATE_TIME + time;
    // --- PI Control ---
    error = setPoint - realTemp;
    if (abs(error) > 0.1) {
      PI_p = 0.1 * kp * error;
      PI_i = 0.1 * (PI_i + (ki * error));
      PI_value = PI_p + PI_i;

      // Clamp valve pos to between min and max values specified, and map from the pid range.
      // multiply by negative 1 for porportional control
      int servoPos = constrain(map((int)PI_value * -1 , -100, 100, VALVE_MIN, VALVE_MAX), VALVE_MIN, VALVE_MAX);
      analogWrite(VALVE_PIN,servoPos);

      Serial.print("Error: "); Serial.print(error);
      Serial.print(" Real: "); Serial.print(realTemp);
      Serial.print(" P: "); Serial.print(PI_p);
      Serial.print(" I: "); Serial.print(PI_i);
      Serial.print(" PI: "); Serial.print(PI_value);
      Serial.print(" Servo: "); Serial.println(servoPos);
    }
  }

  // --- LCD Output ---
  if (prevLCDRefreshTime < time){
    lcd.setCursor(0, 0);
    lcd.print("S:"); lcd.print((int)setPoint); lcd.print("   ");// to remove extra digits
    lcd.setCursor(8, 0);
    lcd.print("R:"); lcd.print(realTemp);lcd.print("   ");// to remove extra digits
    lcd.setCursor(0, 1);
    lcd.print("Error:"); lcd.print(error);lcd.print("   ");
    prevLCDRefreshTime = time + LCD_REFRESH_TIME;
  }

  aLastState = aState;
  //delay(200);  // To avoid bouncing and reduce update rate
}