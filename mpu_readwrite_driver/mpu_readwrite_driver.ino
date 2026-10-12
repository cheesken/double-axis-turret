#include <Wire.h>

#define MPU_ADDR   0x68
#define WHO_AM_I   0x75
#define PWR_MGMT_1 0x6B

void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

uint8_t readRegister(uint8_t reg) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 1);
  return Wire.read();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.print("WHO_AM_I: ");
  Serial.println(readRegister(WHO_AM_I), HEX);

  Serial.print("PWR_MGMT_1 before: ");
  Serial.println(readRegister(PWR_MGMT_1), HEX);

  writeRegister(PWR_MGMT_1, 0x00);

  Serial.print("PWR_MGMT_1 after: ");
  Serial.println(readRegister(PWR_MGMT_1), HEX);
}

void loop() {}