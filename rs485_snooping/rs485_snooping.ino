// Based on https://github.com/jheyman/RS485Logger/blob/master/RS485Logger_sender/RS485Logger_sender.ino

// https://jheyman.github.io/blog/pages/RS485Sniffer/
#include <SPI.h>

#include <MsTimer2.h>
#include <SD.h>
#define MESSAGE_GAP_TIMEOUT_IN_MS 5



byte RecvBuffer[700];
#define SD_CS 10

unsigned int recvIndex = 0;
unsigned long msgIndex = 0;
unsigned long receiveTime = 0;

char receiveTimeString[32];
char msgIndexString[32];

const byte LED_PIN = 13;
File logFile;

#define FILE_NAME "datalogs.csv"

void setup() {

  Serial.begin(4800);  // Baud rate of modbus

  // set an end-of-message detection timeout of 5ms
  MsTimer2::set(MESSAGE_GAP_TIMEOUT_IN_MS, onTimer);

pinMode(LED_BUILTIN, OUTPUT);
  // Setup SD Card for logging
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card failed or not present!");
    while (1)
      ;
  }

  logFile = SD.open(FILE_NAME, FILE_WRITE);
  if (logFile && logFile.size() == 0) {
    // Header row
    logFile.println("UART Data");
  }
  logFile.close();
}

bool dumpData = false;

// If this timer expires, this means no additional character was received for a while: notify main loop
void onTimer() {
  dumpData = true;
}

void loop() {
  char received;

  if (Serial.available() > 0) {
    received = Serial.read();
    RecvBuffer[recvIndex++] = received;
    // Reset the timer
    MsTimer2::stop();
    MsTimer2::start();
  }

  // If the timer expired and positioned this var, we should now dump the received message
  // into a UDP packet to the remote host/logger.
  if (dumpData) {
    receiveTime = micros() - MESSAGE_GAP_TIMEOUT_IN_MS * 1000;

    // reinitialize vars for next detection/dump
    dumpData = false;
    MsTimer2::stop();
    msgIndex++;
      if (msgIndex > 999) msgIndex = 0;

    logFile = SD.open(FILE_NAME, FILE_WRITE);
    if (!logFile) {
      digitalWrite(LED_BUILTIN, HIGH);  // LED is on if SD card error
    } else {

      digitalWrite(LED_BUILTIN,LOW);

      logFile.print(receiveTime); logFile.print(",");
      logFile.print(msgIndex); logFile.print(",");
      
      // Write the buffer as hex
      // Write each byte as 2 hex chars
      for (size_t i = 0; i < recvIndex; i++) {
        int b = (uint8_t) RecvBuffer[i]; // convert to int so we don't have to worry about sign stuff
        if (b < (int) 0x10){ logFile.print("0");} // pad single hex digits
        logFile.print(b, HEX);
        logFile.print(" "); // optional spacing
        //Serial.print(b);
        //Serial.print(" ");
      }
      //Serial.println();
      logFile.println();

    }
    logFile.close();
    // reset index for next message
    recvIndex = 0;
  }
}

