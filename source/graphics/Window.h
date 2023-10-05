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
#include "buffer/Bufferobject.h"
#include "render/Renderer.h"

namespace OddityEngine::Graphics {
    class Window {
    private:
        GLFWwindow* window;
        ImGuiContext* context;
        std::vector<Window*>* list;

        std::vector<OddityEngine::Graphics::Render::Renderer*> renderers;

        Shader::Shader view_vertex_shader = Shader::Shader(GL_VERTEX_SHADER, "shaders/view.vert");
        Shader::Shader view_fragment_shader = Shader::Shader(GL_FRAGMENT_SHADER, "shaders/view.frag");
        GLuint view_program;

        GLuint rbo_color = 0;
        GLuint rbo_depth_stencil = 0;

        GLuint frame_buffer = 0;
        GLuint render_texture = 0;

        Buffer::Buffer screenbuffer;
        Buffer::Buffer texture_transform_buffer;

        vec2 size;
        vec<2, int> view_size;

        GLsizei layers = 0;

        int sample_size = 1;

        void texture_size() const;

    public:
        Window(std::vector<Window*>* list, const char * name, int width = 400, int height = 400);

        ~Window();

        void update();

        uint add_renderer(Render::Renderer* renderer);

        vec<2, GLsizei> get_pos();
        vec<2, GLsizei> get_size();
        bool is_open() const;

        GLFWwindow* get_window();
        ImGuiContext* get_context();

        GLuint get_frame_buffer() const;

        Buffer::Buffer* get_texture_transform_buffer();
    };

}


#endif //ODDITYENGINE_WINDOW_H
