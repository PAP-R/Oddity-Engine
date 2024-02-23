#include <steam_api.h>

#include <thread>
#include <future>
#include <chrono>

using namespace std::chrono_literals;

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

    auto window = OddityEngine::Graphics::Window("Hallo", 800, 600, SDL_WINDOW_RESIZABLE);

    OddityEngine::Graphics::Scene scene;
    window.set_scene(&scene);


    Player player;

    auto renderer = new OddityEngine::Graphics::Render::ComputeRenderer(player.camera);
    scene.add_renderer(renderer);

    // renderer->set_size({11, 11});

    player.position.z = 5;

    player.radius = 1;
    player.mass = 1;

    // player.state &= ~OddityEngine::Physics::SHOW;

    // player.angle_velocity.x = 10;
    // player.angle_velocity.y = 1;

    scene.add_input(&player);

    OddityEngine::Physics::World world;

    renderer->world = &world;

    world.add_object(&player);

    OddityEngine::Physics::Object center_ball({0, 10, 0});
    OddityEngine::Physics::Object ball1({-10, 0, 0});
    OddityEngine::Physics::Object ball2({10, 0, 0});
    OddityEngine::Physics::Object ball3({0, -10, 0});
    OddityEngine::Physics::Object ball4({0, 10, 0});

    world.add_object(&center_ball);
    // world.add_object(&ball1);
    // world.add_object(&ball2);
    // world.add_object(&ball3);
    // world.add_object(&ball4);

    // center_ball.velocity.y = -10;

    center_ball.mass = 100;
    center_ball.radius = 5;
    ball1.radius = 0.5;
    ball2.radius = 0.5;
    ball3.radius = 0.5;
    ball4.radius = 0.5;

    ball4.velocity.x = 1;

    OddityEngine::Vector<OddityEngine::Physics::Object*> balls;

    int round = 8;
    float height = 1;
    float width = 6;
    float layer_width = 2;
    int count = round * 1;

    for (int i = 0; i < count; i++) {
        // balls.push_back(new OddityEngine::Physics::Object({(width + layer_width * (i / round + 1)) * sin((i + 0.5 * ((i / round) % 2)) * std::numbers::pi * 2 / round), height, (width + layer_width * (i / round + 1)) * cos((i + 0.5 * ((i / round) % 2)) * std::numbers::pi * 2 / round)}));
        balls.push_back(new OddityEngine::Physics::Object({(width + layer_width) * sin((i + 0.5 * ((i / round) % 2)) * std::numbers::pi * 2 / round), height * i, (width + layer_width) * cos((i + 0.5 * ((i / round) % 2)) * std::numbers::pi * 2 / round)}));

        if (i > 0) {
            balls[i]->set_prev(balls[i - 1]);
        }

        // balls.push_back(new OddityEngine::Physics::Object({(width + layer_width * (i / round + 1)) * sin((i + 0.5 * ((i / round) % 2)) * std::numbers::pi * 2 / round), (width + layer_width * (i / round + 1)) * cos((i + 0.5 * ((i / round) % 2)) * std::numbers::pi * 2 / round), 0}));
        balls.back()->radius = 1;
        balls.back()->mass = 3;
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
        OddityEngine::Debug::message("Connected? {}", (b->state & OddityEngine::Physics::CONNECTED));
        world.add_object(b);
    }

    // world.gravity = glm::vec4(0);
    // world.ground_height = -std::numeric_limits<float>::infinity();

    // renderer->set_size({10, 10});

    long double delta_added = 0;

    OddityEngine::Util::Time::set_framerate(240);

    do {
        world.update();
    } while (OddityEngine::update());

    for (auto b : balls) {
        delete b;
    }

    balls.clear();

    OddityEngine::terminate();

    return 0;
}
