#include "OddityEngine.h"

#include <Graphics/OpenGL/Graphics.h>

namespace OddityEngine {
    void init() {
        Graphics::init();
    }

    void terminate() {
        Graphics::terminate();
    }

    bool update() {
        return Graphics::update();
    }
} // OddityEngine