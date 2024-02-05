#include "Window.h"

#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Util/Debug.h>

#include "Math/random.h"
#include "Shader/Shader.h"

namespace OddityEngine::Graphics {
    std::vector<Window*> window_list;

    void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        Debug::message(message);
    }

    auto create_window(const char* name, int width, int height) {
        auto window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (window == nullptr) {
            Debug::error("Failed to create window");
        }

        return window;
    }

    auto create_gl_contextCurrent(SDL_Window* window) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        const auto context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, context);

        if (glewInit() != GLEW_OK) {
            Debug::error("Failed to initialize GLEW");
        }

        if (!GLEW_ARB_shader_viewport_layer_array) {
            Debug::error("Needed extension not available");
        }

        glewExperimental = true;

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_MULTISAMPLE);
        // glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);
        // glEnable(GL_CULL_FACE);

        glClearColor(0.3, 0, 0, 1);

        return context;
    }

    Window::Window(const char* name, int width, int height) : window(create_window(name, width, height)), gl_context(create_gl_contextCurrent(window)), screenbuffer(GL_ARRAY_BUFFER), view_vertex_shader(GL_VERTEX_SHADER, "view.vert"), view_fragment_shader(GL_FRAGMENT_SHADER, "view.frag"), view_program({view_vertex_shader, view_fragment_shader}) {
        window_list.emplace_back(this);

        glGenVertexArrays(1, &vertex_array);
        glBindVertexArray(vertex_array);

        glDebugMessageCallback(MessageCallback, nullptr);

        Vector screen = {
            0.0f, 0.0f, -1.0f,
            0.0f, 1.0f, -1.0f,
            1.0f, 0.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            0.0f, 1.0f, -1.0f,
            1.0f, 0.0f, -1.0f,
        };

        auto indices = screenbuffer.insert_back(screen.size(), screen.data());
    }

    Window::~Window() {
        glDeleteVertexArrays(1, &vertex_array);

        ImGui::SetCurrentContext(imgui_context);

        SDL_GL_DeleteContext(gl_context);

        SDL_DestroyWindow(window);
    }

    bool Window::update() {
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT) {
            return false;
        }

        SDL_GL_MakeCurrent(window, gl_context);

        int width, height;

        SDL_GetWindowSizeInPixels(window, &width, &height);

        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glUseProgram(view_program);

        glm::mat4 screen_perspective = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
        glm::mat4 screen_projection = screen_perspective * glm::lookAt(glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.5, 0.5, -1), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(view_program.uniform_location("screen_projection"), 1, GL_FALSE, &screen_projection[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(screenbuffer.get_type(), screenbuffer);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);

        SDL_GL_SwapWindow(window);

        return true;
    }

    bool Window::update_all() {
        for (auto w : window_list) {
            if (!w->update()) {
                window_list.erase(std::remove(window_list.begin(), window_list.end(), w), window_list.end());
                delete(w);
            }
        }

        return !window_list.empty();
    }

    void Window::terminate_all() {
        for (auto w : window_list) {
            delete(w);
        }
        window_list.clear();
    }

    Window* Window::get(SDL_Window* window) {
        return *std::ranges::find_if(window_list, [&window](Window* w){return window == w->window;});
    }
}
