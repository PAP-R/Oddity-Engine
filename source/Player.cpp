#include "Player.h"

#include <SDL.h>
#include <glm/gtx/quaternion.hpp>

#include "Math/basics.h"
#include "Util/Debug.h"

Player::Player(OddityEngine::Graphics::Camera* camera) : camera(camera) {
    mass = 0;
}

void Player::event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            if (!event.key.repeat) {
                switch(event.key.keysym.sym) {
                    case SDLK_w:
                        delta_acceleration.z += manual_acceleration.z;
                    break;
                    case SDLK_s:
                        delta_acceleration.z -= manual_acceleration.z;
                    break;
                    case SDLK_a:
                        delta_acceleration.x -= manual_acceleration.x;
                    break;
                    case SDLK_d:
                        delta_acceleration.x += manual_acceleration.x;
                    break;
                    case SDLK_SPACE:
                        delta_acceleration.y += manual_acceleration.y;
                    break;
                    case SDLK_LCTRL:
                        delta_acceleration.y -= manual_acceleration.y;
                    break;
                    case SDLK_r:
                        // position = {0, 0, 0, 1};
                        velocity = {0, 0, 0, 1};
                        acceleration = {0, 0, 0, 1};
                        // angle = {0, 0, 0, 1};
                        angle_velocity = {0, 0, 0, 1};
                        angle_acceleration = {0, 0, 0, 1};
                        break;
                }
            }
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.sym) {
                case SDLK_w:
                    delta_acceleration.z -= manual_acceleration.z;
                break;
                case SDLK_s:
                    delta_acceleration.z += manual_acceleration.z;
                break;
                case SDLK_a:
                    delta_acceleration.x += manual_acceleration.x;
                break;
                case SDLK_d:
                    delta_acceleration.x -= manual_acceleration.x;
                break;
                case SDLK_SPACE:
                    delta_acceleration.y -= manual_acceleration.y;
                break;
                case SDLK_LCTRL:
                    delta_acceleration.y += manual_acceleration.y;
                break;
            }
            break;

        case SDL_MOUSEMOTION:
            angle.x += -event.motion.xrel * mouse_sensitivity.x;
            angle.y += -event.motion.yrel * mouse_sensitivity.y;
            break;

        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    OddityEngine::Debug::message("State: [ {} | {} ]", state & OddityEngine::Physics::SHOW, state & OddityEngine::Physics::HIT);
                    OddityEngine::Debug::message("Position: [ {} | {} | {} ]", position.x, position.y, position.z);
                    OddityEngine::Debug::message("Velocity: [ {} | {} | {} ]", velocity.x, velocity.y, velocity.z);
                    OddityEngine::Debug::message("Acceleration: [ {} | {} | {} ]", acceleration.x, acceleration.y, acceleration.z);
                    break;
                case SDL_BUTTON_RIGHT:
                    for (size_t j = 0; j < 10; j++) {
                        if (j == 0) {
                            fmt::print("[");
                        }
                        else {
                            fmt::print("|");
                        }
                        fmt::print(" {}: {} ", j, test_value[j]);
                    }
                    fmt::print("]\n");
                    break;
                case SDL_BUTTON_MIDDLE:
                    camera->fov = 90;
                    camera_shift.z = 5;
                    OddityEngine::Debug::message("FOV: {}", camera->fov);
                    break;
            }
            break;

        case SDL_MOUSEWHEEL:
            camera->fov = OddityEngine::Math::min(OddityEngine::Math::max(camera->fov - event.wheel.preciseY, 1.0f), 180.0f);
            camera_shift.z = OddityEngine::Math::min(OddityEngine::Math::max(camera_shift.z - event.wheel.preciseX, 0.0f), 20.0f);
            OddityEngine::Debug::message("FOV: {}", camera->fov);
            OddityEngine::Debug::message("Shift: {}", camera_shift.z);
            break;
    }
}

bool Player::update() {
    acceleration = glm::vec4(glm::mat3(right(), glm::vec3(0, 1, 0), front()) * delta_acceleration, 1);

    normalize();

    camera->position = position + glm::toMat4(orientation) * glm::vec4(camera_shift, 1);
    camera->position.w = 1;
    camera->angle = angle;
    camera->normalize();

    return true;
}

glm::vec3 Player::closest(const glm::vec3 point) {
    const glm::vec3 pa = point - glm::vec3(position);
    const glm::vec3 ba = camera_shift - glm::vec3(position);
    const float h = glm::clamp(dot(pa,ba)/dot(ba,ba), 0.0f, 1.0f);
    const glm::vec3 from_point = pa - ba * h;
    const float len = length(from_point);

    return point + glm::normalize(from_point) * (len - radius);
}

float Player::distance(const glm::vec3 point) {
    return glm::length(closest(point));
}
