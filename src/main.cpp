#include <Arduino.h>

#include "Button.hpp"
#include "Buzzer.hpp"
#include "Config.hpp"
#include "Motion.hpp"
#include "Network.hpp"
#include "Sensors.hpp"
#include "Screen.hpp"

void setup() {
    Serial.begin(Config::serialBaudRate);
    Serial.println("Iniciando...");
    Sensors::begin();
    Button::begin();
    Motion::begin();

    if (!Screen::begin()) {
        Serial.println("OLED não encontrada.");
        while (true);
    }

    Network::begin();
    Buzzer::begin();
}

void loop() {
    Buzzer::update();
    Network::update();
    Motion::update();

    if (Button::wasPressed()) {
        Screen::toggle();
        Buzzer::beep();
    }

    const bool newReading = Sensors::update();
    Screen::update(newReading);

    delay(10);
}
