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
        MESH,
        CURVE
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

        GLuint frame_buffer = 0;
        GLuint render_texture = 0;

        GLuint rbo_color = 0;
        GLuint rbo_depth_stencil = 0;
        Shader::Shader vertex_shader = Shader::Shader(GL_VERTEX_SHADER, "shaders/ray.vert");
        Shader::Shader fragment_shader = Shader::Shader(GL_FRAGMENT_SHADER, "shaders/ray.frag");
        GLuint program;

        Shader::Shader view_vertex_shader = Shader::Shader(GL_VERTEX_SHADER, "shaders/view.vert");
        Shader::Shader view_fragment_shader = Shader::Shader(GL_FRAGMENT_SHADER, "shaders/view.frag");
        GLuint view_program;

        int bounces = 1;
        int spread = 1;
        float cull = 2;
        float split = 0;

        float ratio = 0.5;
        float last_ratio = ratio;

        Buffer::Buffer screenbuffer = Buffer::Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

        vec<2, int> render_size;
        vec<2, int> view_size;

        Window* window;

        vec3 look_at = vec3(0, 0, 1);
        double time;

        void texture_size();

    public:
        Buffer::Buffer objectbuffer = Buffer::Buffer(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);
        Buffer::Buffer materialbuffer = Buffer::Buffer(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);
        Buffer::Buffer vertexbuffer = Buffer::Buffer(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);
        Buffer::Buffer indexbuffer = Buffer::Buffer(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);

        Tracer(Window* window, size_t height, float ratio);
        ~Tracer();
        void update();

        static std::vector<buffervertex> obj_to_vert(Loader::Object object);
    };
}

#endif //ODDITYENGINE_TRACER_H
