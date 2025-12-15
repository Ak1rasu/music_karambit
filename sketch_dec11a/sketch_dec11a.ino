#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const int sliderPin = A0;
const int speakerPin = 8;

const int sliderThreshold = 512;
const int motionThreshold = 15000;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  Serial.println(mpu.testConnection() ? "MPU6050 connected!" : "MPU6050 FAILED!");
}

void loop() {
  if (analogRead(sliderPin) <= sliderThreshold) {
    noTone(speakerPin);
    return;
  }

  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  if (abs(ax) > motionThreshold ||
      abs(ay) > motionThreshold ||
      abs(az) > motionThreshold) 
  {
    tone(speakerPin, 1000);
  } else {
    noTone(speakerPin);
  }

  delay(50);
}
