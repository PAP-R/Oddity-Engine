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
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    Debug::message("Closing Window {}", event.window.windowID);
                    if (const auto window = Window::get(event.window.windowID); window != nullptr) {
                        window->~Window();
                    }
                    Debug::message("Closed Window {}", event.window.windowID);
                }
            }
        }

        return Window::update_all();
    }
}
