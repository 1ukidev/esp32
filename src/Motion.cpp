#include <Arduino.h>

#include "Config.hpp"
#include "Motion.hpp"

namespace {
    Motion::State state = Motion::State::WarmingUp;
    unsigned long startedAt = 0;
    unsigned long clearStartedAt = 0;
    bool waitingForClear = false;
    portMUX_TYPE stateMutex = portMUX_INITIALIZER_UNLOCKED;
}

void Motion::begin() {
    pinMode(Config::pirPin, INPUT);
    startedAt = millis();
    Serial.println("PIR em estabilização por 60 segundos.");
}

void Motion::update() {
    const unsigned long now = millis();
    const State previousState = getState();
    if (previousState == State::WarmingUp &&
        now - startedAt < Config::pirWarmupDuration) {
        return;
    }

    const State nextState = digitalRead(Config::pirPin) == HIGH
        ? State::Detected
        : State::Clear;

    if (nextState == State::Detected) {
        waitingForClear = false;
    } else if (previousState == State::Detected) {
        if (!waitingForClear) {
            clearStartedAt = now;
            waitingForClear = true;
        }
        if (now - clearStartedAt < Config::pirClearDelay) {
            return;
        }
        waitingForClear = false;
    }

    if (nextState == previousState) {
        return;
    }

    portENTER_CRITICAL(&stateMutex);
    state = nextState;
    portEXIT_CRITICAL(&stateMutex);

    if (previousState == State::WarmingUp) {
        Serial.println("PIR pronto para detectar movimento.");
    }
    Serial.println(nextState == State::Detected
        ? "Movimento detectado."
        : "Sem movimento detectado.");
}

Motion::State Motion::getState() {
    portENTER_CRITICAL(&stateMutex);
    const State currentState = state;
    portEXIT_CRITICAL(&stateMutex);
    return currentState;
}

const char* Motion::getStatusText(State currentState) {
    switch (currentState) {
        case State::WarmingUp:
            return "Aquecendo";
        case State::Detected:
            return "Movimento";
        case State::Clear:
            return "Sem movimento";
    }
    return "Indisponivel";
}
