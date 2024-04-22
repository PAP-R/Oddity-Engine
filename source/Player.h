#ifndef PLAYER_H
#define PLAYER_H

#include "Util/Eventable.h"
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
    bool update(void* context) override;

    OddityEngine::Graphics::Camera* camera;
    glm::vec3 camera_shift = {0, 0, 5};

    glm::vec4 closest(glm::vec3 point) override;
    float distance(glm::vec3 point) override;

    void set_acceleration_front(float scale);
    void set_acceleration_right(float scale);
    void set_acceleration_up(float scale);
    void add_acceleration_front(float scale);
    void add_acceleration_right(float scale);
    void add_acceleration_up(float scale);
};



#endif //PLAYER_H
