#include "..\include\Timer.h"

void MeyaS::Timer::start(MeyaS::uint milliseconds) {
    this->duration = milliseconds;
    startTime = std::chrono::system_clock::now();
}

bool MeyaS::Timer::timeUp() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() > duration;
}
