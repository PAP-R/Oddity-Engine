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

#include <stdexcept>
#include <algorithm>

namespace OddityEngine::Graphics {
    Window::Window(std::vector<Window*>* list, const char *name, int width, int height) : list(list) {
        glfwWindowHint(GLFW_SAMPLES, 16);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        this->window = glfwCreateWindow(width, height, name, nullptr, nullptr);
        if (window == NULL) {
            glfwTerminate();
            printf("Failed to create Window");
            throw std::runtime_error("Failed to create Window");
        }

        glfwMakeContextCurrent(this->window);

        this->context = ImGui::CreateContext();
        ImGui::SetCurrentContext(this->context);
        ImGui_ImplGlfw_InitForOpenGL(this->window, true);
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
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);

        glClearColor(0.1, 0.1, 0.1, 0.1);

        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
    }

    Window::~Window() {
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

        glClear(GL_COLOR_BUFFER_BIT);

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
}