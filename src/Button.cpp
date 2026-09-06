#include <Arduino.h>

#include "Button.hpp"
#include "Config.hpp"

namespace {
    bool state = HIGH;
    bool lastReading = HIGH;
    unsigned long lastDebounce = 0;
}

void Button::begin() {
    pinMode(Config::buttonPin, INPUT_PULLUP);
}

bool Button::wasPressed() {
    const bool reading = digitalRead(Config::buttonPin);
    const unsigned long now = millis();
    bool pressed = false;

    if (reading != lastReading) {
        lastDebounce = now;
    }

    if (now - lastDebounce > Config::buttonDebounce && reading != state) {
        state = reading;
        pressed = state == LOW;
    }

    lastReading = reading;
    return pressed;
}
