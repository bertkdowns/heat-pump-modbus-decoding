#include <ModbusMaster.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

#define MAX485_DE_RE 8
#define SD_CS 10

SoftwareSerial rs485(2, 3); // RX, TX
ModbusMaster node;
File logFile;

void preTransmission()  { digitalWrite(MAX485_DE_RE, HIGH); }
void postTransmission() { digitalWrite(MAX485_DE_RE, LOW); }

void setup() {
 Serial.begin(9600);
 rs485.begin(4800);
 pinMode(MAX485_DE_RE, OUTPUT);
 digitalWrite(MAX485_DE_RE, LOW);

 node.begin(0x10, rs485);
 node.preTransmission(preTransmission);
 node.postTransmission(postTransmission);

 if (!SD.begin(SD_CS)) {
   Serial.println("SD card failed or not present!");
   while (1);
 }

 logFile = SD.open("heatpump.csv", FILE_WRITE);
 if (logFile && logFile.size() == 0) {
   // Header row
   logFile.println("Time,UnitState,Mode,InletTemp,OutletTemp,AmbientTemp,Comp1Hz,Comp2Hz,FanRPM,Pump,Comp1,Comp2,ElecHeat,Reg,Value");
 }
 logFile.close();

 Serial.println("PHNIX Heat Pump Scanner Started");
}

float readTemp(uint16_t reg) {
 uint8_t result = node.readHoldingRegisters(reg - 1, 1);
 if (result == node.ku8MBSuccess) {
   int16_t raw = node.getResponseBuffer(0);
   if (raw == 32767) return NAN;
   return raw / 10.0;
 }
 return NAN;
}

int readInt(uint16_t reg) {
 uint8_t result = node.readHoldingRegisters(reg - 1, 1);
 if (result == node.ku8MBSuccess) return node.getResponseBuffer(0);
 return INT16_MIN;  // indicate invalid
}

void loop() {
 // Known values
 int unitState = readInt(2011);
 int mode = readInt(2012);
 float inlet = readTemp(2016);
 float outlet = readTemp(2017);
 float ambient = readTemp(2019);
 int comp1Hz = readInt(2040);
 int comp2Hz = readInt(2041);
 int fanRPM = readInt(2044);
 int outputs = readInt(2072);

 // Decode outputs
 bool pumpOn = outputs & (1 << 8);
 bool comp1On = outputs & (1 << 0);
 bool comp2On = outputs & (1 << 1);
 bool electricHeater = outputs & (1 << 11);

 // Print to Serial

 Serial.print("Unit: "); Serial.print(unitState);
 Serial.print(" Mode: "); Serial.print(mode);
 Serial.print(" Inlet: "); Serial.print(inlet);
 Serial.print(" Outlet: "); Serial.print(outlet);
 Serial.print(" Ambient: "); Serial.print(ambient);
 Serial.print(" Hz1: "); Serial.print(comp1Hz);
 Serial.print(" Hz2: "); Serial.print(comp2Hz);
 Serial.print(" FanRPM: "); Serial.print(fanRPM);
 Serial.print(" Pump: "); Serial.print(pumpOn);
 Serial.print(" Comp1: "); Serial.print(comp1On);
 Serial.print(" Comp2: "); Serial.print(comp2On);
 Serial.print(" ElecHeat: "); Serial.println(electricHeater);

 // Open log file
 logFile = SD.open("HEATPUMP.CSV", FILE_WRITE);
 if (!logFile) {
   Serial.println("SD open error");
   delay(5000);
   return;
 }

 // Log known values
 unsigned long t = millis() / 1000;
 logFile.print(t); logFile.print(",");
 logFile.print(unitState); logFile.print(",");
 logFile.print(mode); logFile.print(",");
 logFile.print(inlet); logFile.print(",");
 logFile.print(outlet); logFile.print(",");
 logFile.print(ambient); logFile.print(",");
 logFile.print(comp1Hz); logFile.print(",");
 logFile.print(comp2Hz); logFile.print(",");
 logFile.print(fanRPM); logFile.print(",");
 logFile.print(pumpOn); logFile.print(",");
 logFile.print(comp1On); logFile.print(",");
 logFile.print(comp2On); logFile.print(",");
 logFile.print(electricHeater);

 // Scan unknown addresses
//  for (uint16_t reg = 2000; reg <= 2090; reg++) {
//    int val = readInt(reg);
//    logFile.print(",");
//    logFile.print(val);
//    Serial.print(reg);
//    Serial.print(":");
//    Serial.println(val);
//   }

 logFile.println();
 logFile.close();

 Serial.println("Logged scan cycle.");
}

