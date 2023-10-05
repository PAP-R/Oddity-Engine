#include "Renderer.h"

#include <stdexcept>

#include "../../util/Debug.h"

namespace OddityEngine::Graphics::Render {
    Renderer::Renderer(Buffer::Buffer* texture_transform_buffer) : texture_transform(texture_transform_buffer, vec4(0)) {
        glGenFramebuffers(1, &this->frame_buffer);
        glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);
    }

    Renderer::~Renderer() {
        glDeleteFramebuffers(1, &this->frame_buffer);
        glDeleteRenderbuffers(1, &this->rbo_color);
    }

    void Renderer::set_texture(GLuint texture, GLint index) {
        glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0, index);

        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            Debug::error("Framebuffer not complete");
        }
    }


    void Renderer::set_size(const vec2 &size) {
        Renderer::size = size;
        set_texture_transform();
    }

    void Renderer::set_screen_pos(const vec2 &screenPos) {
        screen_pos = screenPos;
        set_texture_transform();
    }

    void Renderer::set_screen_size(const vec2 &screenSize) {
        screen_size = screenSize;
        set_texture_transform();
    }

    void Renderer::set_texture_transform() {
        vec2 ratio = size / screen_size;
        texture_transform.set(vec4(screen_pos * ratio, ratio));
    }
}
