#ifndef ODDITYENGINE_CAMERA_H
#define ODDITYENGINE_CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

using namespace glm;

namespace OddityEngine::Graphics {
    class Camera {
    public:
        vec3 position;
        vec3 angle;
        quat orientation;

        float fov;
        explicit Camera(vec3 position=vec3(0), vec3 angle = vec3(0), quat orientation = quat(1, 0, 0, 0), float fov = 90);

        void normalize();

        [[nodiscard]] vec3 direction();
        [[nodiscard]] vec3 right();
        [[nodiscard]] vec3 up();
    };
}


#endif //ODDITYENGINE_CAMERA_H
