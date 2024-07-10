#include "Graphics.h"

#include <SDL.h>

#include <Window/Window.h>

#include <Util/Debug.h>

namespace OddityEngine::Graphics {
    void init() {
        Debug::assert_error(SDL_Init(SDL_INIT_EVERYTHING) < 0, "Failed to initialize SDL");
        Debug::message("Graphics initilized");
    }

    void terminate() {
        SDL_Quit();
        Debug::message("Graphics terminated");
    }

    bool update() {
        return Window::count() != 0;
    }
}
