#pragma once

namespace Motion {
    enum class State {
        WarmingUp,
        Clear,
        Detected
    };

    void begin();
    void update();
    State getState();
    const char* getStatusText(State state);
}
