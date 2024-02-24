#ifndef COMPUTERENDERER_H
#define COMPUTERENDERER_H

#include "Interface.h"
#include "Graphics/Camera.h"
#include "Physics/World.h"

namespace OddityEngine::Graphics::Render {
    struct alignas(16) render_struct {
        glm::vec4 camera_pos;
        glm::mat4 render_vp;
        glm::ivec2 view_size;
        GLuint layer;
    };

    class ComputeRenderer : public render_struct, public Interface {

        Program render_program = Program({Shader(GL_COMPUTE_SHADER, "render.comp")});
        Buffer<render_struct> render_buffer = Buffer<render_struct>();

    public:
        ComputeRenderer(Camera* camera);

        void render() override;

        void set_screen_size(const glm::vec2& size) override;

        Physics::World* world;

        Camera* camera;
    };
}

#endif //COMPUTERENDERER_H
