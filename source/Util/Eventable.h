#ifndef EVENTABLE_H
#define EVENTABLE_H

#include <SDL.h>

namespace OddityEngine::Util {
    class Eventable {
    public:
        bool active;
        virtual void event(const SDL_Event& event) = 0;
    };
}


#endif //EVENTABLE_H
