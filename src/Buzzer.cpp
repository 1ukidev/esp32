#include <Arduino.h>

#include "Buzzer.hpp"
#include "Config.hpp"

namespace {
    unsigned long beepStartedAt = 0;
    bool ready = false;
    bool playing = false;
}

void Buzzer::begin() {
    if (ledcSetup(Config::buzzerChannel, Config::buzzerFrequency, 10) == 0) {
        Serial.println("Falha ao configurar o buzzer.");
        return;
    }

    ledcAttachPin(Config::buzzerPin, Config::buzzerChannel);
    ledcWriteTone(Config::buzzerChannel, 0);
    ready = true;
}

void Buzzer::beep() {
    if (!ready) {
        return;
    }

    ledcWriteTone(Config::buzzerChannel, Config::buzzerFrequency);
    beepStartedAt = millis();
    playing = true;
}

void Buzzer::update() {
    if (playing && millis() - beepStartedAt >= Config::buzzerDuration) {
        ledcWriteTone(Config::buzzerChannel, 0);
        playing = false;
    }
}
