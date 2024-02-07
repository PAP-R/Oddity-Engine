#include "Player.h"

#include <SDL.h>

Player::Player(OddityEngine::Graphics::Camera* camera) : camera(camera) {}

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
    }
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
