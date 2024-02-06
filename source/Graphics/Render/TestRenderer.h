#ifndef TESTRENDERER_H
#define TESTRENDERER_H

#include "Interface.h"
#include <Graphics/Shader/Program.h>
#include <Graphics/Camera.h>

namespace OddityEngine::Graphics::Render {
    class TestRenderer : public Interface {
    protected:
        Camera* camera = nullptr;
        Program program;

        Buffer<GLfloat> screenbuffer;
    public:
        TestRenderer(Camera* camera);
        void render() override;

        void set_camera(Camera* camera);

        void set_screen_size(const glm::vec2 &size);
    };
}

#endif //TESTRENDERER_H
