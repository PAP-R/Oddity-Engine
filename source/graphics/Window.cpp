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
        glBindTexture(GL_TEXTURE_2D_ARRAY, this->render_texture);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, this->view_size.x, this->view_size.y, this->layers, 0, GL_RGBA, GL_FLOAT, nullptr);
    }

    GLFWwindow* make_window(const char *name, int width, int height, ImGuiContext** context) {
//        glfwWindowHint(GLFW_SAMPLES, 16);
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

    Window::Window(std::vector<Window*>* list, const char *name, int width, int height) : list(list), view_size(width, height), window(make_window(name, width, height, &this->context)) {
        glfwMakeContextCurrent(this->window);

        this->view_vertex_shader.compile();
        this->view_fragment_shader.compile();
        this->view_program = Shader::create_program(this->view_vertex_shader, this->view_fragment_shader);

        screenbuffer = Buffer::Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
        texture_transform_buffer = Buffer::Buffer(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);

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

        glGenTextures(1, &this->render_texture);
        texture_size();

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

        Debug::add_value([&](){ImGui::SliderInt("Sample Size", &this->sample_size, 0, 100);});
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

    void Window::update() {
        glfwMakeContextCurrent(this->window);
        ImGui::SetCurrentContext(this->context);

        glfwPollEvents();

        if (glfwWindowShouldClose(this->window)) {
            delete(this);
            return;
        }

        this->size = get_size();

        if(this->size.x == 0 || this->size.y == 0) {
            return;
        }

        if(this->size.x != this->view_size.x || this->size.y != this->view_size.y) {
            this->view_size = this->size;
            texture_size();
            for (auto r : renderers) {
                r->set_screen_size(this->view_size);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(this->size.x, this->size.y));
        Debug::update();

        for (auto r : renderers) {
            r->render();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, this->size.x, this->size.y);

        glUseProgram(this->view_program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, this->render_texture);
        glBindSampler(0, 0);
        glUniform1ui(glGetUniformLocation(this->view_program, "texture_count"), this->renderers.size());

        glBindBufferBase(this->texture_transform_buffer.get_type(), 3, this->texture_transform_buffer);

        mat4 screen_perspective = perspective(radians(90.0f), 1.0f, 0.1f, 100.0f);
        mat4 screen_projection = screen_perspective * lookAt(vec3(0), vec3(0, 0, 1), vec3(0, 1, 0));
        glUniformMatrix4fv(glGetUniformLocation(this->view_program, "screen_projection"), 1, GL_FALSE, &screen_projection[0][0]);

        glUniform2f(glGetUniformLocation(this->view_program, "view_size"), this->view_size.x, this->view_size.y);
        glUniform1f(glGetUniformLocation(this->view_program, "sample_size"), this->sample_size);

        glEnableVertexAttribArray(0);

        glBindBuffer(this->screenbuffer.get_type(), this->screenbuffer);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(this->window);
    }

    vec<2, GLsizei> Window::get_pos() {
        GLsizei pos_x, pos_y;
        glfwGetWindowPos(this->window, &pos_x, &pos_y);
        return {pos_x, pos_y};
    }

    vec<2, GLsizei> Window::get_size() {
        GLsizei width, height;
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

    bool Window::is_open() const {
        return !(this->size.x == 0 || this->size.y == 0);
    }

    uint Window::add_renderer(Render::Renderer *renderer) {
        GLint index = this->renderers.size();
        this->layers = index + 1;
        texture_size();
        renderer->set_screen_size(this->view_size);
        renderer->set_texture(this->render_texture, index);
        this->renderers.emplace_back(renderer);
        return index;
    }

    Buffer::Buffer *Window::get_texture_transform_buffer() {
        return &this->texture_transform_buffer;
    }
}