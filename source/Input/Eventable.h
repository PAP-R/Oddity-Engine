#ifndef INPUT_H
#define INPUT_H

#include <backends/imgui_impl_sdl2.h>

namespace OddityEngine::Util {
    class Eventable {
    public:
        virtual void event(const SDL_Event& event) = 0;
    };
}


#endif //INPUT_H
