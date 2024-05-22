#ifndef PAWN_H
#define PAWN_H

#include "Util/Eventable.h"
#include <Physics/Object.h>

#include "Graphics/Camera.h"

namespace OddityEngine::Util {
    class Pawn : public OddityEngine::Physics::Object {
    protected:
        glm::vec3 delta_acceleration = {0, 0, 0};
        glm::vec3 manual_acceleration = {40, 40, 40};
        glm::vec3 mouse_sensitivity = {0.1, 0.1, 0.1};

    public:
        bool update() override;

        bool update(void *context) override;

        void set_acceleration_front(float scale);

        void set_acceleration_right(float scale);

        void set_acceleration_up(float scale);

        void add_acceleration_front(float scale);

        void add_acceleration_right(float scale);

        void add_acceleration_up(float scale);

        void turn(float right, float up);

        void turn_up(float scale);

        void turn_right(float scale);
    };
}


#endif //PAWN_H
