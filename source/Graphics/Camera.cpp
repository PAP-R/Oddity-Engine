#include "Camera.h"

namespace OddityEngine::Graphics {
    Camera::Camera(glm::vec3 position, glm::vec3 angle, float fov) : Object(position, angle), fov(fov) {
    }

    Camera::Camera(const float fov) : fov(fov) {
    }
}
