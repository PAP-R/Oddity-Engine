#include "Pawn.h"

#include <SDL.h>
#include <glm/gtx/quaternion.hpp>

#include "Math/basics.h"
#include "Util/Debug.h"
#include "Util/Time.h"

namespace OddityEngine::Util {
    bool Pawn::update() {
        acceleration += glm::vec4(glm::mat3(right(), up(), front()) * delta_acceleration, 1);

        normalize();

        return true;
    }

    bool Pawn::update(void *context) {
        update();
        return Object::update(context);
    }

    void Pawn::set_acceleration_front(float scale) {
        delta_acceleration.z = manual_acceleration.z * scale;
    }

    void Pawn::set_acceleration_right(float scale) {
        delta_acceleration.x = manual_acceleration.x * scale;
    }

    void Pawn::set_acceleration_up(float scale) {
        delta_acceleration.y = manual_acceleration.y * scale;
    }

    void Pawn::add_acceleration_front(float scale) {
        delta_acceleration.z += manual_acceleration.z * scale;
    }

    void Pawn::add_acceleration_right(float scale) {
        delta_acceleration.x += manual_acceleration.x * scale;
    }

    void Pawn::add_acceleration_up(float scale) {
        delta_acceleration.y += manual_acceleration.y * scale;
    }

    void Pawn::turn(float right, float up) {
        angle.x += -right * mouse_sensitivity.x;
        angle.y += -up * mouse_sensitivity.y;
    }

    void Pawn::turn_up(float scale) {
        turn(0, scale);
    }

    void Pawn::turn_right(float scale) {
        turn(scale, 0);
    }
}