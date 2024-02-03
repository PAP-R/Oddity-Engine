#include <OddityEngine.h>

#include "Graphics/Graphics.h"
#include "Graphics/Window.h"

int main(int argc, char* args[]) {
    OddityEngine::init();

    auto window = new OddityEngine::Graphics::Window("Hallo", 100, 100);

    while (OddityEngine::update()) {

    }

    OddityEngine::terminate();

    return 0;
}
