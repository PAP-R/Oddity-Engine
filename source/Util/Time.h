#ifndef TIME_H
#define TIME_H

#include <type_traits>
#include <SDL.h>

namespace OddityEngine::Util {
    class Time {
    protected:
        static long double get_time() {
            return SDL_GetPerformanceCounter() / static_cast<long double>(SDL_GetPerformanceFrequency());
        }

        static long double _now;
        static long double _start;
        static long double _last;
        static long double _delta;
        static size_t _frame;

    public:
        Time() = delete;

        static void update() {
            _now = get_time();
            _delta = _now - _last;
            _last = _now;
            ++_frame;
        }

        template<typename T = long double, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        static T now() {
            return _now;
        }

        template<typename T = long double, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        static T start() {
            return _start;
        }

        template<typename T = long double, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        static T delta() {
            return _delta;
        }

        static size_t frame() {
            return _frame;
        }

        template<typename T = long double, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        static T fps() {
            return 1 / delta();
        }
    };
}

#endif //TIME_H
