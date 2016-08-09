// I2C EEPROM test
// This reads Microchip's 24FC1025 EEPROM sequentially,
// and send received data into Arduino's serial monitor.
// but this doesn't work correctly.
// (c) 2016- Akio Fujimoto

#include <Wire.h>

const int addr = 0x57;  // address may vary depends to wiring.
int cbyte = 0x10101110; // 1010(Fixed), a2,1,B0(BlockSelectBit), r/#w
int hbyte = 0x10101110; // a15..8
int lbyte = 0x00000000; // a7..0

void setup() {
  Wire.begin(0x57); // 24fc1025 address.
  Serial.begin(9600); // initialize serial communication port in 9600bps.
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(addr);  // I2C EEPROM's address.
  Wire.write(hbyte); // High Byte.
  Wire.write(lbyte); // Low Byte.
  Wire.endTransmission(addr);
  Wire.beginTransmission(addr);
  Wire.requestFrom(addr, 2);
  Wire.endTransmission(addr);

  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  if (lbyte < 0b11111111) {
    lbyte += 0b1; // add an one bit.
  } else if (lbyte == 0xff) {
    lbyte = 0x0; // sets lbyte to 0 and increment highbyte to next step.
    if (hbyte < 0b10101111) {
      hbyte += 0b1;
    } else {
      hbyte = 0x10101110; // a15..8
      lbyte = 0x00000000; // a7..0
    }
  }
  Wire.endTransmission();
}

