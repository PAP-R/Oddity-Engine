#ifndef ODDITYENGINE_WINDOW_H
#define ODDITYENGINE_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui_internal.h>

#include <glm/glm.hpp>

#include <vector>

#include <Graphics/OpenGL/Shader/Shader.h>
#include <Graphics/OpenGL/Shader/Program.h>
#include <Graphics/OpenGL/Buffer/Buffer.h>
#include <Graphics/OpenGL/Render/Interface.h>
#include "Graphics/OpenGL/Render/Scene.h"

namespace OddityEngine {
    namespace Graphics {

        class Window {
        protected:
            GLFWwindow* window;
            ImGuiContext* context;

            Shader view_vertex_shader;
            Shader view_fragment_shader;
            Program view_program;

            Buffer screenbuffer;

            glm::vec<2, int> size;

            Render::Scene* scene = nullptr;

            bool is_open = true;

            static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        public:
            Window(const char* name, int width, int height);

            ~Window();

            bool update();

            size_t add_renderer(Render::Interface* renderer);

            glm::vec<2, GLsizei> get_pos();
            glm::vec<2, GLsizei> get_size();

            GLFWwindow* get_window();
            ImGuiContext* get_context();

            static bool update_all();

            void set_scene(Render::Scene* scene);
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_WINDOW_H
