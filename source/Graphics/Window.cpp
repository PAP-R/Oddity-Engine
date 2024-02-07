#include "Window.h"

#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Util/Debug.h>

#include "Math/random.h"
#include "Shader/Shader.h"

#include <cstdio>

#include "Camera.h"
#include "Camera.h"

namespace OddityEngine::Graphics {
    std::vector<Window*> window_list;

    void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        Debug::message(message);
    }

    auto create_window(const char* name, const int width, const int height, const unsigned int flags) {
        auto window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags | SDL_WINDOW_OPENGL);
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

    Window::Window(const char* name, const int width, const int height, const unsigned int flags) : window(create_window(name, width, height, flags)), gl_context(create_gl_contextCurrent(window)), screenbuffer(GL_ARRAY_BUFFER), view_program({Shader(GL_VERTEX_SHADER, "view.vert"), Shader(GL_FRAGMENT_SHADER, "view.frag")}), size(width, height) {
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

    void Window::update() {
        SDL_GL_MakeCurrent(window, gl_context);

        glClear(GL_COLOR_BUFFER_BIT);

        if (scene != nullptr && scene->layer_count() > 0) {
            scene->update();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            int width, height;

            SDL_GetWindowSizeInPixels(window, &width, &height);

            glViewport(0, 0, width, height);

            glUseProgram(view_program);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D_ARRAY, scene->get_render_texture());
            glBindSampler(0, 0);

            glUniform1ui(view_program.uniform_location("texture_count"), scene->layer_count());

            glBindBufferBase(scene->get_texture_transform_buffer()->get_type(), 3, scene->get_texture_transform_buffer()->get_ID());

            glm::mat4 screen_perspective = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
            glm::mat4 screen_projection = screen_perspective * glm::lookAt(glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.5, 0.5, -1), glm::vec3(0, 1, 0));
            glUniformMatrix4fv(view_program.uniform_location("screen_projection"), 1, GL_FALSE, &screen_projection[0][0]);

            glUniform2f(view_program.uniform_location("view_size"), width, height);

            glEnableVertexAttribArray(0);
            glBindBuffer(screenbuffer.get_type(), screenbuffer);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glDisableVertexAttribArray(0);
        }

        SDL_GL_SwapWindow(window);
    }

    void Window::set_size(glm::vec2 size) {
        if (scene != nullptr) {
            scene->set_size(size);
        }

        this->size = size;
    }

    void Window::set_scene(Scene* scene) {
        this->scene = scene;

        int width, height;

        SDL_GetWindowSizeInPixels(window, &width, &height);

        scene->set_size({width, height});
    }

    bool Window::update_all() {
        for (auto w : window_list) {
            w->update();
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
        const auto window_iterator = std::ranges::find_if(window_list, [&window](Window* w){return window == w->window;});
        if (window_iterator == window_list.end()) {
            return nullptr;
        }
        return *window_iterator;
    }

    void Window::event(const SDL_Event& event) {
        const auto window = get(SDL_GetWindowFromID(event.window.windowID));
        switch (event.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                // if (!event.key.repeat) {
                //     Debug::message(fmt::format("Key {} + {} state {} \t at {}", event.key.keysym.sym, event.key.keysym.mod, event.key.state, event.key.timestamp, event.key.repeat));
                // }
                break;
            case SDL_TEXTINPUT:
                Debug::message(fmt::format("Textinput {} on window {}", event.text.text, event.window.windowID));
            break;
            case SDL_TEXTEDITING:
                Debug::message(fmt::format("Textedit {}", event.edit.text));
            break;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        window->set_size({event.window.data1, event.window.data2});
                        Debug::message(fmt::format("Window {} resized to [{} / {}]", event.window.windowID, event.window.data1, event.window.data2));
                    break;
                    case SDL_WINDOWEVENT_CLOSE:
                        window_list.erase(std::remove(window_list.begin(), window_list.end(), window), window_list.end());
                        delete(window);
                        Debug::message(fmt::format("Window {} closed", event.window.windowID));
                    break;
                }
            break;
        }
    }
}
