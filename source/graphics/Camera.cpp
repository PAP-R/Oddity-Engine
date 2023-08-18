#include "Camera.h"

namespace OddityEngine::Graphics {

    Camera::Camera(vec3 position, quat orientation, float fov) : position(position), orientation(orientation), fov(fov) {

    }

    vec3 Camera::direction() const {
        return orientation * vec3(0, 0, 1) * inverse(orientation);
    }

    vec3 Camera::right() const {
        return orientation * vec3(1, 0, 0) * inverse(orientation);
    }

    vec3 Camera::up() const {
        return orientation * vec3(0, 1, 0) * inverse(orientation);
    }
}