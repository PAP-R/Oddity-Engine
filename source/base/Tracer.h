#ifndef ODDITYENGINE_TRACER_H
#define ODDITYENGINE_TRACER_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

#include "Shader.h"
#include "Buffer.h"
#include "Camera.h"
#include "Loader.h"

enum objecttypes {
    SPHERE,
    MESH,
};

struct alignas(16) bufferobject {
    vec4 color;
    vec4 emission;
    mat4 transform;
    uint32 type;
    uint32 vertexstart;
    uint32 vertexcount;
};

struct alignas(16) buffervertex {
    vec4 pos;
    vec4 color;
    vec4 normal;
    vec2 uv;
};

class Tracer {
private:
    Shader vertex_shader;
    Shader fragment_shader;
    GLuint program;
    Buffer<float> buffer;
    Buffer<bufferobject> objectbuffer;
    Buffer<buffervertex> vertexbuffer;

    size_t bounces = 2;

    float time;

    vector<float> screen;
    vec2 screensize;
    Camera screencamera;
    Camera* camera;

public:
    explicit Tracer(vec2 size, Camera* camera);
    ~Tracer();

    void loop(double dtime);

    static mat4 transform(vec3 translation = vec3(0), vec3 rotation = vec3(0), vec3 scale = vec3(0));

    vector<buffervertex> obj_to_vert(Loader::Object object);
};

#endif //ODDITYENGINE_TRACER_H
