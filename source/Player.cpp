#include "Player.h"

#include <SDL.h>
#include <glm/gtx/quaternion.hpp>

#include "Math/basics.h"
#include "Util/Debug.h"
#include "Util/Time.h"

Player::Player(OddityEngine::Graphics::Camera* camera) : camera(camera) {
    add_action("MoveForward", [&](const SDL_Event& event){this->add_acceleration_front(1);});
    add_action("MoveBack", [&](const SDL_Event& event){this->add_acceleration_front(-1);});
    add_action("MoveRight", [&](const SDL_Event& event){this->add_acceleration_right(1);});
    add_action("MoveLeft", [&](const SDL_Event& event){this->add_acceleration_right(-1);});
    add_action("MoveUp", [&](const SDL_Event& event){this->add_acceleration_up(1);});
    add_action("MoveDown", [&](const SDL_Event& event){this->add_acceleration_up(-1);});

    add_mapping("MoveForward", SDL_KEYDOWN, SDL_GetKeyFromName("w"));
    add_mapping("MoveBack", SDL_KEYDOWN, SDL_GetKeyFromName("s"));
    add_mapping("MoveRight", SDL_KEYDOWN, SDL_GetKeyFromName("d"));
    add_mapping("MoveLeft", SDL_KEYDOWN, SDL_GetKeyFromName("a"));
    add_mapping("MoveUp", SDL_KEYDOWN, SDLK_SPACE);
    add_mapping("MoveDown", SDL_KEYDOWN, SDLK_LCTRL);

    add_mapping("MoveForward", SDL_KEYUP, SDL_GetKeyFromName("s"));
    add_mapping("MoveBack", SDL_KEYUP, SDL_GetKeyFromName("w"));
    add_mapping("MoveRight", SDL_KEYUP, SDL_GetKeyFromName("a"));
    add_mapping("MoveLeft", SDL_KEYUP, SDL_GetKeyFromName("d"));
    add_mapping("MoveUp", SDL_KEYUP, SDLK_LCTRL);
    add_mapping("MoveDown", SDL_KEYUP, SDLK_SPACE);
}

bool Player::update() {
    camera->position = position + glm::toMat4(orientation) * glm::vec4(camera_shift, 1);
    camera->position.w = 1;
    camera->angle = angle;
    camera->normalize();

    return true;
}

bool Player::update(void* context) {
    auto result = Pawn::update(context);
    return update() && result;
}
