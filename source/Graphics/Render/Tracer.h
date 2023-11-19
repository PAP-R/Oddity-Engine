#ifndef ODDITYENGINE_TRACER_H
#define ODDITYENGINE_TRACER_H

#include "Interface.h"
#include "Graphics/Camera.h"
#include "Graphics/Shader/Shader.h"
#include "Graphics/Shader/Program.h"
#include "Graphics/Loader.h"

namespace OddityEngine {
    namespace Graphics {
        namespace Render {

            class Tracer : public Interface {
            protected:
                Camera* camera;

                Shader vertex_shader = Shader(GL_VERTEX_SHADER, "screen_ray.vert");
                Shader fragment_shader = Shader(GL_FRAGMENT_SHADER, "screen_ray.frag");
                Program program = Program({vertex_shader, fragment_shader});

                int bounces = 1;
                int spread = 1;
                float cull = 2;
                float split = 0;

                float ratio = 0.4;
                float last_ratio = ratio;

                Buffer<float> screenbuffer = Buffer<float>(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

                glm::vec3 look_at = {0, 0, 1};

                void init();

            public:
                explicit Tracer(Camera* camera = new Camera());
                explicit Tracer(Buffer<glm::vec4>* texture_transform_buffer, Camera* camera = new Camera());

                void render() override;

                void set_screen_size(const glm::vec2 &size) override;
            };
        } // OddityEngine
    } // Graphics
} // Render

#endif //ODDITYENGINE_TRACER_H
