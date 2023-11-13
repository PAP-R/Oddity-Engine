#include "Window.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "glm/gtc/matrix_transform.hpp"

#include <algorithm>

#include <Util/Debug.h>
#include <Graphics/OpenGL/Buffer/Bufferobject.h>

namespace OddityEngine {
    namespace Graphics {
        std::vector<Window*> windows;

        void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
            Debug::message(message);
        }

        void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
            for (auto w : windows) {
                if (w->get_window() == window) {
                    w->size.x = width;
                    w->size.y = height;

                    w->is_open = width != 0 && height != 0;

                    if (w->scene != nullptr) {
                        w->scene->set_size(w->size);
                    }
                }
            }
        }

        GLFWwindow* make_window(const char* name, int width, int height, ImGuiContext** context) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);
            if (window == NULL) {
                glfwTerminate();
                Debug::error("Failed to create Window");
            }

            glfwMakeContextCurrent(window);

            *context = ImGui::CreateContext();
            ImGui::SetCurrentContext(*context);
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init();

            if (glewInit() != GLEW_OK) {
                Debug::error("Failed to initialize GLEW");
            }

            glewExperimental = true;

            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_BLEND);
            glEnable(GL_MULTISAMPLE);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glClearColor(0, 0, 0, 1);

            GLuint vertex_array_ID;
            glGenVertexArrays(1, &vertex_array_ID);
            glBindVertexArray(vertex_array_ID);

            return window;
        }

        Window::Window(const char *name, int width, int height) : size(width, height), window(make_window(name, width, height, &context)), view_vertex_shader(GL_VERTEX_SHADER, "view.vert"), view_fragment_shader(GL_FRAGMENT_SHADER, "view.frag"), view_program(view_vertex_shader, view_fragment_shader), screenbuffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW) {
            windows.emplace_back(this);
            glfwMakeContextCurrent(window);

            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
            glDebugMessageCallback(MessageCallback, nullptr);

            std::vector<float> screen = {
                    -1.0f, -1.0f, 1.0f,
                    1.0f, -1.0f, 1.0f,
                    -1.0f, 1.0f, 1.0f,
                    1.0f, 1.0f, 1.0f,
                    1.0f, -1.0f, 1.0f,
                    -1.0f, 1.0f, 1.0f,
            };

            create_buffer_object_list(&screenbuffer, screen);
        }

        Window::~Window() {
            glfwMakeContextCurrent(window);
            ImGui::SetCurrentContext(context);

            glfwDestroyWindow(window);

            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext(context);
        }

        bool Window::update() {
            glfwMakeContextCurrent(window);
            ImGui::SetCurrentContext(context);

            glfwPollEvents();

            if (glfwWindowShouldClose(window)) {
                return false;
            }

            if (!is_open) {
                return true;
            }

            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
            Debug::update();

            if (scene != nullptr && scene->get_layers() > 0) {
                scene->update();

                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                glViewport(0, 0, size.x, size.y);

                glUseProgram(view_program);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D_ARRAY, scene->get_render_texture());
                glBindSampler(0, 0);
                int layers = scene->get_layers();
                glUniform1ui(view_program.uniform_location("texture_count"), scene->get_layers());

                glBindBufferBase(scene->get_texture_transform_buffer()->get_type(), 3, *scene->get_texture_transform_buffer());

                glm::mat4 screen_perspective = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 2.0f);
                glm::mat4 screen_projection = screen_perspective * glm::lookAt(glm::vec3(0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
                glUniformMatrix4fv(view_program.uniform_location("screen_projection"), 1, GL_FALSE, &screen_projection[0][0]);

                glUniform2f(view_program.uniform_location("view_size"), size.x, size.y);

                glEnableVertexAttribArray(0);

                glBindBuffer(screenbuffer.get_type(), screenbuffer);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                glDisableVertexAttribArray(0);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            return true;
        }

        size_t Window::add_renderer(Render::Interface *renderer) {
            return 0;
        }

        //TODO
        glm::vec<2, GLsizei> Window::get_pos() {
            return glm::vec<2, GLsizei>();
        }

        glm::vec<2, GLsizei> Window::get_size() {
            return size;
        }

        GLFWwindow *Window::get_window() {
            return window;
        }

        ImGuiContext *Window::get_context() {
            return context;
        }

        bool Window::update_all() {
            for (auto w : windows) {
                if (!w->update()) {
                    windows.erase(std::remove(windows.begin(), windows.end(), w));
                    delete(w);
                }
            }

            return !windows.empty();
        }

        void Window::set_scene(Render::Scene *scene) {
            this->scene = scene;
            scene->set_size(size);
        }


    } // OddityEngine
} // Graphics