#include "Tracer.h"

#include "util/Debug.h"
#include "buffer/Bufferobject.h"
#include "util/Time.h"

namespace OddityEngine::Graphics {
    Tracer::Tracer(Window* window, size_t width, size_t height) : window(window), render_size(width, height), vertex_shader(GL_VERTEX_SHADER, "shaders/ray.vert"), fragment_shader(GL_FRAGMENT_SHADER, "shaders/ray.frag"), view_vertex_shader(GL_VERTEX_SHADER, "shaders/view.vert"), view_fragment_shader(GL_FRAGMENT_SHADER, "shaders/view.frag") {
        glfwMakeContextCurrent(this->window->get_window());

//        Shader
        this->vertex_shader.compile();
        this->fragment_shader.compile();
        this->program = Shader::create_program(this->vertex_shader, this->fragment_shader);

        this->view_vertex_shader.compile();
        this->view_fragment_shader.compile();
        this->view_program = Shader::create_program(this->view_vertex_shader, this->view_fragment_shader);

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
        Debug::add_value([&](){ImGui::SliderFloat3("Position", &this->camera->position[0], -10, 10);});
        Debug::add_value([&](){ImGui::SliderFloat("Cull", &this->cull, 0, 10);});
        Debug::add_value([&](){ImGui::SliderInt("Bounces", &this->bounces, 0, 5);});
        Debug::add_value([&](){ImGui::SliderInt("Spread", &this->spread, 0, 5);});


        glGenFramebuffers(1, &this->frame_buffer);
        glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);

        glGenTextures(1, &this->render_texture);
        glBindTexture(GL_TEXTURE_2D, this->render_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->render_size.x, this->render_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->render_texture, 0);

        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Framebuffer not complete");
        }
    }

    Tracer::~Tracer() {
        glDeleteFramebuffers(1, &this->frame_buffer);
        glDeleteRenderbuffers(1, &this->rbo_color);
        delete(this->camera);
    }

    void Tracer::update() {
        auto window_size = this->window->get_size();

        if(window_size.x == 0 || window_size.y == 0) {
            return;
        }

        glfwMakeContextCurrent(this->window->get_window());

        glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glViewport(0, 0, this->render_size.x, this->render_size.y);

        this->time = Time::get_runtime<std::chrono::milliseconds, float>();

        glUniform1f(glGetUniformLocation(program, "TIME"), this->time);

//        this->look_at = vec3(sin(time / 1000), 0, cos(time / 1000));

        mat4 screen_perspective = perspective(radians(90.0f), 1.0f, 0.1f, 100.0f);
        mat4 screen_projection = screen_perspective * lookAt(vec3(0), vec3(0, 0, 1), vec3(0, 1, 0));

        glUniformMatrix4fv(glGetUniformLocation(program, "screen_projection"), 1, GL_FALSE, &screen_projection[0][0]);

        mat4 render_perspective = perspective(this->camera->fov, static_cast<float>(this->render_size.y) / static_cast<float>(this->render_size.x), 0.1f, 100.0f);
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

        glEnableVertexAttribArray(0);

        glBindBuffer(this->screenbuffer.get_type(), this->screenbuffer);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, window_size.x, window_size.y);

        glUseProgram(this->view_program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->render_texture);
        glUniform1i(glGetUniformLocation(this->view_program, "render_texture"), 0);

        screen_perspective = perspective(radians(90.0f), static_cast<float>(window_size.x) / static_cast<float>(window_size.y), 0.1f, 100.0f);
        screen_projection = screen_perspective * lookAt(vec3(0), vec3(0, 0, 1), vec3(0, 1, 0));
        glUniformMatrix4fv(glGetUniformLocation(this->view_program, "screen_projection"), 1, GL_FALSE, &screen_projection[0][0]);

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