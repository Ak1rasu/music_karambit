#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "CoolTune.h"
#include "CoolLed.h"

#define AUDIO_PIN 9
#define BUTTON_PIN 2

#define MOTION_THRESHOLD 11.0
#define LOW_COUNT_TO_STOP 3
#define LOOP_DELAY 50 

Adafruit_MPU6050 mpu;

int lowMotionCounter = 0;
bool systemEnabled = false;        // ON / OFF state
bool lastButtonState = HIGH;      

void setup() {
  Serial.begin(9600);

  Wire.begin();
  Wire.setClock(100000);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(AUDIO_PIN, OUTPUT);

  CoolLED.begin();
  CoolLED.setActive(false);

  if (!mpu.begin()) {
    Serial.println("MPU6050 FAILED!");
    while (1);
  }

  mpu.enableSleep(false);  
  delay(100);

  Serial.println("MPU6050 detected!");
}


void loop() {
  // ---------- BUTTON TOGGLE ----------
  bool buttonState = digitalRead(BUTTON_PIN);

  // Detect button press (HIGH → LOW)
  if (lastButtonState == HIGH && buttonState == LOW) {
    systemEnabled = !systemEnabled;

    Serial.print("System ");
    Serial.println(systemEnabled ? "ON" : "OFF");

    if (!systemEnabled) {
      noTone(AUDIO_PIN);
      CoolTune.reset();
      CoolLED.setActive(false);
      lowMotionCounter = 0;
    }

    delay(200);
  }
  lastButtonState = buttonState;

  // ---------- IF SYSTEM IS OFF ----------
  if (!systemEnabled) {
    CoolLED.update();
    delay(LOOP_DELAY);
    return; // skip motion logic
  }

  // ---------- MPU6050 READING ----------
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float totalAccel = sqrt(
    a.acceleration.x * a.acceleration.x +
    a.acceleration.y * a.acceleration.y +
    a.acceleration.z * a.acceleration.z
  );

  Serial.print("Total Accel: ");
  Serial.println(totalAccel);

  // ---------- MOTION CHECK ----------
  if (totalAccel > MOTION_THRESHOLD) {
    // Motion detected
    CoolLED.setActive(true);   // active / rainbow mode
    CoolTune.playStep();       // non-blocking sound step
    lowMotionCounter = 0;
  } else {
    // No significant motion
    lowMotionCounter++;
    if (lowMotionCounter >= LOW_COUNT_TO_STOP) {
      noTone(AUDIO_PIN);
      CoolTune.reset();
      CoolLED.setActive(false); // idle LEDs
      lowMotionCounter = 0;
    }
  }

  CoolLED.update();

  delay(LOOP_DELAY);
}
