#include <Wire.h>
#include <Servo.h>
#include <math.h>

#define MPU_ADDR        0x68
#define PWR_MGMT_1      0x6B
#define ACCEL_XOUT_H    0x3B
#define PITCH_SERVO_PIN 9
#define ROLL_SERVO_PIN  10
#define WINDOW          128

Servo pitchServo;
Servo rollServo;

int16_t ax, ay, az, temp, gx, gy, gz;

// Rolling window of the last 128 raw ax samples
int16_t window[WINDOW];
uint8_t head = 0;    // where the next sample goes
uint8_t count = 0;   // how many slots are filled (max 128)
int32_t sum = 0;     // running sum of the window

void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

int16_t read16() {
  uint8_t hi = Wire.read();
  uint8_t lo = Wire.read();
  return (int16_t)((hi << 8) | lo);
}

void readSensor() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14);
  ax = read16(); ay = read16(); az = read16();
  temp = read16();
  gx = read16(); gy = read16(); gz = read16();
}

void addSample(int16_t x) {
  if (count == WINDOW) {
    sum -= window[head];      // window full: drop the oldest
  } else {
    count++;
  }
  window[head] = x;
  sum += x;
  head = (head + 1) % WINDOW; // wrap around
}

float windowMean() {
  return (float)sum / count;
}

float windowStdDev(float mean) {
  float acc = 0;
  for (uint8_t i = 0; i < count; i++) {
    float d = window[i] - mean;
    acc += d * d;
  }
  return sqrt(acc / count);
}

void printField(const char* label, float value, int digits) {
  Serial.print(label);
  Serial.print(value, digits);
  Serial.print(' ');
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  writeRegister(PWR_MGMT_1, 0x00);
  pitchServo.attach(PITCH_SERVO_PIN);
  rollServo.attach(ROLL_SERVO_PIN);
  pitchServo.write(90);
  rollServo.write(90);
  delay(500);
}

void loop() {
  readSensor();

  float pitch = atan2(-ax, sqrt((float)ay * ay + (float)az * az)) * 180.0 / PI;
  float roll  = atan2(ay, az) * 180.0 / PI;

  int pitchCmd = constrain(90 + (int)pitch, 0, 180);
  int rollCmd  = constrain(90 + (int)roll, 0, 180);
  pitchServo.write(pitchCmd);
  rollServo.write(rollCmd);

  addSample(ax);
  float mean = windowMean();
  float sd = windowStdDev(mean);

  printField("ax:", ax, 0); printField("ay:", ay, 0); printField("az:", az, 0);
  printField("gx:", gx, 0); printField("gy:", gy, 0); printField("gz:", gz, 0);
  printField("pitch:", pitch, 1); printField("roll:", roll, 1);
  printField("srvP:", pitchCmd, 0); printField("srvR:", rollCmd, 0);
  printField("axMean:", mean, 1);
  Serial.print("axStd:"); Serial.println(sd, 1);

  delay(20);
}