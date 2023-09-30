#include "Window.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"

using namespace glm;

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "GL/glew.h"

#include "GLFW/glfw3.h"
#include "util/Debug.h"

#include <stdexcept>
#include <algorithm>
#include "buffer/Buffer.h"
#include "buffer/Bufferobject.h"

namespace OddityEngine::Graphics {
    void Window::texture_size() const {
        glBindTexture(GL_TEXTURE_2D, this->render_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->render_size.x, this->render_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }

    GLFWwindow* make_window(const char *name, int width, int height, ImGuiContext** context) {
        glfwWindowHint(GLFW_SAMPLES, 16);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);
        if (window == NULL) {
            glfwTerminate();
            printf("Failed to create Window");
            throw std::runtime_error("Failed to create Window");
        }

        glfwMakeContextCurrent(window);

        *context = ImGui::CreateContext();
        ImGui::SetCurrentContext(*context);
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        if (glewInit() != GLEW_OK) {
            printf("Failed to initialize GLEW");
            throw std::runtime_error("Failed to initialize GLEW");
        }

        glewExperimental = true;

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        glEnable(GL_DEPTH_TEST);
//        glDepthFunc(GL_LESS);
//        glEnable(GL_CULL_FACE);

        glClearColor(0.1, 0.1, 0.1, 0.1);

        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        return window;
    }

    Window::Window(std::vector<Window*>* list, const char *name, int width, int height) : list(list), render_size(width, height), window(make_window(name, width, height, &this->context)) {
        glfwMakeContextCurrent(this->window);

        this->view_vertex_shader.compile();
        this->view_fragment_shader.compile();
        this->view_program = Shader::create_program(this->view_vertex_shader, this->view_fragment_shader);

        screenbuffer = Buffer::Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

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

        glGenFramebuffers(1, &this->frame_buffer);
        glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);

        glGenTextures(1, &this->render_texture);
        texture_size();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->render_texture, 0);

        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Framebuffer not complete");
        }

        Debug::add_value([&](){ImGui::SliderFloat("Ratio", &this->ratio, 0, 1);});
    }

    Window::~Window() {
        glDeleteFramebuffers(1, &this->frame_buffer);
        glDeleteRenderbuffers(1, &this->rbo_color);

        this->list->erase(std::remove(this->list->begin(), this->list->end(), this));

        glfwMakeContextCurrent(this->window);
        ImGui::SetCurrentContext(this->context);

        glfwDestroyWindow(this->window);

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext(this->context);
    }

    void Window::begin_update() {
        glfwMakeContextCurrent(this->window);
        ImGui::SetCurrentContext(this->context);

        glfwPollEvents();

        if (glfwWindowShouldClose(this->window)) {
            delete(this);
            return;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        int width, height;
        glfwGetWindowSize(this->window, &width, &height);
        ImGui::SetNextWindowSize(ImVec2(width, height));
        Debug::update();

        vec2 window_size = get_size();

        if(window_size.x == 0 || window_size.y == 0) {
            return;
        }

        if(window_size.x != this->view_size.x || window_size.y != this->view_size.y || this->ratio != this->last_ratio) {
            this->render_size = window_size * this->ratio + vec2(1);
            texture_size();
            this->view_size = window_size;
            this->last_ratio = this->ratio;
        }
    }

    void Window::end_update() {
        vec2 window_size = get_size();

        if(window_size.x == 0 || window_size.y == 0) {
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, window_size.x, window_size.y);

        glUseProgram(this->view_program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->render_texture);
        glUniform1i(glGetUniformLocation(this->view_program, "render_texture"), 0);

        mat4 screen_perspective = perspective(radians(90.0f), 1.0f, 0.1f, 100.0f);
        mat4 screen_projection = screen_perspective * lookAt(vec3(0), vec3(0, 0, 1), vec3(0, 1, 0));
        glUniformMatrix4fv(glGetUniformLocation(this->view_program, "screen_projection"), 1, GL_FALSE, &screen_projection[0][0]);

        glUniform2f(glGetUniformLocation(this->view_program, "view_size"), this->render_size.x, this->render_size.y);

        glEnableVertexAttribArray(0);

        glBindBuffer(this->screenbuffer.get_type(), this->screenbuffer);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(this->window);
    }

    vec<2, size_t> Window::get_pos() {
        int pos_x, pos_y;
        glfwGetWindowPos(this->window, &pos_x, &pos_y);
        return {pos_x, pos_y};
    }

    vec<2, size_t> Window::get_size() {
        int width, height;
        glfwGetWindowSize(this->window, &width, &height);
        return {width, height};
    }

    GLFWwindow *Window::get_window() {
        return this->window;
    }

    ImGuiContext *Window::get_context() {
        return this->context;
    }

    GLuint Window::get_frame_buffer() const {
        return this->frame_buffer;
    }

    vec<2, int> Window::get_render_size() const {
        return this->render_size;
    }
}