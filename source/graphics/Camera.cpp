#include "Camera.h"

namespace OddityEngine::Graphics {

    Camera::Camera(vec3 position, vec3 angle, quat orientation, float fov) : position(position), angle(angle), orientation(orientation), fov(fov) {

    }

    void Camera::normalize() {

        for (int i = 0; i < this->angle.length(); i++) {
            if (this->angle[i] > 360) {
                this->angle[i] -= 720;
            }
            if (this->angle[i] < -360) {
                this->angle[i] += 720;
            }
        }


        this->orientation = glm::angleAxis(glm::radians(this->angle.y), vec3(1, 0, 0));
        this->orientation *= glm::angleAxis(glm::radians(this->angle.x), vec3(0, 1, 0));

//        this->orientation = glm::normalize(this->orientation);
    }

    vec3 Camera::direction() {
        normalize();
        return orientation * vec3(0, 0, 1);
    }

    vec3 Camera::right() {
        normalize();
        return orientation * vec3(1, 0, 0);
    }

    vec3 Camera::up() {
        normalize();
        return orientation * vec3(0, 1, 0);
    }
}