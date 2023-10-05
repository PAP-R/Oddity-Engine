#ifndef ODDITYENGINE_RENDERER_H
#define ODDITYENGINE_RENDERER_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "imgui_internal.h"
#include "graphics/buffer/Buffer.h"
#include "graphics/buffer/Bufferobject.h"

using namespace glm;

namespace OddityEngine::Graphics::Render {
    struct alignas(16) Texture_box_struct {
        vec4 render;
        vec4 actual;
    };

    class Renderer {
    protected:
        GLuint rbo_color = 0;
        GLuint rbo_depth_stencil = 0;

        GLuint frame_buffer = 0;

        GLint index;

        vec2 size;
        vec2 screen_pos;
        vec2 screen_size;

        Buffer::Bufferobject<vec4> texture_transform;

        void set_texture_transform();

    public:
        Renderer(Buffer::Buffer* texture_transform_buffer);
        ~Renderer();

        virtual void set_size(const vec2 &size);
        virtual void set_screen_pos(const vec2 &screenPos);
        virtual void set_screen_size(const vec2 &screenSize);

        virtual GLuint render() = 0;
        void set_texture(GLuint texture, GLint index);
    };
}


#endif //ODDITYENGINE_RENDERER_H
