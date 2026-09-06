#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "Config.hpp"
#include "Motion.hpp"
#include "Sensors.hpp"
#include "Screen.hpp"

namespace {
    Adafruit_SSD1306 display{
        Config::screenWidth,
        Config::screenHeight,
        &Wire,
        Config::oledReset
    };
    bool showSensors = true;
    bool refreshPending = true;
    bool screenOn = true;
    unsigned long lastActivity = 0;
    Motion::State lastMotionState = Motion::State::WarmingUp;

    void wakeDisplay() {
        lastActivity = millis();
        if (!screenOn) {
            display.ssd1306_command(SSD1306_DISPLAYON);
            screenOn = true;
            refreshPending = true;
        }
    }
}

bool Screen::begin() {
    Wire.begin(Config::sdaPin, Config::sclPin);
    if (!display.begin(SSD1306_SWITCHCAPVCC, Config::oledAddress)) {
        return false;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 20);
    display.println("Iniciando...");
    display.display();

    lastActivity = millis();

    return true;
}

void Screen::toggle() {
    wakeDisplay();
    showSensors = !showSensors;
    refreshPending = true;
    Serial.print("Tela alterada para: ");
    Serial.println(showSensors ? "Sensores" : "Sistema");
}

void Screen::update(bool newReading) {
    const Motion::State motionState = Motion::getState();
    if (motionState != lastMotionState) {
        lastMotionState = motionState;
        refreshPending = true;
    }

    if (motionState == Motion::State::WarmingUp || motionState == Motion::State::Detected) {
        wakeDisplay();
    } else if (screenOn && millis() - lastActivity >= Config::screenTimeout) {
        display.ssd1306_command(SSD1306_DISPLAYOFF);
        screenOn = false;
    }

    if (!screenOn) {
        return;
    }

    if (!refreshPending && !newReading) {
        return;
    }
    refreshPending = false;
    const Sensors::Reading reading = Sensors::getReading();

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    if (showSensors) {
        display.setCursor(0, 0);
        display.println("=== SENSORES ===");

        display.setCursor(0, 18);
        display.print("Temperatura: ");
        display.print(reading.temperature, 1);
        display.println(" C");

        display.setCursor(0, 32);
        display.print("Umidade: ");
        display.print(reading.humidity, 1);
        display.println(" %");

        display.setCursor(0, 46);
        display.print("PIR: ");
        display.println(Motion::getStatusText(motionState));
    } else {
        display.setCursor(0, 0);
        display.println("=== ESP32 ===");

        display.setCursor(0, 18);
        display.print("CPU: ");
        display.println(ESP.getChipModel());

        display.setCursor(0, 32);
        display.print("Freq: ");
        display.print(ESP.getCpuFreqMHz());
        display.println(" MHz");

        display.setCursor(0, 46);
        display.print("RAM: ");
        display.print(ESP.getFreeHeap());
        display.println(" B");
    }

    display.display();
}
