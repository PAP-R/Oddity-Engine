#include <steam_api.h>

#include <thread>
#include <future>
#include <chrono>
#include <complex>


using namespace std::chrono_literals;

#include <OddityEngine.h>

#include "Player.h"
#include "Graphics/Camera.h"
#include "Graphics/Graphics.h"
#include "Graphics/Window.h"
#include "Graphics/Render/ComputeRenderer.h"
#include "Graphics/Render/TestRenderer.h"
#include "Graphics/Render/Layered.h"
#include "Math/random.h"
#include "Physics/World.h"

#include <numbers>

#include "Util/Time.h"
#include <Util/Debug.h>
#include <Util/Commander.h>

float print(...) {
    return 0;
};

int main(int argc, char* args[]) {
    OddityEngine::init();

    auto window = OddityEngine::Graphics::Window("Hallo", 800, 600, SDL_WINDOW_RESIZABLE);

    OddityEngine::Graphics::Scene scene;
    window.set_scene(&scene);

    Player player;

    auto renderer = new OddityEngine::Graphics::Render::Layered(player.camera);
    scene.add_renderer(renderer);
    auto renderer2 = new OddityEngine::Graphics::Render::ComputeRenderer(player.camera);
    scene.add_renderer(renderer2);

    // renderer->set_size({11, 11});

    // player.position = glm::vec4(0, 50, 50, 1);

    player.radius = 1;
    player.mass = 1;

    player.camera->fov = 100;

    player.state |= OddityEngine::Physics::MOVE | OddityEngine::Physics::CLIP;

    // player.state &= ~OddityEngine::Physics::SHOW;

    // player.state &= ~OddityEngine::Physics::CLIP;
    // player.state &= ~OddityEngine::Physics::MOVE;

    // player.angle_velocity.x = 10;
    // player.angle_velocity.y = 1;

    scene.add_input(&player);

    OddityEngine::Physics::World world;

    renderer->world = &world;
    renderer2->world = &world;

    world.add_object(&player);

    float earth_scale = 1E-2;

    float earth_radius = 6371E+3 * earth_scale;
    float earth_volume = (4 * std::numbers::pi * std::pow(earth_radius, 3)) / 3;
    float earth_density = 5515 / earth_scale;

    // float earth_mass = 5.972E+24 * earth_scale;
    float earth_mass = earth_volume * earth_density;

    OddityEngine::Physics::Object earth({0, -earth_radius, 0});

    std::cout << std::setprecision(15) << earth.position.y << std::endl;

    earth.mass = earth_mass;
    earth.restitution = 1;
    earth.radius = earth_radius;
    earth.state &= ~OddityEngine::Physics::MOVE;

    world.add_object(&earth);

    OddityEngine::Physics::Object center_ball({0, 10, 0});
    OddityEngine::Physics::Object ball1({-10, 0, 0});
    OddityEngine::Physics::Object ball2({10, 0, 0});
    OddityEngine::Physics::Object ball3({0, -10, 0});
    OddityEngine::Physics::Object ball4({0, 10, 0});

    // player.shape = OddityEngine::Physics::CUBE;
    // center_ball.shape = OddityEngine::Physics::CUBE;
    // center_ball.state &= ~OddityEngine::Physics::MOVE;

    world.add_object(&center_ball);
    // world.add_object(&ball1);
    // world.add_object(&ball2);
    // world.add_object(&ball3);
    // world.add_object(&ball4);

    // center_ball.velocity.y = -10;

    center_ball.mass = 1;
    center_ball.radius = 5;
    ball1.radius = 0.5;
    ball2.radius = 0.5;
    ball3.radius = 0.5;
    ball4.radius = 0.5;

    ball4.velocity.x = 1;

    OddityEngine::Vector<OddityEngine::Physics::Object*> balls;

    int round = 8;
    float height = 10;
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
        balls.back()->mass = 1;
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
        // OddityEngine::Debug::message("Connected? {}", (b->state & OddityEngine::Physics::CONNECTED) != 0);
        world.add_object(b);
    }

    // renderer->set_size({10, 10});
    OddityEngine::Debug::message("World: [ {} | {} | {} ]", earth.position.x, earth.position.y, earth.position.z);

    OddityEngine::Util::Time::set_framerate(20);


    OddityEngine::Util::Commander commander;
    commander.add_command("print", [&](std::string* text) {
        std::string result;

        while (!text->empty()) {
            if (text->front() == '/') {
                *text = text->substr(1);
                result += commander.apply(text);
            }
            else {
                result += OddityEngine::Util::chop(text);
            }
            result += " ";
        }

        OddityEngine::Debug::message("{}", result);
        return "";
    });

    commander.add_command("teleport", [&](std::string* command) {
        player.position.x = std::stof(OddityEngine::Util::chop(command));
        player.position.y = std::stof(OddityEngine::Util::chop(command));
        player.position.z = std::stof(OddityEngine::Util::chop(command));
        return fmt::format("{} {} {}", player.position.x, player.position.y, player.position.z);
    });

    commander.apply("print Hallo Welt, wie geht's dir heute? 42");

    commander.apply("teleport 50 50 50");

    commander.apply("teleport 0 50 50 print teleported back");
    commander.apply("print teleporting to /teleport 50 50 50 so were at the start again");

    do {
        world.update();
        // OddityEngine::Debug::message("Ball Acc: [ {} | {} | {} ]", center_ball.acceleration.x, center_ball.acceleration.y, center_ball.acceleration.z);
        // OddityEngine::Debug::message("Acc: [ {} | {} | {} ]", player.acceleration.x, player.acceleration.y, player.acceleration.z);
    } while (OddityEngine::update());

    for (auto b : balls) {
        delete b;
    }

    balls.clear();

    OddityEngine::terminate();

    return 0;
}
