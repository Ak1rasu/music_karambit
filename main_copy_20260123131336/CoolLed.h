#ifndef COOLLED_H
#define COOLLED_H

#include <Adafruit_NeoPixel.h>

#define LED_PIN 6 // which pin its connected to
#define NUM_LEDS 38 // how many led lights turn on

class CoolLEDClass {
private:
    Adafruit_NeoPixel strip;
    uint16_t hue;                 // hue for idle glow
    unsigned long lastUpdate;     // last time LEDs updated
    int interval;                 // ms between updates
    bool activeMode;              // true = active rainbow, false = idle glow

public:
    CoolLEDClass() : strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800), hue(0), lastUpdate(0), interval(50), activeMode(false) {}

    void begin() {
        strip.begin();
        strip.show();
    }

    void update() {
        unsigned long now = millis();
        if (activeMode) {
            // Active rainbow animation
            for (int i = 0; i < strip.numPixels(); i++) {
                int pixelHue = (i * 65536L / strip.numPixels() + now * 50) & 65535;
                strip.setPixelColor(i, strip.ColorHSV(pixelHue));
            }
            strip.show();
        } else {
            // Idle glow
            if (now - lastUpdate >= interval) {
                for (int i = 0; i < strip.numPixels(); i++) {
                    strip.setPixelColor(i, strip.ColorHSV(hue, 255, 30)); // low brightness
                }
                strip.show();
                hue += 200;                  // increment hue slowly
                if (hue > 65535) hue = 0;
                lastUpdate = now;
            }
        }
    }

    void setActive(bool active) {
        activeMode = active;
    }

    void allOff() {
        for (int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, 0);
        }
        strip.show();
    }
};

extern CoolLEDClass CoolLED;

#endif
