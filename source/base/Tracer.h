#ifndef ODDITYENGINE_TRACER_H
#define ODDITYENGINE_TRACER_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
using namespace glm;

#include "Shader.h"
#include "Buffer.h"
#include "Camera.h"

class Tracer {
private:
    Shader vertex_shader;
    Shader fragment_shader;
    GLuint program;
    Buffer<float> buffer;
    Buffer<float> spherebuffer;

    float time;

    vector<float> screen;
    vec2 screensize;
    Camera* camera;

public:
    explicit Tracer(vec2 size, Camera* camera);
    ~Tracer();

    void loop(double dtime);
};


#endif //ODDITYENGINE_TRACER_H
