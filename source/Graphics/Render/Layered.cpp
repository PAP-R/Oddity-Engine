#include "Layered.h"

#include <glm/gtx/quaternion.hpp>

namespace OddityEngine::Graphics::Render {
    Layered::Layered(Camera* camera) : camera(camera) {
        Vector<float> screen = {
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
        };

        screen_buffer.insert_back(screen.size(), screen.data());
    }

    void Layered::render() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, size.x, size.y);

        glUseProgram(screen_program);

        world->bind_buffers();

        glm::mat4 screen_perspective = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
        glm::mat4 screen_projection = screen_perspective * glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(screen_program.uniform_location("screen_projection"), 1, GL_FALSE, &screen_projection[0][0]);

        float fov = camera->fov / 90.0f;
        float aspect = size.y / size.x;
        // float aspect = 1;

        auto render_projection = glm::mat4(1);
        render_projection[0][0] = fov / aspect;
        render_projection[1][1] = fov;
        auto render_view = glm::toMat4(camera->orientation);

        auto render_vp = render_view * render_projection * glm::mat4(1);//TODO maybe change order to leave out mat4(1)

        glUniformMatrix4fv(screen_program.uniform_location("render_vp"), 1, GL_FALSE, &render_vp[0][0]);

        glUniform3f(screen_program.uniform_location("camera_pos"), camera->position.x, camera->position.y, camera->position.z);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, screen_buffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glDisableVertexAttribArray(0);
    }

    void Layered::set_screen_size(const glm::vec2& size) {
        set_size(size);
        Interface::set_screen_size(size);
    }
}
