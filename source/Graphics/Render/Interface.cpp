#include "Interface.h"

#include "Graphics/Loader.h"
#include "Util/Debug.h"
#include "Util/Time.h"

namespace OddityEngine {
    namespace Graphics {
        namespace Render {
            Interface::Interface(size_t layers) : texture_transform(*create_empty_buffer_object_list<glm::vec4>(nullptr, layers)), layer_count(layers) {
                glGenFramebuffers(1, &framebuffer);
            }

            Interface::Interface(Buffer<glm::vec4> *texture_transform_buffer, size_t layers) : texture_transform(*create_empty_buffer_object_list(texture_transform_buffer, layers)), layers(layers) {
            }

            Interface::~Interface() {
                glDeleteFramebuffers(1, &framebuffer);
            }

            void Interface::set_texture(GLuint texture, const std::vector<GLuint>&layers) {
                this->layers = layers;

                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
                glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0, layers[0]);

                glDrawBuffer(GL_COLOR_ATTACHMENT0);

                if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                    Debug::error("Framebuffer not complete");
                }
            }

            void Interface::set_texture_transform_buffer(Buffer<glm::vec4> *texture_transform_buffer) {
                for (auto& t : texture_transform) {
                    t.set_buffer(texture_transform_buffer);
                }
            }

            float Interface::get_ratio() {
                return ratio;
            }

            void Interface::set_ratio(const float ratio) {
                this->ratio = ratio;
            }

            void Interface::shift_ratio(float ratio) {
                this->ratio += (ratio - this->ratio) * 0.1;
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

            GLsizei Interface::get_layers() {
                return layer_count;
            }

            void Interface::set_texture_transform() {
                for (int i = 0; i < layer_count; i++) {
                    auto scale = size * static_cast<float>(pow(layer_ratio, i)) / screen_size;
                    texture_transform[i].set(glm::vec4(screen_pos / screen_size, scale));
                }
            }

            std::vector<buffervertex> obj_to_vert(Loader::Object object) {
                std::vector<buffervertex> vertices;
                for (auto f : object.faces) {
                    vertices.emplace_back(glm::vec4(object.vertices[f[0] - 1], 1), glm::vec4(object.colors[f[0] - 1], 1), glm::vec4(object.normals[f[2] - 1], 1), object.uvs[f[1] - 1]);
                }

                return vertices;
            }
        } // OddityEngine
    } // Graphics
} // Render