#ifndef TIME_H
#define TIME_H

#include <chrono>

class Time {
protected:
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::duration<long double> duration;
    typedef std::chrono::time_point<clock, duration> time;

    static time _now;
    static time _start;
    static time _last;
    static duration _delta;
    static duration _runtime;
    static size_t _frame;

public:
    Time() = delete;

    static auto get_time() {
        return std::chrono::high_resolution_clock::now();
    }

    static auto now() {
        return _now.time_since_epoch().count();
    }

    static auto start() {
        return _start.time_since_epoch().count();
    }

    static auto last() {
        return _last.time_since_epoch().count();
    }

    static auto delta() {
        return _delta.count();
    }

    static auto runtime() {
        return _runtime.count();
    }

    static auto frame() {
        return _frame;
    }

    static void update() {
        _now = get_time();
        _delta = _now - _last;
        _runtime = _now - _start;

        _last = _now;
        _frame += 1;
    }

    static void reset() {
        _now = get_time();
        _start = _now;
        _last = _now;
        _delta = duration::zero();
        _frame = 0;
    }
};



#endif //TIME_H
