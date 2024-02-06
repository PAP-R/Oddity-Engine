#include "Interface.h"

namespace OddityEngine::Graphics::Render {
    glm::vec4 Interface::texture_tranform() {
        auto scale = size / screen_size;
        return glm::vec4(screen_pos / screen_size, scale);
    }

    void Interface::set_texture_transform() {
        texture_transform_buffer->set(**texture_transform_index, texture_tranform());
    }

    Interface::Interface() {
        glGenFramebuffers(1, &framebuffer);
    }

    Interface::Interface(Buffer<glm::vec4>* texture_transform_buffer) : Interface() {
        set_texture_transform_buffer(texture_transform_buffer);
    }

    Interface::~Interface() {
        glDeleteFramebuffers(1, &framebuffer);
    }

    void Interface::set_texture(const GLuint texture, const GLsizei layer) {
        this->texture = texture;

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0, layer);

        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            Debug::error("Framebuffer not complete");
        }
    }

    void Interface::set_texture_transform_buffer(Buffer<glm::vec4>* texture_transform_buffer) {
        if (this->texture_transform_buffer != nullptr && *texture_transform_index != nullptr) {
            this->texture_transform_buffer->remove(1, **texture_transform_index);
        }

        this->texture_transform_buffer = texture_transform_buffer;

        if (texture_transform_buffer != nullptr) {
            texture_transform_index = texture_transform_buffer->insert_back(texture_tranform());
        }
    }

    void Interface::set_size(const glm::vec2& size) {
        this->size = size;
        set_texture_transform();
    }

    void Interface::set_screen_size(const glm::vec2& size) {
        this->screen_size = size;
        set_texture_transform();
    }

    void Interface::set_screen_pos(const glm::vec2& pos) {
        this->screen_pos = pos;
        set_texture_transform();
    }
}
