#include "Graphics.h"

#include <SDL.h>

#include <Util/Debug.h>
#include <Graphics/Window.h>

namespace OddityEngine::Graphics {
    void init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            Debug::error("Failed to initialize SDL Video");
        }
    }

    void terminate() {
        Window::terminate_all();
        SDL_Quit();
    }

    bool update() {
        return Window::update_all();
    }

}