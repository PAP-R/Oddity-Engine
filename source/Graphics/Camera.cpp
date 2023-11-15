#include "Camera.h"

#include <math.h>

namespace OddityEngine {
    namespace Graphics {
        Camera::Camera(glm::vec3 position, glm::vec3 angle, glm::quat orientation, float fov) : position(position), angle(angle), orientation(orientation), fov(fov) {

        }

        Camera::Camera(float fov) : Camera() {
            this->fov = fov;
        }

        void Camera::normalize() {
            for (int i = 0; i < angle.length(); i++) {
                angle[i] = fmod(angle[i], 360);
            }

            orientation = glm::angleAxis(glm::radians(angle.y), glm::vec3(1, 0, 0));
            orientation *= glm::angleAxis(glm::radians(angle.x), glm::vec3(0, 1, 0));
        }

        glm::vec3 qrot(glm::quat q, glm::vec3 v) {
            glm::vec3 qv(q.x, q.y, q.z);
            return v + 2.0f * glm::cross(qv, cross(qv, v) + q.w * v);
        }

        glm::vec3 Camera::front() {
            normalize();
            return qrot(orientation, glm::vec3(0, 0, 1));
        }

        glm::vec3 Camera::right() {
            normalize();
            return qrot(orientation, glm::vec3(1, 0, 0));
        }

        glm::vec3 Camera::up() {
            normalize();
            return qrot(orientation, glm::vec3(0, 1, 0));
        }
    } // OddityEngine
} // Graphics