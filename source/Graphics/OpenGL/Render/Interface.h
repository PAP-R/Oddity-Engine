#ifndef ODDITYENGINE_INTERFACE_H
#define ODDITYENGINE_INTERFACE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <Graphics/OpenGL/Buffer/Buffer.h>
#include <Graphics/OpenGL/Buffer/Bufferobject.h>

namespace OddityEngine {
    namespace Graphics {
        namespace Render {
            class Interface {
            protected:
                GLuint framebuffer;

                glm::vec2 size;
                glm::vec2 screen_pos;
                glm::vec2 screen_size;

                Bufferobject<glm::vec4> texture_transform;

                void set_texture_transform();

            public:
                explicit Interface(Buffer* texture_transform_buffer);
                explicit Interface();
                ~Interface();

                virtual void render() = 0;
                void set_texture(GLuint texture, GLuint layer);
                void set_texture_transform_buffer(Buffer* texture_transform_buffer);

                virtual void set_size(const glm::vec2 &size);
                virtual void set_screen_size(const glm::vec2 &size);
                virtual void set_screen_pos(const glm::vec2 &pos);
            };
        } // OddityEngine
    } // Graphics
} // Render

#endif //ODDITYENGINE_INTERFACE_H
