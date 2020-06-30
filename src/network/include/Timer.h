#pragma once

#include <chrono>
#include "common.h"

namespace MeyaS {
    class Timer {
    public:
        Timer() = default;
        void start(uint milliseconds);
        bool timeUp();
    private:
        std::chrono::time_point<std::chrono::system_clock> startTime;
        uint duration{};
    };
}
