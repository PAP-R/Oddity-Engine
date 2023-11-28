#ifndef TESTSPIEL_RAYSTERIZER_H
#define TESTSPIEL_RAYSTERIZER_H

#include <vector>
#include <map>

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

                std::map<Program*, std::vector<Graphics::Object*>> program_object_map;

                GLuint depthbuffer = 0;

                float ratio = 1;
                float last_ratio = ratio;

                void create_depthbuffer();
                void resize_depthbuffer();

            public:
                explicit Raysterizer(Camera* camera = new Camera());
                explicit Raysterizer(Buffer<glm::vec4>* texture_transform_buffer, Camera* camera = new Camera());
                ~Raysterizer();

                void render() override;

                void add_object(Program* program, Graphics::Object* object);

                void set_size(const glm::vec2 &size) override;
                void set_screen_size(const glm::vec2 &size) override;
            };

        } // OddityEngine
    } // Graphics
} // Render

#endif //TESTSPIEL_RAYSTERIZER_H
