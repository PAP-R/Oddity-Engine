#ifndef ODDITYENGINE_SCENE_H
#define ODDITYENGINE_SCENE_H

#include <vector>
#include "Interface.h"

namespace OddityEngine {
    namespace Graphics {
        namespace Render {
            class Scene {
            protected:
                GLuint render_texture;

                std::vector<Interface*> renderers;

                Buffer<glm::vec4> texture_transform_buffer = Buffer<glm::vec4>(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);

                glm::vec<2, int> size;

                void texture_size() const;

            public:
                Scene();

                bool update();

                void set_size(const glm::vec<2, int>& size);

                size_t add_renderer(Interface* renderer);

                size_t get_layers();
                Buffer<glm::vec4>* get_texture_transform_buffer();
                GLuint get_render_texture();
            };
        } // OddityEngine
    } // Graphics
} // Render

#endif //ODDITYENGINE_SCENE_H
