#include "Time.h"

namespace OddityEngine::Time {
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> start = now;
    std::chrono::time_point<std::chrono::system_clock> last = now;
    double delta;

    void update() {
        now = std::chrono::system_clock::now();
        delta = std::chrono::duration<double>(now - last).count();
        last = now;
    }

    time get_time() {
        return now;
    }

    double get_time_delta() {
        return delta;
    }
};