#ifndef PLAYER_H
#define PLAYER_H

#include <Util/Eventable.h>
#include <Physics/Object.h>

#include "Graphics/Camera.h"

class Player : public OddityEngine::Physics::Object, public OddityEngine::Util::Eventable {
protected:
    glm::vec3 delta_acceleration = {0, 0, 0};
    glm::vec3 manual_acceleration = {40, 40, 40};
    glm::vec3 mouse_sensitivity = {0.1, 0.1, 0.1};

public:
    Player(OddityEngine::Graphics::Camera* camera = new OddityEngine::Graphics::Camera());

    void event(const SDL_Event& event) override;

    bool update() override;

    OddityEngine::Graphics::Camera* camera;
    glm::vec3 camera_shift = {0, 0, 5};

    glm::vec3 closest(glm::vec3 point) override;
    float distance(glm::vec3 point) override;
};



#endif //PLAYER_H
