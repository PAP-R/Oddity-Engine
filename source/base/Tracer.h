#ifndef ODDITYENGINE_TRACER_H
#define ODDITYENGINE_TRACER_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
using namespace glm;

#include "Shader.h"
#include "Buffer.h"
#include "Camera.h"

struct alignas(16) bufferobject {
    vec4 color;
    vec4 emission;
    vec4 pos;
    vec4 scale;
    uint32 type;
};

class Tracer {
private:
    Shader vertex_shader;
    Shader fragment_shader;
    GLuint program;
    Buffer<float> buffer;
    Buffer<bufferobject> objectbuffer;

    size_t bounces = 4;

    float time;

    vector<float> screen;
    vec2 screensize;
    Camera screencamera;
    Camera* camera;

public:
    explicit Tracer(vec2 size, Camera* camera);
    ~Tracer();

    void loop(double dtime);
};

#endif //ODDITYENGINE_TRACER_H
