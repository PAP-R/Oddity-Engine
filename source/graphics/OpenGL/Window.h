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

namespace OddityEngine {
    namespace Graphics {

        class Window {
        protected:
            GLFWwindow* window;
            ImGuiContext* context;

            std::vector<Render::Interface*> renderers;

            Shader view_vertex_shader;
            Shader view_fragment_shader;
            Program view_program;

            GLuint rbo_color;
            GLuint rbo_depth_stencil;

            GLuint framebuffer;
            GLuint render_texture;

            Buffer screenbuffer;
            Buffer texture_transform_buffer;

            glm::vec<2, int> size;

            GLsizei layers = 1;

            int sample_size = 1;

            bool is_open = true;

            void texture_size() const;
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

            GLuint get_framebuffer() const;
            Buffer* get_texture_transform_buffer();

            static bool update_all();
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_WINDOW_H
