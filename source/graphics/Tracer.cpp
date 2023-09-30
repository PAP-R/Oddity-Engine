#include "Tracer.h"

#include "util/Debug.h"
#include "buffer/Bufferobject.h"
#include "util/Time.h"

namespace OddityEngine::Graphics {
    Tracer::Tracer(Window* window, size_t height, float ratio) : window(window) {
        glfwMakeContextCurrent(this->window->get_window());

//        Shader
        this->vertex_shader.compile();
        this->fragment_shader.compile();
        this->program = Shader::create_program(this->vertex_shader, this->fragment_shader);

//        Screen
        std::vector<float> screen = {
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
        };

        for (auto p : screen) {
            Buffer::Bufferobject(&this->screenbuffer, p);
        }

        this->camera = new Camera();

        Debug::add_value([&](){ImGui::Text("%s", fmt::format("Front: [ {: 05.05f} / {: 05.05f} / {: 05.05f} ]", this->camera->direction().x, this->camera->direction().y, this->camera->direction().z).c_str());});
        Debug::add_value([&](){ImGui::Text("%s", fmt::format("Up: [ {: 05.05f} / {: 05.05f} / {: 05.05f} ]", this->camera->up().x, this->camera->up().y, this->camera->up().z).c_str());});
        Debug::add_value([&](){ImGui::SliderFloat4("Quat", &this->camera->orientation[0], -1.1, 1.1);});
        Debug::add_value([&](){ImGui::SliderFloat3("Angle", &this->camera->angle[0], -361, 361);});
        Debug::add_value([&](){ImGui::SliderFloat3("Position", &this->camera->position[0], -20, 20);});
        Debug::add_value([&](){ImGui::SliderFloat("Cull", &this->cull, 0, 5);});
        Debug::add_value([&](){ImGui::SliderInt("Bounces", &this->bounces, 0, 5);});
        Debug::add_value([&](){ImGui::SliderInt("Spread", &this->spread, 0, 5);});
    }

    Tracer::~Tracer() {
        delete(this->camera);
    }

    void Tracer::update() {
        glfwMakeContextCurrent(this->window->get_window());

        glBindFramebuffer(GL_FRAMEBUFFER, this->window->get_frame_buffer());

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glViewport(0, 0, this->window->get_render_size().x, this->window->get_render_size().y);

        this->time = Time::get_runtime<std::chrono::milliseconds, float>();

        glUniform1f(glGetUniformLocation(program, "TIME"), this->time);

        mat4 screen_perspective = perspective(radians(90.0f), 1.0f, 0.1f, 100.0f);
        mat4 screen_projection = screen_perspective * lookAt(vec3(0), vec3(0, 0, 1), vec3(0, 1, 0));

        glUniformMatrix4fv(glGetUniformLocation(program, "screen_projection"), 1, GL_FALSE, &screen_projection[0][0]);

        mat4 render_perspective = perspective(this->camera->fov, static_cast<float>(this->window->get_render_size().y) / static_cast<float>(this->window->get_render_size().x), 0.1f, 100.0f);
        mat4 render_projection = render_perspective * lookAt(vec3(0), this->camera->direction(), this->camera->up());

        glUniformMatrix4fv(glGetUniformLocation(program, "render_projection"), 1, GL_FALSE, &render_projection[0][0]);

        glUniform1ui(glGetUniformLocation(program, "bounces"), this->bounces);
        glUniform1ui(glGetUniformLocation(program, "spread"), this->spread);
        glUniform1f(glGetUniformLocation(program, "cull"), this->cull);
        glUniform1f(glGetUniformLocation(program, "split"), this->split);

        glUniform3f(glGetUniformLocation(program, "CAMERAPOS"), camera->position.x, camera->position.y, camera->position.z);

        glBindBufferBase(this->objectbuffer.get_type(), 3, this->objectbuffer);
        glBindBufferBase(this->materialbuffer.get_type(), 4, this->materialbuffer);
        glBindBufferBase(this->vertexbuffer.get_type(), 5, this->vertexbuffer);
        glBindBufferBase(this->indexbuffer.get_type(), 6, this->indexbuffer);

        glEnableVertexAttribArray(0);

        glBindBuffer(this->screenbuffer.get_type(), this->screenbuffer);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);
    }

    std::vector<buffervertex> Tracer::obj_to_vert(Loader::Object object) {
        std::vector<buffervertex> vertices;
        for (auto f : object.faces) {
            vertices.emplace_back(vec4(object.vertices[f[0] - 1], 1), vec4(object.colors[f[0] - 1], 1), vec4(object.normals[f[2] - 1], 1), object.uvs[f[1] - 1]);
        }

        return vertices;
    }
}