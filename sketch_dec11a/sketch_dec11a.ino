#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Pins
int sliderPin = A0;      // Middle pin of slider
int speakerPin = 8;       // Speaker

// Settings
int sliderThreshold = 512;   // Slider ON if > 512
int motionThreshold = 15000; // Motion detection sensitivity

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize the MPU6050
  Serial.println("Initializing MPU6050...");
  mpu.initialize();

  // Check if sensor works
  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected!");
  } else {
    Serial.println("MPU6050 connection FAILED!");
  }

  pinMode(speakerPin, OUTPUT);
}

void loop() {

  // Read slider
  int sliderValue = analogRead(sliderPin);

  if (sliderValue > sliderThreshold) {

    // -------- MPU6050 Motion Read --------
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Print values
    Serial.print("AX: "); Serial.print(ax);
    Serial.print(" AY: "); Serial.print(ay);
    Serial.print(" AZ: "); Serial.print(az);
    Serial.print(" | GX: "); Serial.print(gx);
    Serial.print(" GY: "); Serial.print(gy);
    Serial.print(" GZ: "); Serial.println(gz);

    // -------- Motion Detection --------
    if (abs(ax) > motionThreshold ||
        abs(ay) > motionThreshold ||
        abs(az) > motionThreshold) {

      tone(speakerPin, 1000, 150); // beep
      delay(200);
    }

  } else {
    // Slider is OFF
    Serial.println("SLIDER OFF");
    delay(200);
  }
}
