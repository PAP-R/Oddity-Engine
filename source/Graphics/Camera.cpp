#include "Camera.h"

#include <math.h>

namespace OddityEngine {
    namespace Graphics {
        Camera::Camera(glm::vec3 position, glm::vec3 angle, float fov) : Object(position, angle), fov(fov) {
        }

        Camera::Camera(float fov) : Camera() {
            this->fov = fov;
        }
    } // OddityEngine
} // Graphics