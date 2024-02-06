#ifndef CAMERA_H
#define CAMERA_H

#include <Physics/Object.h>

namespace OddityEngine::Graphics {
    class Camera : public Physics::Object {
    public:
        float fov;

        explicit Camera(glm::vec3 position = {0, 0, 0}, glm::vec3 angle = {0, 0, 0}, float fov = 90);
        explicit Camera(float fov);
    };
}


#endif //CAMERA_H
