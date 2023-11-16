#ifndef ODDITYENGINE_CAMERA_H
#define ODDITYENGINE_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Physics/Object.h>

namespace OddityEngine {
    namespace Graphics {
        class Camera : public OddityEngine::Physics::Object {
        public:
            float fov;

            explicit Camera(glm::vec3 position = {0, 0, 0}, glm::vec3 angle = {0, 0, 0}, float fov = 90);
            explicit Camera(float fov);
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_CAMERA_H
