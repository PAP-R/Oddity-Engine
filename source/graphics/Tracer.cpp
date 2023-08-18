#include "Tracer.h"

#include "util/Debug.h"
#include "buffer/Bufferobject.h"
#include "util/Time.h"

namespace OddityEngine::Graphics {
    Tracer::Tracer(Window* window, size_t width, size_t height) : window(window), render_size(width, height), vertex_shader(GL_VERTEX_SHADER, "shaders/simple.vert"), fragment_shader(GL_FRAGMENT_SHADER, "shaders/simple.frag"), screenbuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW) {
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

        Debug::add_value("Look at: [ {: 05.05f} / {: 05.05f} / {: 05.05f} / {: 05.05f} ]", &this->camera->orientation.x, &this->camera->orientation.y, &this->camera->orientation.z, &this->camera->orientation.w);
        Debug::add_value("Time: [ {: 05.05f} ]", &this->time);

    }

    Tracer::~Tracer() {
        glDeleteFramebuffers(1, &this->fbo);
        glDeleteRenderbuffers(1, &this->rbo_color);
        delete(this->camera);
    }

    void Tracer::update() {
        glfwMakeContextCurrent(this->window->get_window());

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        this->time = Time::get_runtime<std::chrono::milliseconds, float>();

        glUniform1f(glGetUniformLocation(program, "TIME"), time);

//        this->look_at = vec3(sin(time / 1000), 0, cos(time / 1000));
        this->camera->orientation.x = sin(time);
        this->camera->orientation.y = sin(time);

        mat4 screen_projection = perspective(radians(90.0f), 1.0f, 0.1f, 100.0f);

//        mat4 vp = screen_projection * lookAt(vec3(0), this->look_at, vec3(0, 1, 0));
        mat4 vp = screen_projection * lookAt(this->camera->position, this->camera->position + this->camera->direction(), this->camera->up());

        glUniformMatrix4fv(glGetUniformLocation(program, "screen_projection"), 1, GL_FALSE, &vp[0][0]);

        glEnableVertexAttribArray(0);

        glBindBuffer(this->screenbuffer.get_type(), this->screenbuffer);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }


}