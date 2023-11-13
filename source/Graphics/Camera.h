#ifndef ODDITYENGINE_CAMERA_H
#define ODDITYENGINE_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace OddityEngine {
    namespace Graphics {
        class Camera {
        public:
            glm::vec3 position;
            glm::vec3 angle;
            glm::quat orientation;

            float fov;

            explicit Camera(glm::vec3 position = {0, 0, 0}, glm::vec3 angle = {0, 0, 0}, glm::quat orientation = {1, 0, 0, 0}, float fov = 90);
            explicit Camera(float fov);

            void normalize();

            [[nodiscard]] glm::vec3 front();
            [[nodiscard]] glm::vec3 right();
            [[nodiscard]] glm::vec3 up();
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_CAMERA_H
