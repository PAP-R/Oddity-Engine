#include <OddityEngine.h>

#include "Graphics/Camera.h"
#include "Graphics/Graphics.h"
#include "Graphics/Window.h"
#include "Graphics/Render/TestRenderer.h"
#include "Math/random.h"

int main(int argc, char* args[]) {
    OddityEngine::init();

    auto window = new OddityEngine::Graphics::Window("Hallo", 600, 600, SDL_WINDOW_RESIZABLE);

    OddityEngine::Graphics::Scene scene;
    window->set_scene(&scene);

    OddityEngine::Graphics::Camera camera(90);
    auto renderer = new OddityEngine::Graphics::Render::TestRenderer(&camera);
    scene.add_renderer(renderer);

    renderer->set_size({100, 100});

    while (OddityEngine::update()) {
        renderer->set_screen_pos({OddityEngine::Math::random(100, 300), OddityEngine::Math::random(100, 300)});
    }

    OddityEngine::terminate();

    return 0;
}
