#ifndef LAYERED_H
#define LAYERED_H

#include "Interface.h"
#include "Graphics/Camera.h"
#include <Graphics/Shader/Shader.h>
#include <Graphics/Shader/Program.h>

#include "Physics/World.h"

namespace OddityEngine::Graphics::Render {
    class Layered : public Interface {
    protected:
        Program screen_program = Program({Shader(GL_VERTEX_SHADER, "ray_screen.vert"), Shader(GL_FRAGMENT_SHADER, "ray_screen.frag")});
        Buffer<float> screen_buffer = Buffer<float>(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    public:
        explicit Layered(Camera* camera = new Camera());

        void render() override;

        void set_screen_size(const glm::vec2& size) override;

        Physics::World* world;

        Camera* camera = nullptr;
    };
}


#endif //LAYERED_H
