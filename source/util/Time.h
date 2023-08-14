#ifndef ODDITYENGINE_TIME_H
#define ODDITYENGINE_TIME_H

#include <chrono>

namespace OddityEngine::Time {
    typedef std::chrono::time_point<std::chrono::system_clock> time;
    void update();
    time get_time();
    double get_time_delta();

    template<typename T>
    double get_time() {
        auto time = get_time();
        auto now = std::chrono::time_point_cast<T>(time);
        auto epoch = now.time_since_epoch();
        auto value = std::chrono::duration_cast<T>(epoch);
        return value.count();
    }
};


#endif //ODDITYENGINE_TIME_H
