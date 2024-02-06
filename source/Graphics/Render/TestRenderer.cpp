#include "TestRenderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Math/random.h>

#include <Graphics/Shader/Shader.h>

OddityEngine::Graphics::Render::TestRenderer::TestRenderer(Camera* camera) : camera(camera), screenbuffer(GL_ARRAY_BUFFER), program({Shader(GL_VERTEX_SHADER, "test.vert"), Shader(GL_FRAGMENT_SHADER, "test.frag")}) {
    Vector screen = {
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
    };

    auto indices = screenbuffer.insert_back(screen.size(), screen.data());
}

void OddityEngine::Graphics::Render::TestRenderer::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, size.x, size.y);

    glUseProgram(program);

    glm::mat4 screen_perspective = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    glm::mat4 screen_projection = screen_perspective * glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)) * glm::mat4(1);

    glUniformMatrix4fv(program.uniform_location("screen_projection"), 1, GL_FALSE, &screen_projection[0][0]);

    glEnableVertexAttribArray(0);

    glBindBuffer(screenbuffer.get_type(), screenbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
}

void OddityEngine::Graphics::Render::TestRenderer::set_camera(Camera* camera) {
    this->camera = camera;
}

void OddityEngine::Graphics::Render::TestRenderer::set_screen_size(const glm::vec2& size) {
    Interface::set_screen_size(size);
    set_size(size);
}
