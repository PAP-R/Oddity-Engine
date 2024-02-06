#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Interface.h"
#include <Graphics/Buffer/Buffer.h>

#include <Util/Vector.h>

namespace OddityEngine::Graphics {
    class Scene {
    protected:
        GLuint texture = 0;

        Vector<Render::Interface*> renderers;

        Buffer<glm::vec4> texture_transform_buffer = Buffer<glm::vec4>(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);

        glm::vec<2, int> size = {0, 0};

        void texture_size() const;

    public:
        Scene();

        bool update();

        void set_size(const glm::vec<2, int>& size);
        GLsizei add_renderer(Render::Interface* renderer);

        GLsizei layer_count() const;
        Buffer<glm::vec4>* get_texture_transform_buffer();
        GLuint get_render_texture() const;
    };
}

#endif //SCENE_H
