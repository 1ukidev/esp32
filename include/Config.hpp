#pragma once

namespace Config {
    constexpr int oledReset = -1;
    constexpr unsigned int screenWidth = 128;
    constexpr unsigned int screenHeight = 64;
    constexpr unsigned int oledAddress = 0x3C;
    constexpr unsigned int sdaPin = 21;
    constexpr unsigned int sclPin = 22;
    constexpr unsigned int dhtPin = 19;
    constexpr unsigned int buttonPin = 18;
    constexpr unsigned int pirPin = 14;
    constexpr unsigned int ledPin = 2;
    constexpr unsigned int buzzerPin = 13;
    constexpr unsigned int buzzerChannel = 2;
    constexpr unsigned int buzzerFrequency = 2000;
    constexpr unsigned long buzzerDuration = 50;
    constexpr unsigned long serialBaudRate = 115200;
    constexpr unsigned long sensorInterval = 4000;
    constexpr unsigned long pirWarmupDuration = 60000;
    constexpr unsigned long pirClearDelay = 3000;
    constexpr unsigned long screenTimeout = 30000;
    constexpr unsigned long buttonDebounce = 50;
    constexpr unsigned long reconnectInterval = 15000;
    constexpr unsigned long serverRetryInterval = 5000;
    constexpr unsigned int pageRefreshSeconds = 4;
    constexpr const char* wifiSSID = "(SSID)";
    constexpr const char* wifiPassword = "(Password)";
}
