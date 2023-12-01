#ifndef TESTSPIEL_TIME_H
#define TESTSPIEL_TIME_H

#include <cstddef>

namespace OddityEngine {
    namespace Time {
        void update();
        double get();
        double get_start();
        double get_delta();
        size_t get_frame();
    } // Time
} // OddityEngine

#endif //TESTSPIEL_TIME_H
