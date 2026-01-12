#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

MPU6050 mpu;

// DFPlayer
SoftwareSerial mp3Serial(10, 11); // TX, RX
DFRobotDFPlayerMini mp3;

// Button
const int buttonPin = 2;
bool systemOn = false;
bool lastButtonState = HIGH;

// Motion
int16_t ax, ay, az;
unsigned long lastPlayTime = 0;
const unsigned long cooldown = 800; // ms

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(buttonPin, INPUT_PULLUP);

  // MPU6050 init
  Serial.println("Initializing MPU6050...");
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("❌ MPU6050 FAILED");
    while (true);
  }
  Serial.println("✅ MPU6050 connected");

  // DFPlayer init
  mp3Serial.begin(9600);
  delay(1000);

  Serial.println("Initializing DFPlayer...");
  if (!mp3.begin(mp3Serial)) {
    Serial.println("❌ DFPlayer FAILED");
    while (true);
  }

  mp3.volume(28);
  Serial.println("✅ DFPlayer ready");
}

void loop() {
  handleButton();
  if (systemOn) {
    detectMotion();
  }
}

void handleButton() {
  bool buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    systemOn = !systemOn;
    delay(200); // debounce

    if (systemOn) {
      Serial.println("▶ System ON");
      mp3.play(1);
    } else {
      Serial.println("⏹ System OFF");
      mp3.stop();
    }
  }

  lastButtonState = buttonState;
}

void detectMotion() {
  mpu.getAcceleration(&ax, &ay, &az);

  long motion =
    abs(ax) + abs(ay) + abs(az);

  Serial.print("Motion: ");
  Serial.println(motion);

  if (motion > 25000 && millis() - lastPlayTime > cooldown) {
    Serial.println("💥 Motion trigger!");
    mp3.play(2);
    lastPlayTime = millis();
  }
}
