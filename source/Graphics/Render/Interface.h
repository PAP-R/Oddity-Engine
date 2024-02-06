#ifndef INTERFACE_H
#define INTERFACE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Graphics/Buffer/Buffer.h"

namespace OddityEngine::Graphics::Render {
    class Interface {
    protected:
        GLuint framebuffer = 0;

        GLuint texture = 0;
        glm::vec2 size = {0, 0};
        glm::vec2 screen_pos = {0, 0};
        glm::vec2 screen_size = {0, 0};

        Buffer<glm::vec4>* texture_transform_buffer = nullptr;
        GLsizei** texture_transform_index = nullptr;

        glm::vec4 texture_tranform();
        void set_texture_transform();

    public:
        Interface();
        explicit Interface(Buffer<glm::vec4>* texture_transform_buffer);
        ~Interface();

        virtual void render() = 0;

        void bind_framebuffer();
        virtual void set_texture(GLuint texture, GLsizei layer);
        virtual void set_texture_transform_buffer(Buffer<glm::vec4>* texture_transform_buffer);

        virtual void set_size(const glm::vec2 &size);
        virtual void set_screen_size(const glm::vec2 &size);
        virtual void set_screen_pos(const glm::vec2 &pos);
    };
}

#endif //INTERFACE_H
