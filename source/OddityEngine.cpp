#include "OddityEngine.h"

#include "graphics/Graphics.h"
#include "util/Time.h"

namespace OddityEngine {
    void init() {
        Graphics::init();
    }

    void terminate() {
        Graphics::terminate();
    }

    bool update() {
        Time::update();
        return Graphics::update();
    }
}
