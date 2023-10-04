#include "Renderer.h"

#include <stdexcept>

#include "../../util/Debug.h"

namespace OddityEngine::Graphics::Render {
    void Renderer::texture_size() const {
        glBindTexture(GL_TEXTURE_2D, this->render_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->size.x, this->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }

    Renderer::Renderer(GLsizei width, GLsizei height) : size(width, height) {
        glGenFramebuffers(1, &this->frame_buffer);
        glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);

        glGenTextures(1, &this->render_texture);
        texture_size();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->render_texture, 0);

        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            Debug::error("Framebuffer not complete");
        }
    }
}
