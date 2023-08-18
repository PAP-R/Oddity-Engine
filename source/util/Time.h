#ifndef ODDITYENGINE_TIME_H
#define ODDITYENGINE_TIME_H

#include <chrono>

namespace OddityEngine::Time {
    typedef std::chrono::time_point<std::chrono::system_clock> time;
    void update();
    time get_time();
    time get_start();
    double get_time_delta();

    template<typename from, typename to>
    to get_time() {
        auto time = get_time();
        auto now = std::chrono::time_point_cast<from>(time);
        auto epoch = now.time_since_epoch();
        auto value = std::chrono::duration_cast<from>(epoch);
        return value.count();
    }

    template<typename from, typename to>
    to get_runtime() {
        auto time = get_time();
        auto now = std::chrono::time_point_cast<from>(time);
        std::chrono::duration<to> duration = now - get_start();
        return duration.count();
    }
};


#endif //ODDITYENGINE_TIME_H
