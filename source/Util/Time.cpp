#include "Time.h"

#include <SDL.h>

namespace OddityEngine::Util {
    long double Time::_now = get_time();
    long double Time::_start = _now;
    long double Time::_last = _now;
    long double Time::_delta = 0;
    size_t Time::_frame = 0;
    long double Time::_frame_time = 0;

    void Time::reset() {
        long double _now = get_time();
        long double _start = _now;
        long double _last = _now;
        long double _delta = 0;
        size_t _frame = 0;
        long double _frame_time = 0;
    }
}