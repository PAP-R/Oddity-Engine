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
        quat orientation;

        float fov;
        explicit Camera(vec3 position=vec3(0), quat orientation = quat(), float fov = 90);

        [[nodiscard]] vec3 direction() const;
        [[nodiscard]] vec3 right() const;
        [[nodiscard]] vec3 up() const;
    };
}


#endif //ODDITYENGINE_CAMERA_H
