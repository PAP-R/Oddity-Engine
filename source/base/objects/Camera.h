#ifndef ODDITYENGINE_CAMERA_H
#define ODDITYENGINE_CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

class Camera {
public:
    vec3 position;
    vec2 angle;

    float fov;

    explicit Camera(vec3 position=vec3(0), vec2 angle=vec2(0), float fov=float(45));

    [[nodiscard]] vec3 direction() const;
    [[nodiscard]] vec3 right() const;
    [[nodiscard]] vec3 up() const;
};


#endif //ODDITYENGINE_CAMERA_H
