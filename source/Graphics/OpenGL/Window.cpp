#include "Window.h"

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Util/Debug.h"
#include <cstdio>
#include <algorithm>

#include "Math/random.h"
#include "Graphics/OpenGL/Shader/Shader.h"

#include <Graphics/Camera.h>

namespace OddityEngine::Graphics::OpenGL {
    void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        Debug::message("Source: {}\n Type: {}\n Severity: {}\n Error: {}", source, type, severity, message);
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

        glewExperimental = true;

        auto glew_status = glewInit();

        if (glew_status != GLEW_OK) {
            std::cout << glewGetErrorString(glew_status);
            Debug::error("Failed to initialize GLEW");
        }

        std::cout << std::endl << glGetString(GL_RENDERER) << std::endl;


        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_MULTISAMPLE);
        // glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);
        // glEnable(GL_CULL_FACE);

        glClearColor(0, 0, 0, 1);

        return context;
    }

    Window::Window(const char* name, const int width, const int height, const unsigned int flags) : Graphics::Window(name, width, height, flags | SDL_WINDOW_OPENGL), gl_context(create_gl_contextCurrent(window)), screenbuffer(GL_ARRAY_BUFFER), view_program({Shader(GL_VERTEX_SHADER, "view.vert"), Shader(GL_FRAGMENT_SHADER, "view.frag")}) {
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

    void Window::make_current() {
        SDL_GL_MakeCurrent(window, gl_context);
    }

    void Window::update() {
        make_current();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

            scene->get_texture_transform_buffer()->bind_base(TEXTURE_TRANSFORM);

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

    void Window::set_scene(Scene* scene) {
        this->scene = scene;

        int width, height;

        SDL_GetWindowSizeInPixels(window, &width, &height);

        scene->set_size({width, height});
    }
}
