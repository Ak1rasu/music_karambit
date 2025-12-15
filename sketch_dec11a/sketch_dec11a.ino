#include <Wire.h>
#include <MPU6050.h>
#include "sounds.h"

MPU6050 mpu;

// Pins
const int sliderPin  = A0;
const int speakerPin = 8;

// Thresholds
const int sliderThreshold = 512;
const int motionThreshold = 2000;

// Previous acceleration values
int16_t lastAx = 0;
int16_t lastAy = 0;
int16_t lastAz = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected!");
  } else {
    Serial.println("MPU6050 FAILED!");
  }

  pinMode(speakerPin, OUTPUT);
}

void loop() {
  if (analogRead(sliderPin) <= sliderThreshold) {
    noTone(speakerPin);
    return;
  }

  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  int motion =
    abs(ax - lastAx) +
    abs(ay - lastAy) +
    abs(az - lastAz);

  lastAx = ax;
  lastAy = ay;
  lastAz = az;

  Serial.print("Motion: ");
  Serial.println(motion);

  if (motion > motionThreshold) {
    tone(speakerPin, melody);
  } else {
    noTone(speakerPin);
  }

  delay(50);
}
