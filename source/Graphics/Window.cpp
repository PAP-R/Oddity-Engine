#include "Window.h"

#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include <vector>

#include <Util/Debug.h>

namespace OddityEngine::Graphics {
    std::vector<Window*> window_list;

    Window::Window(const char* name, int width, int height) {
        window_list.emplace_back(this);

        window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN_DESKTOP);
        if (window == nullptr) {
            Debug::error("Failed to create Window");
        }

        gl_context = SDL_GL_CreateContext(window);
    }

    Window::~Window() {
        ImGui::SetCurrentContext(imgui_context);

        SDL_GL_DeleteContext(gl_context);

        SDL_DestroyWindow(window);
    }

    bool Window::update() {
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT) {
            return false;
        }

        SDL_GL_MakeCurrent(window, gl_context);

        SDL_UpdateWindowSurface(window);

        return true;
    }

    bool Window::update_all() {
        for (auto w : window_list) {
            if (!w->update()) {
                window_list.erase(std::remove(window_list.begin(), window_list.end(), w), window_list.end());
                delete(w);
            }
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
        return *std::ranges::find_if(window_list, [&window](Window* w){return window == w->window;});
    }
}
