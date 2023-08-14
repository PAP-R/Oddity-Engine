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

//        Framebuffer
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);


        glGenRenderbuffers(1, &this->rbo_color);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_color);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, this->render_size.x, this->render_size.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->rbo_color);


//        glGenRenderbuffers(1, &this->rbo_depth_stencil);
//        glBindRenderbuffer(GL_RENDERBUFFER, this->rbo_depth_stencil);
//
//        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->render_size.x, this->render_size.y);
//        glBindRenderbuffer(GL_RENDERBUFFER, 0);
//
//        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo_depth_stencil);


        OddityEngine::Debug::add_value("rbo_color: {}\nrbo_depth: {}", &this->rbo_color, &this->rbo_depth_stencil);


        auto fbstatus = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        OddityEngine::Debug::add_value("Framebufferstatus: {}", &fbstatus);

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

        Debug::add_value("Look at: [ {: 05.05f} / {: 05.05f} / {: 05.05f} ]", &this->look_at.x, &this->look_at.y, &this->look_at.z);
    }

    Tracer::~Tracer() {
        glDeleteFramebuffers(1, &this->fbo);
        glDeleteRenderbuffers(1, &this->rbo_color);
    }

    void Tracer::update() {
        glfwMakeContextCurrent(this->window->get_window());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        mat4 screen_projection = perspective(radians(90.0f), float(this->render_size.x) / float(this->render_size.y), 0.1f, 100.0f);

//        this->look_at = vec3(sin(Time::get_time<std::chrono::milliseconds>() / 1000), 0, cos(Time::get_time<std::chrono::milliseconds>() / 1000));

        this->look_at = vec3(0, 0, 1);

        mat4 vp = screen_projection * lookAt(vec3(0), this->look_at, vec3(0, 1, 0));

        glUniformMatrix4fv(glGetUniformLocation(program, "screen_projection"), 1, GL_FALSE, &vp[0][0]);

        glEnableVertexAttribArray(0);

        glBindBuffer(this->screenbuffer.get_type(), this->screenbuffer);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        int width, height;
        glfwGetWindowSize(this->window->get_window(), &width, &height);

        glBlitFramebuffer(0, 0, this->render_size.x, this->render_size.y, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }


}