#ifndef ODDITYENGINE_CAMERA_H
#define ODDITYENGINE_CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

class Camera {
public:
    vec3 position = vec3(0);
    vec2 angle = vec2(0);

    float fov = 45.0f;

    Camera(const vec3 &pos=vec3(0), const vec2 &angle=vec2(0), float pov=45.0f);

    vec3 direction() const;
    vec3 right() const;
    vec3 up() const;
};


#endif //ODDITYENGINE_CAMERA_H
