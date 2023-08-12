#ifndef ODDITYENGINE_TIME_H
#define ODDITYENGINE_TIME_H

#include <chrono>

namespace OddityEngine::Time {
    typedef std::chrono::time_point<std::chrono::system_clock> time;
    void update();
    time get_time();
    double get_time_delta();
};


#endif //ODDITYENGINE_TIME_H
