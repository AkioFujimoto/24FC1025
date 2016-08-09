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
  // put your setup code here, to run once:
  Wire.begin(0x57); // 24fc1025 address.
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(addr);
  Wire.write(hbyte);
  Wire.write(lbyte);
  Wire.endTransmission(addr);
  Wire.beginTransmission(addr);
  Wire.requestFrom(addr, 2);
  Wire.endTransmission(addr);

  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  if (lbyte < 0b11111111) {
    lbyte += 0b1;
  } else if (lbyte == 0xff) {
    lbyte = 0x0;
    if (hbyte < 0b10101111) {
      hbyte += 0b1;
    } else {
      hbyte = 0x10101110; // a15..8
      lbyte = 0x00000000; // a7..0
    }
  }
  Wire.endTransmission();
}

