#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <imgui.h>

#include <Graphics/Buffer/Buffer.h>

#include "Render/Scene.h"
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

        Program view_program;

        Scene* scene = nullptr;

    public:
        Window(const char* name, int width, int height, unsigned int flags);
        ~Window();

        bool update();

        void set_scene(Scene* scene);

        static bool update_all();
        static void terminate_all();
        static Window* get(SDL_Window* window);
    };
}

#endif //WINDOW_H
