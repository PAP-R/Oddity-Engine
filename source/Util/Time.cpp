#include "Time.h"

#include <SDL.h>

namespace OddityEngine::Util {
    long double Time::_now = get_time();
    long double Time::_start = _now;
    long double Time::_last = _now;
    long double Time::_delta = 0;
    size_t Time::_frame = 0;
    long double Time::_frame_time = 0;
}