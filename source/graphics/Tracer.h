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
#include "util/Loader.h"

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
        uint32 material;
        uint32 vertexstart;
        uint32 vertexcount;
    };

    struct alignas(16) buffermaterial {
        vec4 color;
        vec4 emission;
        float roughness;
    };

    struct alignas(16) buffervertex {
        vec4 pos;
        vec4 color;
        vec4 normal;
        vec2 uv;
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

        int bounces = 2;
        int spread = 2;
        float cull = 2;
        float split = 0;

        Buffer::Buffer screenbuffer = Buffer::Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

        vec<2, int> render_size;

        Window* window;

        vec3 look_at = vec3(0, 0, 1);
        double time;

    public:
        Buffer::Buffer objectbuffer = Buffer::Buffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW);
        Buffer::Buffer materialbuffer = Buffer::Buffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW);
        Buffer::Buffer vertexbuffer = Buffer::Buffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW);

        Tracer(Window* window, size_t width, size_t height);
        ~Tracer();
        void update();

        static std::vector<buffervertex> obj_to_vert(Loader::Object object);
    };
}

#endif //ODDITYENGINE_TRACER_H
