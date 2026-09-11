#include <Arduino.h>
#include <DHT.h>

#include "Config.hpp"
#include "Sensors.hpp"

namespace {
    DHT dht{Config::dhtPin, DHT11};
    Sensors::Reading reading{0, 0, NAN};
    unsigned long lastReading = 0;
    portMUX_TYPE sensorMutex = portMUX_INITIALIZER_UNLOCKED;
}

void Sensors::begin() {
    if (Config::dhtEnabled) {
        dht.begin();
    }
}

bool Sensors::update() {
    const unsigned long now = millis();
    if (now - lastReading < Config::sensorInterval) {
        return false;
    }
    lastReading = now;

    Reading nextReading = getReading();
    const float chipTemperature = temperatureRead();
    nextReading.chipTemperature = isfinite(chipTemperature) ? chipTemperature : NAN;

    if (Config::dhtEnabled) {
        const float temperature = dht.readTemperature();
        const float humidity = dht.readHumidity();

        if (!isnan(temperature) && !isnan(humidity)) {
            nextReading.temperature = temperature;
            nextReading.humidity = humidity;
        }
    }

    portENTER_CRITICAL(&sensorMutex);
    reading = nextReading;
    portEXIT_CRITICAL(&sensorMutex);

    return true;
}

Sensors::Reading Sensors::getReading() {
    portENTER_CRITICAL(&sensorMutex);
    const Reading currentReading = reading;
    portEXIT_CRITICAL(&sensorMutex);
    return currentReading;
}
