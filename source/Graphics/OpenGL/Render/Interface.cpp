#include "Interface.h"

#include <Util/Debug.h>

namespace OddityEngine {
    namespace Graphics {
        namespace Render {
            Interface::Interface() : texture_transform(glm::vec4(0)) {
                glGenFramebuffers(1, &framebuffer);
            }

            Interface::Interface(Buffer *texture_transform_buffer) : texture_transform(texture_transform_buffer, glm::vec4(0)) {
            }

            Interface::~Interface() {
                glDeleteFramebuffers(1, &framebuffer);
            }

            void Interface::set_texture(GLuint texture, GLuint layer) {
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
                glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0, layer);

                glDrawBuffer(GL_COLOR_ATTACHMENT0);

                if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                    Debug::error("Framebuffer not complete");
                }
            }

            void Interface::set_texture_transform_buffer(Buffer *texture_transform_buffer) {
                texture_transform.set_buffer(texture_transform_buffer);
            }

            void Interface::set_size(const glm::vec2 &size) {
                this->size = size;
                set_texture_transform();
            }

            void Interface::set_screen_size(const glm::vec2 &size) {
                this->screen_size = size;
                set_texture_transform();
            }

            void Interface::set_screen_pos(const glm::vec2 &pos) {
                this->screen_pos = pos;
                set_texture_transform();
            }

            void Interface::set_texture_transform() {
                glm::vec2 scale = size / screen_size;
                texture_transform.set(glm::vec4(screen_pos / screen_size, scale));
            }
        } // OddityEngine
    } // Graphics
} // Render