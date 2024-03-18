#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Interface.h"
#include <Graphics/Buffer/Buffer.h>

#include <Util/Vector.h>

#include "../../Input/Eventable.h"

namespace OddityEngine::Graphics {
    class Scene {
    protected:
        GLuint texture = 0;

        Vector<Render::Interface*> renderers;
        Vector<Util::Eventable*> eventables;

        Buffer<Render::render_transform> texture_transform_buffer = Buffer<Render::render_transform>(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);

        glm::vec<2, int> size = {0, 0};

        void texture_size() const;

    public:
        Scene();

        bool update();

        void event(const SDL_Event& event);

        void set_size(const glm::vec<2, int>& size);
        GLsizei add_renderer(Render::Interface* renderer);
        GLsizei add_input(Util::Eventable* input);

        GLsizei layer_count() const;
        Buffer<Render::render_transform>* get_texture_transform_buffer();
        GLuint get_render_texture() const;
    };
}

#endif //SCENE_H
