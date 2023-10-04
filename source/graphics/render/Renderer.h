#ifndef ODDITYENGINE_RENDERER_H
#define ODDITYENGINE_RENDERER_H

#include "../Window.h"

namespace OddityEngine::Graphics::Render {
    class Renderer {
    private:
        GLuint frame_buffer = 0;
        GLuint render_texture = 0;

        vec<2, GLsizei> size;

        void texture_size() const;

    public:
        Renderer(GLsizei width, GLsizei height);

        virtual GLuint render() = 0;

        virtual void resize() = 0;
    };
}


#endif //ODDITYENGINE_RENDERER_H
