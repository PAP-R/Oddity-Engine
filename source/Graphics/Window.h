#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <imgui.h>

namespace OddityEngine::Graphics {
    class Window {
    protected:
        SDL_Window* window = nullptr;
        SDL_Surface* surface = nullptr;
        SDL_Event event;

        SDL_GLContext gl_context;

        ImGuiContext* imgui_context = nullptr;

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
