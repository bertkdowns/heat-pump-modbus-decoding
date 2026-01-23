/*
Simple UART Control Protocol
write an arduino script which reads uart data in this format: 
first byte = always 0x00 second byte = 0x01 is digitalread 0x02 is analogRead,
0x03 is digitalwrite, 0x04 is analogWrite 
third byte (if digitalwrite or analogwrite) is value 0 or 1 for digital or 0-255 for analog 
a 5ms delay between reading on the serial clears the serial buffer. 
if a read command is recieved, one byte of digital or analog data is returned over uart. 
also write a library of python functions to run these. 
Make sure to also handle switching the pins to the correct pinmode.
*/

#define BAUDRATE 9600

#define CMD_DIGITAL_READ  0x01
#define CMD_ANALOG_READ   0x02
#define CMD_DIGITAL_WRITE 0x03
#define CMD_ANALOG_WRITE  0x04

unsigned long lastByteTime = 0;
uint8_t buffer[4];
uint8_t index = 0;

enum PinModeState {
  MODE_UNKNOWN,
  MODE_INPUT,
  MODE_OUTPUT,
  MODE_PWM
};


// store the current mode of all the pins so we only have to update if it changes.
// not sure if this is necessary or not tbh, arduino might do this automatically.
#define NUM_PINS 20
PinModeState currentMode[20];

void setDigitalInput(uint8_t pin) {
  if (currentMode != PinModeState.MODE_INPUT) {
    pinMode(pin, INPUT);
    currentMode = MODE_INPUT;
  }
}

void setDigitalOutput(uint8_t pin) {
  if (currentMode != PinModeState.MODE_OUTPUT) {
    pinMode(pin, OUTPUT);
    currentMode = PinModeState.MODE_OUTPUT;
  }
}

void setPWMOutput(uint8_t pin) {
  if (currentMode != PinModeState.MODE_PWM) {
    pinMode(pin, OUTPUT);
    currentMode = PinModeState.MODE_PWM;
  }
}

void setup() {
  Serial.begin(BAUDRATE);
  for(int i = 0; i < NUM_PINS; i++){
    currentMode[i] = PinModeState.MODE_UNKNOWN
  }
}

void loop() {
  unsigned long now = millis();

  // Clear buffer if gap > 5ms
  if (index > 0 && (now - lastByteTime) > 5) {
    index = 0;
  }
  // avoid buffer overflows
  if (index >= 4) {
    index = 0
  }

  while (Serial.available()) {
    uint8_t b = Serial.read();
    lastByteTime = millis();

    buffer[index++] = b;

    if (index == 1 && buffer[0] != 0x00) {
      // got an invalid something, just ignore.
      index = 0;
      return;
    }


    // Read commands (3 bytes) (0 byte, function, pin)
    if (index == 3 &&
        (buffer[1] == CMD_DIGITAL_READ ||
         buffer[1] == CMD_ANALOG_READ)) {

      handleCommand(buffer[1], buffer[2], 0); // no value for analog read
      index = 0;
    }

    // Write commands (4 bytes) (0 byte, function, pin, value)
    if (index == 4 &&
        (buffer[1] == CMD_DIGITAL_WRITE ||
         buffer[1] == CMD_ANALOG_WRITE)) {

      handleCommand(buffer[1], buffer[2],buffer[3]);
      index = 0;
    }
  }
}

void handleCommand(uint8_t cmd,uint8_t pin, uint8_t value) {
  switch (cmd) {

    case CMD_DIGITAL_READ: {
      setDigitalInput(pin);
      uint8_t v = digitalRead(pin) ? 1 : 0;
      Serial.write(v);
      break;
    }

    case CMD_ANALOG_READ: {
      setDigitalInput(pin);
      int v = analogRead(pin);
      Serial.write(map(v, 0, 1023, 0, 255));
      break;
    }

    case CMD_DIGITAL_WRITE: {
      setDigitalOutput(pin);
      digitalWrite(PIN, value ? HIGH : LOW);
      Serial.write(0)
      break;
    }

    case CMD_ANALOG_WRITE: {
      setPWMOutput(pin);
      analogWrite(PIN, value);
      Serial.write(0)
      break;
    }
  }
}
