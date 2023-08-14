#include "Graphics.h"

// Imgui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// OpenGL
#include "GL/glew.h"
#include "GLFW/glfw3.h"

// STD
#include <stdexcept>
#include <vector>

#include "fmt/core.h"

// OddityEngine
#include "Window.h"
#include "Tracer.h"

namespace OddityEngine::Graphics {
    size_t error_count = 0;
    void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        fmt::print("{:4d} : {}\n", error_count++, message);
    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    std::vector<Window*> windows;
    std::vector<Tracer*> tracers;

    void init() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
    }

    Window* create_window(const char *name, int width, int height) {
        auto window = new Window(&windows, name, width, height);

        glfwSetFramebufferSizeCallback(window->get_window(), framebuffer_size_callback);

        windows.emplace_back(window);

        glDebugMessageCallback(MessageCallback, nullptr);

        return window;
    }

    Tracer* create_tracer(Window* window, size_t width, size_t height) {
        auto tracer = new Tracer(window, width, height);
        tracers.emplace_back(tracer);

        return tracer;
    }


    void terminate() {
        while (!windows.empty()) {
            delete(windows.front());
        }

        glfwTerminate();
    }

    bool update() {
        for (auto win : windows) {
            win->begin_update();
            if (windows.empty()) {
                return false;
            }
        }
        for (auto trace : tracers) {
            trace->update();
        }
        for (auto win : windows) {
            win->end_update();
            if (windows.empty()) {
                return false;
            }
        }
        return true;
    }
}