#ifndef ODDITYENGINE_TRACER_H
#define ODDITYENGINE_TRACER_H

#include "Interface.h"
#include <Graphics/Camera.h>
#include <Graphics/OpenGL/Shader/Shader.h>
#include <Graphics/OpenGL/Shader/Program.h>
#include <Graphics/Loader.h>

namespace OddityEngine {
    namespace Graphics {
        namespace Render {
            enum Objecttype {
                SPHERE,
                CUBE,
                MESH,
                CURVE
            };

            struct alignas(16) bufferobject {
                glm::mat4 transform;
                float radius;
                glm::uint32 type;
                glm::uint32 material;
                glm::uint32 vertexstart;
                glm::uint32 vertexcount;
            };

            struct alignas(16) buffermaterial {
                glm::vec4 color;
                glm::vec4 emission;
                float roughness;
            };

            struct alignas(16) buffervertex {
                glm::vec4 pos;
                glm::vec4 color;
                glm::vec4 normal;
                glm::vec2 uv;
            };

            class Tracer : public Interface {
            private:
                Camera* camera;

                Shader vertex_shader = Shader(GL_VERTEX_SHADER, "ray.vert");
                Shader fragment_shader = Shader(GL_FRAGMENT_SHADER, "ray.frag");
                Program program = Program(vertex_shader, fragment_shader);

                int bounces = 1;
                int spread = 1;
                float cull = 2;
                float split = 0;

                float ratio = 0.4;
                float last_ratio = ratio;

                Buffer screenbuffer = Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

                glm::vec3 look_at = {0, 0, 1};

                void init();

            public:
                Buffer* objectbuffer = nullptr;
                Buffer* materialbuffer = nullptr;
                Buffer* vertexbuffer = nullptr;
                Buffer* indexbuffer = nullptr;

                Tracer(Camera* camera = new Camera());
                Tracer(Buffer* texture_transform_buffer, Camera* camera = new Camera());
                ~Tracer();

                void render() override;

                void set_screen_size(const glm::vec2 &size) override;

                static std::vector<buffervertex> obj_to_vert(Loader::Object object);
            };
        } // OddityEngine
    } // Graphics
} // Render

#endif //ODDITYENGINE_TRACER_H
