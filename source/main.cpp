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

void create_commander(OddityEngine::Util::Commander& commander, Player& player) {
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
        return fmt::format("{}\n[{}, {}, {}]\n[{}, {}, {}]\n[{}, {}, {}]", OddityEngine::Util::Time::fps<size_t>(), player.position.x, player.position.y, player.position.z, player.velocity.x, player.velocity.y, player.velocity.z, player.acceleration.x, player.acceleration.y, player.acceleration.z);
    });

    commander.add_command("reset", [&](std::string* command){
        player.position = {0, 0, 0, 1};
        player.velocity = {0, 0, 0, 1};
        player.acceleration = {0, 0, 0, 1};
        return "";
    });

    commander.add_command("shift", [&](std::string* command) {
        player.camera_shift.x = std::stof(OddityEngine::Util::chop(command));
        player.camera_shift.y = std::stof(OddityEngine::Util::chop(command));
        player.camera_shift.z = std::stof(OddityEngine::Util::chop(command));
        return fmt::format("{} {} {}", player.camera_shift.x, player.camera_shift.y, player.camera_shift.z);
    });
}

int main(int argc, char* args[]) {
    OddityEngine::init();

    auto window = OddityEngine::Graphics::Window("Hallo", 800, 600, SDL_WINDOW_RESIZABLE);

    std::vector<std::string> files = {"test.frag"};

    OddityEngine::Graphics::Shader shader(GL_FRAGMENT_SHADER);

    for (const auto& f : files) {
        shader.add(OddityEngine::Graphics::Shader::read_shader(f));
    }

    OddityEngine::Debug::message("Shadercode:\n{}", shader.compile());


    OddityEngine::Graphics::Scene scene;
    window.set_scene(&scene);

    OddityEngine::Graphics::Camera camera({0, 10, 10});

    Player player(&camera);
    scene.add_eventable(&player);

    auto renderer = new OddityEngine::Graphics::Render::Layered(player.camera);
    scene.add_renderer(renderer);
    auto renderer2 = new OddityEngine::Graphics::Render::ComputeRenderer(player.camera);
//    scene.add_renderer(renderer2);

    // renderer->set_size({11, 11});

    // player.position = glm::vec4(0, 50, 50, 1);

    player.radius = 1;
    player.mass = 1;

    player.camera->fov = 100;

    player.state |= OddityEngine::Physics::MOVE | OddityEngine::Physics::CLIP;

    // player.state &= ~OddityEngine::Physics::SHOW;

     player.state &= ~OddityEngine::Physics::CLIP;
     player.state &= ~OddityEngine::Physics::MOVE;

    // player.angle_velocity.x = 10;
    // player.angle_velocity.y = 1;

    OddityEngine::Physics::World world;

    renderer->world = &world;
//    renderer2->world = &world;

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

    OddityEngine::Util::Commander commander;
    create_commander(commander, player);

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

    input.add_action("GeneralCheck", [](const SDL_Event& event){OddityEngine::Debug::message("Event Type : {}", event.type);});
    input.add_mapping("GeneralCheck", SDL_JOYAXISMOTION);
    input.add_mapping("GeneralCheck", SDL_JOYBUTTONUP);
    input.add_mapping("GeneralCheck", SDL_CONTROLLERDEVICEADDED);
    input.add_mapping("GeneralCheck", SDL_CONTROLLERAXISMOTION);
    input.add_mapping("GeneralCheck", SDL_CONTROLLERBUTTONDOWN);

    input.add_action("ControllerButton", [&](const SDL_Event& event){OddityEngine::Debug::message("Pressed Controller Button {}", event.jbutton.button);});
    for (int i = 0; i < 16; i++) input.add_mapping("ControllerButton", SDL_JOYBUTTONDOWN, i);

    input.add_action("NumJoysticks", [&](const SDL_Event& event) {
        for (size_t i = 0; i < SDL_NumJoysticks(); i++) {
            OddityEngine::Debug::message("Joystick {} : {}", i, SDL_JoystickNameForIndex(i));
        }
    });
    input.add_mapping("NumJoysticks", SDL_KEYDOWN, SDLK_n);

    std::vector<SDL_Joystick*> joysticks;

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

    OddityEngine::Util::Time::reset();

    player.velocity = {0, 1, 0, 1};

    do {
        world.update();
    } while (OddityEngine::update());

    OddityEngine::terminate();

    return 0;
}
