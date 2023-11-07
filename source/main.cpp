#include <OddityEngine.h>
#include <Graphics/OpenGL/Window.h>

int main() {
    OddityEngine::init();

    auto window = new OddityEngine::Graphics::Window("Oddity", 400, 400);

    do {

    } while (OddityEngine::update());

    OddityEngine::terminate();
}