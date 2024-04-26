#include <steam_api.h>

#include <thread>
#include <future>
#include <chrono>
#include <complex>

#include "Util/CombinedIndex.h"


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
#include <Input/Input.h>

int main(int argc, char* args[]) {
    OddityEngine::init();

    auto window = OddityEngine::Graphics::Window("Hallo", 800, 600, SDL_WINDOW_RESIZABLE);



    std::vector<std::string> files = {"test.frag"};

    OddityEngine::Graphics::Shader shader(GL_FRAGMENT_SHADER);

    for (const auto& f : files) {
        shader.add(OddityEngine::Graphics::Shader::read_shader(f));

        OddityEngine::Debug::message("Shadercode:\n{}", shader.compile());
    }



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
    center_ball.shape = OddityEngine::Physics::CUBE;
    center_ball.state &= ~OddityEngine::Physics::MOVE;

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
    float height = 12;
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
//    OddityEngine::Debug::message("World: [ {} | {} | {} ]", earth.position.x, earth.position.y, earth.position.z);

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

    commander.add_command("stat", [&](std::string* command){
        return fmt::format("{}", OddityEngine::Util::Time::fps<size_t>());
    });

//    commander.apply("print Hallo Welt, wie geht's dir heute? 42");
//
//    commander.apply("teleport 50 50 50");
//
//    commander.apply("teleport 0 50 50 print teleported back");
    commander.apply("print teleporting to /teleport 0 10 10");

//    OddityEngine::Vector<GLuint> indices = {6, 17, 20};
//    auto indices_simplified = OddityEngine::Util::CombinedIndex::generate(indices);
//    auto indices_restored = OddityEngine::Util::CombinedIndex::get(indices_simplified);
//
//    OddityEngine::Debug::message("Simplified: {}\t{}\t{}\t{}", indices_simplified.offset, indices_simplified.base, indices_simplified.count, indices_simplified.indices);
//
//    std::cout << indices << std::endl << indices_restored << std::endl;

    OddityEngine::Input::Input input;
    scene.add_eventable(&input);

    input.add_action("CaptureMouse", [](const SDL_Event& event){SDL_SetRelativeMouseMode(SDL_GetRelativeMouseMode() == SDL_FALSE ? SDL_TRUE : SDL_FALSE);});
    input.add_mapping("CaptureMouse", SDL_KEYDOWN, SDL_GetKeyFromName("`"));

    input.add_action("KeyCheck", [](const SDL_Event& event){OddityEngine::Debug::message("Hallo, sie haben {} gedrückt. Key sagt {}, aber Button sagt {}", SDL_GetKeyName(event.key.keysym.sym), event.key.keysym.sym, event.button.button);});
    input.add_mapping("KeyCheck", SDL_KEYDOWN, SDLK_q);
    input.add_mapping("KeyCheck", SDL_KEYDOWN, SDL_GetKeyFromName("E"));
    input.add_mapping("KeyCheck", SDL_MOUSEBUTTONDOWN, 1);
    input.add_mapping("KeyCheck", SDL_MOUSEBUTTONDOWN, 2);
    input.add_mapping("KeyCheck", SDL_MOUSEBUTTONDOWN, 3);
    input.add_mapping("KeyCheck", SDL_MOUSEBUTTONDOWN, 4);
    input.add_mapping("KeyCheck", SDL_MOUSEBUTTONDOWN, 5);

    input.add_action("MouseMove", [&](const SDL_Event& event){player.turn(event.motion.xrel, event.motion.yrel);});
    input.add_mapping("MouseMove", SDL_MOUSEMOTION);

    input.add_action("GeneralCheck", [](const SDL_Event& event){OddityEngine::Debug::message("Event Type : {}", event.type);});
    input.add_mapping("GeneralCheck", SDL_JOYAXISMOTION);
    input.add_mapping("GeneralCheck", SDL_JOYBUTTONUP);
    input.add_mapping("GeneralCheck", SDL_CONTROLLERDEVICEADDED);
    input.add_mapping("GeneralCheck", SDL_CONTROLLERAXISMOTION);
    input.add_mapping("GeneralCheck", SDL_CONTROLLERBUTTONDOWN);

    input.add_action("MoveForward", [&](const SDL_Event& event){player.add_acceleration_front(1);});
    input.add_action("MoveBack", [&](const SDL_Event& event){player.add_acceleration_front(-1);});
    input.add_action("MoveRight", [&](const SDL_Event& event){player.add_acceleration_right(1);});
    input.add_action("MoveLeft", [&](const SDL_Event& event){player.add_acceleration_right(-1);});
    input.add_action("MoveUp", [&](const SDL_Event& event){player.add_acceleration_up(1);});
    input.add_action("MoveDown", [&](const SDL_Event& event){player.add_acceleration_up(-1);});

    input.add_action("MoveForwardAxis", [&](const SDL_Event& event){player.set_acceleration_front(-(event.jaxis.value + 0.5f) / CONTROLLER_AXIS_MAX);});
    input.add_mapping("MoveForwardAxis", SDL_JOYAXISMOTION, 1);

    input.add_action("MoveRightAxis", [&](const SDL_Event& event){player.set_acceleration_right((event.jaxis.value + 0.5f) / CONTROLLER_AXIS_MAX);});
    input.add_mapping("MoveRightAxis", SDL_JOYAXISMOTION, 0);

    input.add_action("ControllerButton", [&](const SDL_Event& event){OddityEngine::Debug::message("Pressed Controller Button {}", event.jbutton.button);});
    for (int i = 0; i < 16; i++) input.add_mapping("ControllerButton", SDL_JOYBUTTONDOWN, i);

    input.add_action("NumJoysticks", [&](const SDL_Event& event) {
        for (size_t i = 0; i < SDL_NumJoysticks(); i++) {
            OddityEngine::Debug::message("Joystick {} : {}", i, SDL_JoystickNameForIndex(i));
        }
    });
    input.add_mapping("NumJoysticks", SDL_KEYDOWN, SDLK_n);

    std::vector<SDL_Joystick*> joysticks;

//    for (size_t i = 0; i < SDL_NumJoysticks(); i++) {
//        joysticks.push_back(SDL_JoystickOpen(i));
//        OddityEngine::Debug::message("Connected Joystick {} : {}", i, SDL_JoystickName(joysticks.back()));
//    }

    input.add_action("JoystickAdd", [&](const SDL_Event& event){
        joysticks.push_back(SDL_JoystickOpen(event.jdevice.which));
        OddityEngine::Debug::message("Connected Joystick {} : {}", event.jdevice.which, SDL_JoystickName(joysticks.back()));
    });
    input.add_mapping("JoystickAdd", SDL_JOYDEVICEADDED);

    input.add_action("JoystickRemove", [&](const SDL_Event& event){
        OddityEngine::Debug::message("Removed Joystick {} : {}", event.jdevice.which, SDL_JoystickNameForIndex(event.jdevice.which));
        SDL_JoystickClose(SDL_JoystickFromInstanceID(event.jdevice.which));
        joysticks.erase(std::remove(joysticks.begin(), joysticks.end(), SDL_JoystickFromInstanceID(event.jdevice.which)), joysticks.end());
    });
    input.add_mapping("JoystickRemove", SDL_JOYDEVICEREMOVED);


    std::vector<SDL_GameController*> controllers;

    input.add_action("ControllerAdd", [&](const SDL_Event& event){
        controllers.push_back(SDL_GameControllerOpen(event.cdevice.which));
        OddityEngine::Debug::message("Connected Controller {} : {}", event.cdevice.which, SDL_GameControllerName(controllers.back()));
    });
    input.add_mapping("ControllerAdd", SDL_CONTROLLERDEVICEADDED);

    input.add_action("ControllerRemove", [&](const SDL_Event& event){
        OddityEngine::Debug::message("Removed Controller {} : {}", event.jdevice.which, SDL_GameControllerNameForIndex(event.cdevice.which));
        SDL_JoystickClose(SDL_JoystickFromInstanceID(event.cdevice.which));
        controllers.erase(std::remove(controllers.begin(), controllers.end(), SDL_GameControllerFromInstanceID(event.cdevice.which)), controllers.end());
    });
    input.add_mapping("ControllerRemove", SDL_CONTROLLERDEVICEREMOVED);


//    SDL_JoystickRumbleTriggers();

    input.add_mapping("MoveForward", SDL_KEYDOWN, SDL_GetKeyFromName("w"));
    input.add_mapping("MoveBack", SDL_KEYDOWN, SDL_GetKeyFromName("s"));
    input.add_mapping("MoveRight", SDL_KEYDOWN, SDL_GetKeyFromName("d"));
    input.add_mapping("MoveLeft", SDL_KEYDOWN, SDL_GetKeyFromName("a"));
    input.add_mapping("MoveUp", SDL_KEYDOWN, SDLK_SPACE);
    input.add_mapping("MoveDown", SDL_KEYDOWN, SDLK_LCTRL);

    input.add_mapping("MoveForward", SDL_KEYUP, SDL_GetKeyFromName("s"));
    input.add_mapping("MoveBack", SDL_KEYUP, SDL_GetKeyFromName("w"));
    input.add_mapping("MoveRight", SDL_KEYUP, SDL_GetKeyFromName("a"));
    input.add_mapping("MoveLeft", SDL_KEYUP, SDL_GetKeyFromName("d"));
    input.add_mapping("MoveUp", SDL_KEYUP, SDLK_LCTRL);
    input.add_mapping("MoveDown", SDL_KEYUP, SDLK_SPACE);



    OddityEngine::Input::Input text_input;
    text_input.disable();
    scene.add_eventable(&text_input);

    input.add_action("TextEnter", [&](const SDL_Event& event){OddityEngine::Debug::message("Swapped to text input"); text_input.enable(); input.disable();});
    input.add_mapping("TextEnter", SDL_KEYDOWN, SDLK_RETURN);

    std::string input_text;

    text_input.add_action("Text", [&](const SDL_Event& event){ input_text += event.text.text; OddityEngine::Debug::message("{}", input_text);});
    text_input.add_mapping("Text", SDL_TEXTINPUT);
    text_input.add_action("TextBack", [&](const SDL_Event& event){if (!input_text.empty()) input_text.pop_back(); OddityEngine::Debug::message("{}", input_text);});
    text_input.add_mapping("TextBack", SDL_KEYDOWN, SDLK_BACKSPACE);
    text_input.add_action("TextEnter", [&](const SDL_Event& event){OddityEngine::Debug::message("Applying command and swapping back {}", commander.apply(input_text)); input_text.clear(); input.enable(); text_input.disable();});
    text_input.add_mapping("TextEnter", SDL_KEYDOWN, SDLK_RETURN);

    do {
        center_ball.orientation.x = cos(OddityEngine::Util::Time::now<float>());
        center_ball.orientation.y = cos(OddityEngine::Util::Time::now<float>());
        center_ball.orientation.z = sin(OddityEngine::Util::Time::now<float>());
        center_ball.orientation.w = sin(OddityEngine::Util::Time::now<float>());
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
