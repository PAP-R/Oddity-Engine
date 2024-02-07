#include "Time.h"

#include <SDL.h>

namespace OddityEngine::Util::Time {
    float get_time() {
        return SDL_GetPerformanceCounter() / static_cast<float>(SDL_GetPerformanceFrequency());
    }

    float _now = get_time();
    float _start = _now;
    float _last = _now;
    float _delta = 0;
    size_t _frame = 0;

    void update() {
        _now = get_time();
        _delta = _now - _last;
        _last = _now;
        ++_frame;
    }

    float now() {
        return _now;
    }

    float start() {
        return _start;
    }

    float delta() {
        return _delta;
    }

    size_t frame() {
        return _frame;
    }

    float fps() {
        return 1 / delta();
    }
}