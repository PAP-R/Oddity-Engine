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

// OddityEngine
#include "Window.h"

namespace OddityEngine::Graphics {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }


    std::vector<Window*> windows;

    void init() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
    }

    Window* create_window(const char *name, int width, int height) {
        auto window = new Window(&windows, name, width, height);

        glfwSetFramebufferSizeCallback(window->get_window(), framebuffer_size_callback);

        windows.emplace_back(window);
        return window;
    }

    void terminate() {
        while (!windows.empty()) {
            delete(windows.front());
        }

        glfwTerminate();
    }

    bool update() {
        for (auto win : windows) {
            win->update();
            if (windows.empty()) {
                return false;
            }
        }
        return true;
    }
}