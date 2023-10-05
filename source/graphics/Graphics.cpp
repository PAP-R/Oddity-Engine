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
#include "graphics/render/Tracer.h"
#include "util/Debug.h"

namespace OddityEngine::Graphics {
    void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        Debug::message(message);
    }

    std::vector<Window*> windows;

    void init() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
    }

    Window* create_window(const char *name, int width, int height) {
        auto window = new Window(&windows, name, width, height);

        windows.emplace_back(window);

        glDebugMessageCallback(MessageCallback, nullptr);

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

    mat4 transform(vec3 translation, vec3 rotation, vec3 scale) {
        return translate(mat4(1), translation) * glm::toMat4(quat(rotation)) * glm::scale(mat4(1), scale);
    }

    mat4 rotate(vec3 rotation) {
        return glm::toMat4(quat(rotation));
    }


}