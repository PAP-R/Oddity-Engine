#include "ComputeRenderer.h"

#include <glm/gtx/quaternion.hpp>

namespace OddityEngine::Graphics::Render {
    ComputeRenderer::ComputeRenderer(Camera* camera) : camera(camera) {}

    void ComputeRenderer::render() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(render_program);

        world->bind_buffers();

        float fov = camera->fov / 90.0f;
        float aspect = size.y / size.x;
        // float aspect = 1;

        view_size = size;
        auto render_projection = glm::mat4(1);
        render_projection[0][0] = fov / aspect;
        render_projection[1][1] = fov;
        // render_projection[2][3] = -1;
        // render_projection = glm::perspective(camera->fov, aspect, 0.1f, 100.0f);
        auto render_view = glm::toMat4(camera->orientation);

        render_vp = render_view * render_projection * glm::mat4(1);//TODO maybe change order to leave out mat4(1)

        camera_pos = camera->position;

        layer = texture_layer;

        render_buffer.set(0, this);

        render_buffer.bind_base(RENDER);

        glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
        glBindImageTexture(0, texture, 0, GL_FALSE, layer, GL_READ_WRITE, GL_RGBA32F);

        // glDispatchCompute(view_size.x, view_size.y, 1);
        glDispatchCompute(world->object_count(), world->object_count(), 1);
        // glDispatchCompute(world->object_count(), 1, 1);

        // glMemoryBarrier(GL_ALL_BARRIER_BITS);
        glFinish();
    }

    void ComputeRenderer::set_screen_size(const glm::vec2& size) {
        set_size(size);
        Interface::set_screen_size(size);
    }
}
