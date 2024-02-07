#include "Event_Poller.h"

#include <Util/Debug.h>
#include <fmt/core.h>

#include <Graphics/Window.h>
#include <Util/Eventable.h>

namespace OddityEngine::Event {
    void update() {
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            Graphics::Window::event(event);
        }
    }
}
