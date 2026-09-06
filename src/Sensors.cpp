#include <Arduino.h>
#include <DHT.h>

#include "Config.hpp"
#include "Sensors.hpp"

namespace {
    DHT dht{Config::dhtPin, DHT11};
    Sensors::Reading reading{0, 0};
    unsigned long lastReading = 0;
    portMUX_TYPE sensorMutex = portMUX_INITIALIZER_UNLOCKED;
}

void Sensors::begin() {
    dht.begin();
}

bool Sensors::update() {
    const unsigned long now = millis();
    if (now - lastReading < Config::sensorInterval) {
        return false;
    }
    lastReading = now;

    const float temperature = dht.readTemperature();
    const float humidity = dht.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
        portENTER_CRITICAL(&sensorMutex);
        reading = {temperature, humidity};
        portEXIT_CRITICAL(&sensorMutex);
    }

    return true;
}

Sensors::Reading Sensors::getReading() {
    portENTER_CRITICAL(&sensorMutex);
    const Reading currentReading = reading;
    portEXIT_CRITICAL(&sensorMutex);
    return currentReading;
}
