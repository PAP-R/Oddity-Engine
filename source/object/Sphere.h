#ifndef ODDITYENGINE_SPHERE_H
#define ODDITYENGINE_SPHERE_H


#include "source/base/objects/Graphics.h"
#include "source/base/objects/Physics.h"

class Sphere : public Physics, public Graphics {
    explicit Sphere(const vec3 &pos=vec3(0), const vec3 &dir=vec3(0, 1, 0), const vec3 &scale=vec3(1, 1, 1), const std::string &vertexShader=loadShader("shaders/vert.shader"), const std::string &fragmentShader=loadShader("shaders/wavefrag.shader"));
};


#endif //ODDITYENGINE_SPHERE_H
