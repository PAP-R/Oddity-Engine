#ifndef TESTSPIEL_RAYSTERIZER_H
#define TESTSPIEL_RAYSTERIZER_H

#include <vector>

#include "Interface.h"
#include <Graphics/Camera.h>
#include <Graphics/Object/Object.h>
#include <Graphics/Shader/Program.h>

namespace OddityEngine {
    namespace Graphics {
        namespace Render {

            class Raysterizer : public Interface {
            protected:
                Camera* camera;

                std::vector<std::pair<Program*, Graphics::Object*>> program_object_list;

                float ratio = 1;
                float last_ratio = ratio;

                Buffer<float> screenbuffer = Buffer<float>(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

            public:
                explicit Raysterizer(Camera* camera = new Camera());
                explicit Raysterizer(Buffer<glm::vec4>* texture_transform_buffer, Camera* camera = new Camera());

                void render() override;

                size_t add_object(Program* program, Graphics::Object* object);

                void set_screen_size(const glm::vec2 &size) override;
            };

        } // OddityEngine
    } // Graphics
} // Render

#endif //TESTSPIEL_RAYSTERIZER_H
