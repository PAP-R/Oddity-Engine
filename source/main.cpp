#include <OddityEngine.h>

#include "Player.h"
#include "Graphics/Camera.h"
#include "Graphics/Graphics.h"
#include "Graphics/Window.h"
#include "Graphics/Render/TestRenderer.h"
#include "Math/random.h"
#include "Physics/World.h"

int main(int argc, char* args[]) {
    OddityEngine::init();

    auto window = new OddityEngine::Graphics::Window("Hallo", 600, 600, SDL_WINDOW_RESIZABLE);

    OddityEngine::Graphics::Scene scene;
    window->set_scene(&scene);

    OddityEngine::Graphics::Camera camera(90);
    auto renderer = new OddityEngine::Graphics::Render::TestRenderer(&camera);
    scene.add_renderer(renderer);

    renderer->set_size({10, 10});

    Player player;
    player.angle_velocity += glm::vec4(1, 0, 0, 0);

    OddityEngine::Physics::World world;

    OddityEngine::Physics::Object ball1({-1, 0, 0});
    OddityEngine::Physics::Object ball2({1, 0, 0});
    OddityEngine::Physics::Object ball3({0, 1, 0});
    OddityEngine::Physics::Object ball4({0, -1, 0});

    world.add_object(&ball1);
    world.add_object(&ball2);
    world.add_object(&ball3);
    world.add_object(&ball4);

    ball1.test_value.x = 0.5;
    ball2.test_value.x = 0.5;
    ball3.test_value.x = 0.5;
    ball4.test_value.x = 0.5;

    while (OddityEngine::update()) {
        fmt::print("< [ {} / {} / {} ] >", ball1.position.x, ball1.position.y, ball1.position.z);
        fmt::print("< [ {} / {} / {} ] >", ball2.position.x, ball2.position.y, ball2.position.z);
        fmt::print("< [ {} / {} / {} ] >", ball3.position.x, ball3.position.y, ball3.position.z);
        fmt::print("< [ {} / {} / {} ] >", ball4.position.x, ball4.position.y, ball4.position.z);
        fmt::print("\n");
        // fmt::print("[ {} / {} / {} ] >-< [ {} / {} / {} ]\n\n", ball1.acceleration.x, ball1.acceleration.y, ball1.acceleration.z, ball2.acceleration.x, ball2.acceleration.y, ball2.acceleration.z);
        world.update();
    }

    OddityEngine::terminate();

    return 0;
}
