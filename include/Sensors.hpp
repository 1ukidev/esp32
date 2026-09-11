#pragma once

namespace Sensors {
    struct Reading {
        float temperature;
        float humidity;
        float chipTemperature;
    };

    void begin();
    bool update();
    Reading getReading();
}
