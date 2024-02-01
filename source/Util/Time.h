#ifndef TESTSPIEL_TIME_H
#define TESTSPIEL_TIME_H

#include <cstddef>

namespace OddityEngine {
    namespace Time {
        void update();
        float get();
        float get_start();
        float get_delta();
        size_t get_frame();
        float get_fps();
    } // Time
} // OddityEngine

#endif //TESTSPIEL_TIME_H
