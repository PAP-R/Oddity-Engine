#ifndef ODDITYENGINE_TRACER_H
#define ODDITYENGINE_TRACER_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
using namespace glm;

#include "Window.h"
#include "Shader.h"
#include "buffer/Buffer.h"
#include "Camera.h"

namespace OddityEngine::Graphics {
    enum Objecttype {
        SPHERE,
        CUBE,
        MESH
    };

    struct alignas(16) bufferobject {
        mat4 transform;
        float radius;
        uint32 type;
        uint32 vertexstart;
        uint32 vertexcount;
    };

    struct alignas(16) buffermaterial {
        vec4 color;
        vec4 emission;
        float roughness;
    };

    class Tracer {
    private:
        Camera * camera;

        GLuint fbo = 0;
        GLuint rbo_color = 0;
        GLuint rbo_depth_stencil = 0;
        Shader::Shader vertex_shader;
        Shader::Shader fragment_shader;
        GLuint program;

        Buffer::Buffer screenbuffer;

        vec<2, int> render_size;

        Window* window;

        vec3 look_at = vec3(0, 0, 1);
        double time;

    public:
        Tracer(Window* window, size_t width, size_t height);
        ~Tracer();
        void update();
    };
}

#endif //ODDITYENGINE_TRACER_H
