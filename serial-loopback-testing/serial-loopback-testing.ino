#include <ModbusMaster.h>
#include <SoftwareSerial.h>

#define MAX485_DE_RE 8
#define SD_CS 10

SoftwareSerial rs485(2, 3); // RX, TX
ModbusMaster node;

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
//  int unitState = readInt(2011);
//  int mode = readInt(2012);
//  float inlet = readTemp(2016);
//  float outlet = readTemp(2017);
//  float ambient = readTemp(2019);
//  int comp1Hz = readInt(2040);
//  int comp2Hz = readInt(2041);
//  int fanRPM = readInt(2044);
//  int outputs = readInt(2072);

//  // Decode outputs
//  bool pumpOn = outputs & (1 << 8);
//  bool comp1On = outputs & (1 << 0);
//  bool comp2On = outputs & (1 << 1);
//  bool electricHeater = outputs & (1 << 11);

//  // Print to Serial

//  Serial.print("Unit: "); Serial.print(unitState);
//  Serial.print(" Mode: "); Serial.print(mode);
//  Serial.print(" Inlet: "); Serial.print(inlet);
//  Serial.print(" Outlet: "); Serial.print(outlet);
//  Serial.print(" Ambient: "); Serial.print(ambient);
//  Serial.print(" Hz1: "); Serial.print(comp1Hz);
//  Serial.print(" Hz2: "); Serial.print(comp2Hz);
//  Serial.print(" FanRPM: "); Serial.print(fanRPM);
//  Serial.print(" Pump: "); Serial.print(pumpOn);
//  Serial.print(" Comp1: "); Serial.print(comp1On);
//  Serial.print(" Comp2: "); Serial.print(comp2On);
//  Serial.print(" ElecHeat: "); Serial.println(electricHeater);


 Serial.println("Logged scan cycle.");
}

