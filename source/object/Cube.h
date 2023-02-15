#ifndef ODDITYENGINE_CUBE_H
#define ODDITYENGINE_CUBE_H

#include "source/base/objects/Graphics.h"
#include "source/base/objects/Physics.h"

class Cube : public Physics, public Graphics {
public:
    explicit Cube(const vec3 &pos=vec3(0), const vec3 &dir=vec3(0, 1, 0), const vec3 &scale=vec3(1, 1, 1), const string &vertexShader=loadShader("shaders/vert.shader"), const string &fragmentShader=loadShader("shaders/wavefrag.shader"));

    void loop(float deltaSeconds);
};


#endif //ODDITYENGINE_CUBE_H
