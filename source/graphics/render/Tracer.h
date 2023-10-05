#ifndef ODDITYENGINE_TRACER_H
#define ODDITYENGINE_TRACER_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
using namespace glm;

#include "Renderer.h"
#include "graphics/Shader.h"
#include "graphics/buffer/Buffer.h"
#include "graphics/Camera.h"
#include "util/Loader.h"

namespace OddityEngine::Graphics::Render {
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

    class Tracer : public Renderer {
    private:
        Camera* camera;

        Shader::Shader vertex_shader = Shader::Shader(GL_VERTEX_SHADER, "shaders/ray.vert");
        Shader::Shader fragment_shader = Shader::Shader(GL_FRAGMENT_SHADER, "shaders/ray.frag");
        GLuint program;

        int bounces = 1;
        int spread = 1;
        float cull = 2;
        float split = 0;

        float ratio = 0.4;
        float last_ratio = ratio;

        Buffer::Buffer screenbuffer = Buffer::Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

        vec3 look_at = vec3(0, 0, 1);
        double time;


    public:
        Buffer::Buffer objectbuffer = Buffer::Buffer(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);
        Buffer::Buffer materialbuffer = Buffer::Buffer(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);
        Buffer::Buffer vertexbuffer = Buffer::Buffer(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);
        Buffer::Buffer indexbuffer = Buffer::Buffer(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);

        Tracer(Buffer::Buffer* texture_transform_buffer);
        ~Tracer();

        GLuint render() override;

        static std::vector<buffervertex> obj_to_vert(Loader::Object object);
    };
}

#endif //ODDITYENGINE_TRACER_H
