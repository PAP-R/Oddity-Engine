#include "Scene.h"

namespace OddityEngine::Graphics {
    void Scene::texture_size() const {
        if (!renderers.empty()) {
            glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
            glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, size.x, size.y, layer_count(), 0, GL_RGBA, GL_FLOAT, nullptr);
        }
    }

    Scene::Scene() {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    }

    bool Scene::update() {
        if (renderers.empty()) {
            return false;
        }

        for (const auto r : renderers) {
            r->render();
        }

        return true;
    }

    void Scene::event(const SDL_Event& event) {
        for (const auto e : eventables) {
            e->event(event);
        }
    }

    void Scene::set_size(const glm::vec<2, int>& size) {
        this->size = size;
        texture_size();
        for (auto r : renderers) {
            r->set_screen_size(size);
        }
    }

    GLsizei Scene::add_renderer(Render::Interface* renderer) {
        auto index = layer_count();

        renderers.emplace_back(renderer);

        texture_size();

        renderer->set_texture_transform_buffer(&texture_transform_buffer);
        renderer->set_screen_size(size);

        renderer->set_texture(texture, index);

        return index;
    }

    GLsizei Scene::add_input(Util::Eventable* eventable) {
        eventables.emplace_back(eventable);
        return eventables.size() - 1;
    }

    GLsizei Scene::layer_count() const {
        return renderers.size();
    }

    Buffer<Render::render_transform>* Scene::get_texture_transform_buffer() {
        return &texture_transform_buffer;
    }

    GLuint Scene::get_render_texture() const {
        return texture;
    }
}
