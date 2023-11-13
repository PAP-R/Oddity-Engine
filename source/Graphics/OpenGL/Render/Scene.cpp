#include "Scene.h"

namespace OddityEngine {
    namespace Graphics {
        namespace Render {
            Scene::Scene() {
                glGenTextures(1, &render_texture);
                texture_size();

                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
            }

            void Scene::texture_size() const {
                glBindTexture(GL_TEXTURE_2D_ARRAY, render_texture);
                if (!renderers.empty()) {
                    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, size.x, size.y, renderers.size(), 0, GL_RGBA, GL_FLOAT, nullptr);
                }
            }

            bool Scene::update() {
                if (renderers.empty()) {
                    return false;
                }
                for (auto r : renderers) {
                    r->render();
                }
                return true;
            }

            void Scene::set_size(const glm::vec<2, int>& size) {
                this->size = size;
                texture_size();
                for (auto r : renderers) {
                    r->set_screen_size(size);
                }
            }

            size_t Scene::add_renderer(Interface *renderer) {
                size_t index = renderers.size();
                renderers.emplace_back(renderer);
                texture_size();
                renderer->set_texture_transform_buffer(&texture_transform_buffer);
                renderer->set_screen_size(size);
                renderer->set_texture(render_texture, index);
                return index;
            }

            size_t Scene::get_layers() {
                return renderers.size();
            }

            Buffer* Scene::get_texture_transform_buffer() {
                return &texture_transform_buffer;
            }

            GLuint Scene::get_render_texture() {
                return render_texture;
            }
        } // OddityEngine
    } // Graphics
} // Render