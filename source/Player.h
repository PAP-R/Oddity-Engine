#ifndef PLAYER_H
#define PLAYER_H

#include "Util/Eventable.h"
#include <Physics/Object.h>

#include <Util/Pawn.h>
#include <Input/Input.h>

#include "Graphics/Camera.h"

class Player : public OddityEngine::Util::Pawn, public OddityEngine::Input::Input {
protected:
    glm::vec3 manual_acceleration = {40, 40, 40};
    glm::vec3 mouse_sensitivity = {0.1, 0.1, 0.1};

public:
    explicit Player(OddityEngine::Graphics::Camera* camera = new OddityEngine::Graphics::Camera());

    bool update() override;
    bool update(void* context) override;

    OddityEngine::Graphics::Camera* camera;
    glm::vec3 camera_shift = {0, 0, 5};
};



#endif //PLAYER_H
