#include "OddityEngine.h"

#include <Graphics/Graphics.h>

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

}