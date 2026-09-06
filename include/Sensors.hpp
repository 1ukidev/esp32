#pragma once

namespace Sensors {
    struct Reading {
        float temperature;
        float humidity;
    };

    void begin();
    bool update();
    Reading getReading();
}
