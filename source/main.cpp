#include <steam_api.h>

#include <OddityEngine.h>

#include "Player.h"
#include "Graphics/Camera.h"
#include "Graphics/Graphics.h"
#include "Graphics/Window.h"
#include "Graphics/Render/ComputeRenderer.h"
#include "Graphics/Render/TestRenderer.h"
#include "Math/random.h"
#include "Physics/World.h"

#include <numbers>

#include "Util/Time.h"
#include <Util/Debug.h>

int main(int argc, char* args[]) {
    OddityEngine::init();

    std::cout << SteamFriends()->GetPersonaName() << std::endl;

    auto window = OddityEngine::Graphics::Window("Hallo", 60, 60, SDL_WINDOW_RESIZABLE);

    OddityEngine::Graphics::Scene scene;
    window.set_scene(&scene);


    Player player;

    auto renderer = new OddityEngine::Graphics::Render::ComputeRenderer(player.camera);
    scene.add_renderer(renderer);

    player.position.z = 50;

    player.state &= ~OddityEngine::Physics::SHOW;

    // player.angle_velocity.x = 10;
    // player.angle_velocity.y = 1;

    scene.add_input(&player);

    OddityEngine::Physics::World world;

    renderer->world = &world;

    world.add_object(&player);

    OddityEngine::Physics::Object center_ball({0, 0, 0});
    OddityEngine::Physics::Object ball1({-10, 0, 0});
    OddityEngine::Physics::Object ball2({10, 0, 0});
    OddityEngine::Physics::Object ball3({0, -10, 0});
    OddityEngine::Physics::Object ball4({0, 10, 0});

    world.add_object(&center_ball);
    // world.add_object(&ball1);
    // world.add_object(&ball2);
    // world.add_object(&ball3);
    // world.add_object(&ball4);

    center_ball.mass = 100000;
    center_ball.test_value.x = 5;
    center_ball.test_value.y = 150;
    ball1.test_value.x = 0.5;
    ball1.test_value.y = 150;
    ball2.test_value.x = 0.5;
    ball2.test_value.y = 150;
    ball3.test_value.x = 0.5;
    ball3.test_value.y = 150;
    ball4.test_value.x = 0.5;
    ball4.test_value.y = 150;

    ball4.velocity.x = 1;

    OddityEngine::Vector<OddityEngine::Physics::Object*> balls;

    int round = 8;
    float height = 6;
    float layer_height = 2;
    int count = round * 1;

    for (int i = 0; i < count; i++) {
        balls.push_back(new OddityEngine::Physics::Object({(height + layer_height * (i / round + 1)) * sin((i + 0.5 * ((i / round) % 2)) * std::numbers::pi * 2 / round), (height + layer_height * (i / round + 1)) * cos((i + 0.5 * ((i / round) % 2)) * std::numbers::pi * 2 / round), 0}));
        balls.back()->test_value.x = 1;
        balls.back()->mass = 0.5;
        // if (balls.back()->position.y > 0) {
        //     balls.back()->velocity.y = -1;
        // }
        // else {
        //     balls.back()->velocity.y = 1;
        // }
        // balls.back()->velocity.x = (height + layer_height * (i / round + 1)) * sin((i + 0.5 * ((i / round) % 2)) * std::numbers::pi * 2 / round) - balls.back()->position.x;
        // balls.back()->velocity.y = (height + layer_height * (i / round + 1)) * cos((i + 0.5 * ((i / round) % 2)) * std::numbers::pi * 2 / round) - balls.back()->position.y;
    }

    for (auto b : balls) {
        world.add_object(b);
    }

    player.camera->fov = 45;

    SDL_SetRelativeMouseMode(SDL_TRUE);

    // renderer->set_size({10, 10});

    do {
        // fmt::print("[{}] : FPS: {}\tDelta: {}\n", OddityEngine::Util::Time::frame(), OddityEngine::Util::Time::fps<size_t>(), OddityEngine::Util::Time::delta<float>() * 1000);
        world.update();
    } while (OddityEngine::update());

    for (auto b : balls) {
        delete b;
    }

    balls.clear();

    OddityEngine::terminate();

    return 0;
}
