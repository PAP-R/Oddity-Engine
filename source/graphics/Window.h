#ifndef ODDITYENGINE_WINDOW_H
#define ODDITYENGINE_WINDOW_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "imgui_internal.h"

using namespace glm;

#include <vector>

#include "Shader.h"
#include "buffer/Buffer.h"

namespace OddityEngine::Graphics {
    class Window {
    private:
        GLFWwindow* window;
        ImGuiContext* context;
        std::vector<Window*>* list;

        Shader::Shader view_vertex_shader = Shader::Shader(GL_VERTEX_SHADER, "shaders/view.vert");
        Shader::Shader view_fragment_shader = Shader::Shader(GL_FRAGMENT_SHADER, "shaders/view.frag");
        GLuint view_program;

        GLuint rbo_color = 0;
        GLuint rbo_depth_stencil = 0;

        GLuint frame_buffer = 0;
        GLuint render_texture = 0;

        Buffer::Buffer screenbuffer;

        vec2 size;

        float ratio = 0.4;
        float last_ratio = ratio;
        vec<2, int> render_size;
        vec<2, int> view_size;

        int sample_size = 1;

        void texture_size() const;

    public:
        Window(std::vector<Window*>* list, const char * name, int width = 400, int height = 400);

        ~Window();

        void begin_update();
        void end_update();

        vec<2, size_t> get_pos();
        vec<2, size_t> get_size();
        bool is_open() const;

        GLFWwindow* get_window();
        ImGuiContext* get_context();

        GLuint get_frame_buffer() const;
        vec<2, int> get_render_size() const;
    };

}


#endif //ODDITYENGINE_WINDOW_H
