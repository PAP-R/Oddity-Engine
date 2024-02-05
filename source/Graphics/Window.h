#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <imgui.h>

#include <Graphics/Buffer/Buffer.h>

#include "Shader/Program.h"
#include "Shader/Shader.h"

namespace OddityEngine::Graphics {
    class Window {
    protected:
        SDL_Window* window = nullptr;
        SDL_Surface* surface = nullptr;
        SDL_Event event;

        SDL_GLContext gl_context;

        ImGuiContext* imgui_context = nullptr;

        GLuint vertex_array;

        Buffer<GLfloat> screenbuffer;

        Shader view_vertex_shader;
        Shader view_fragment_shader;
        Program view_program;

    public:
            Window(const char* name, int width, int height);
        ~Window();

        bool update();

        static bool update_all();
        static void terminate_all();
        static Window* get(SDL_Window* window);
    };
}

#endif //WINDOW_H
