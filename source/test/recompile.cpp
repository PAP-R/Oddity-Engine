#include <steam_api.h>

#include <thread>
#include <future>
#include <chrono>
#include <complex>

#include "Util/CombinedIndex.h"

using namespace std::chrono_literals;

#include <OddityEngine.h>

#include "Player.h"
#include "Graphics/OpenGL/Camera.h"
#include "Graphics/Graphics.h"
#include "Graphics/OpenGL/Window.h"
#include "Graphics/OpenGL/Render/ComputeRenderer.h"
#include "Graphics/OpenGL/Render/TestRenderer.h"
#include "Graphics/OpenGL/Render/Layered.h"
#include "Math/random.h"
#include "Physics/World.h"

#include <numbers>

#include "Util/Time.h"
#include <Util/Debug.h>
#include <Util/Commander.h>
#include <Input/Input.h>

int main(int argc, char* args[]) {
    OddityEngine::init();

    auto window = OddityEngine::Graphics::Window("Hallo", 800, 600, SDL_WINDOW_RESIZABLE);

    std::vector<std::string> files = {"test.frag"};

    OddityEngine::Graphics::Shader shader(GL_FRAGMENT_SHADER);

    for (const auto& f : files) {
        shader.add(OddityEngine::Graphics::Shader::read_shader(f));
    }

    OddityEngine::Debug::message("Shadercode:\n{}", shader.compile());


    OddityEngine::Graphics::Program program({OddityEngine::Graphics::Shader(GL_COMPUTE_SHADER, "operator.comp")});

    program.shaders.front().add(OddityEngine::Graphics::Shader::read_shader("physics_operations.glsl"));

    program.recompile();


    OddityEngine::Graphics::Scene scene;
    window.set_scene(&scene);

    OddityEngine::Graphics::Camera camera({0, 10, 10});

    auto renderer = new OddityEngine::Graphics::Render::Layered(&camera);
    scene.add_renderer(renderer);

    OddityEngine::Util::Time::reset();

    do {
    } while (OddityEngine::update());

    OddityEngine::terminate();

    return 0;
}
