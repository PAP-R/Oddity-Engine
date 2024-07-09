#include "Time.h"

Time::time Time::_now = Time::get_time();
Time::time Time::_start = Time::_now;
Time::time Time::_last = Time::_now;
Time::duration Time::_delta = duration::zero();
Time::duration Time::_runtime = duration::zero();
size_t Time::_frame = 0;