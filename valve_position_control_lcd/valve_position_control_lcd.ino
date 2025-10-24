// -----
// LimitedRotator.ino - Example for the RotaryEncoder library.
// This class is implemented for use with the Arduino environment.
//
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD 3-Clause License. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 26.03.2017 created by Matthias Hertel
// 06.02.2021 conditions and settings added for ESP8266
// -----

// This example checks the state of the rotary encoder in the loop() function.
// The current position is printed on output when changed.
// In addition to the SimplePollRotator example here the range of the rotator is limited to the range 0 - 16 and only incremental steps of 2 are realized.
// To implement this limit the boundaries are checked and eventually the current position is adjusted.
// The internal (physical) position of the rotary encoder library remains by stepping with the increment 1
// so the the logical position is calculated by applying the ROTARYSTEPS factor.

// Hardware setup:
// Attach a rotary encoder with output pins to A2 and A3.
// The common contact should be attached to ground.

#include <Arduino.h>
#include <RotaryEncoder.h>

#define PIN_IN1 D2
#define PIN_IN2 D3


#define ROTARYSTEPS 2
#define ROTARYMIN 0
#define ROTARYMAX 255

#define VALVE_PIN D6

// Setup a RotaryEncoder with 4 steps per latch for the 2 signal input pins:
// RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::FOUR3);

// Setup a RotaryEncoder with 2 steps per latch for the 2 signal input pins:
RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);

// Last known rotary position.
int lastPos = -1;

void setup()
{
  Serial.begin(115200);
  while (! Serial);
  Serial.println("LimitedRotator example for the RotaryEncoder library.");
  encoder.setPosition(200 / ROTARYSTEPS); // start with the value of 200

  pinMode(VALVE_PIN,OUTPUT);
} // setup()


// Read the current position of the encoder and print out when changed.
void loop()
{
  encoder.tick();
 

  // get the current physical position and calc the logical position
  int newPos = encoder.getPosition() * ROTARYSTEPS;

  

  if (newPos < ROTARYMIN) {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos = ROTARYMIN;

  } else if (newPos > ROTARYMAX) {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos = ROTARYMAX;
  } 


  analogWrite(VALVE_PIN, newPos);

  if (lastPos != newPos) {
    Serial.print(newPos);
    Serial.println();
    lastPos = newPos;
  } // if
} // loop ()

// The End

