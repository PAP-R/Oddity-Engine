#ifndef ODDITYENGINE_WINDOW_H
#define ODDITYENGINE_WINDOW_H

#include <SDL.h>
#include <glm/glm.hpp>

namespace OddityEngine::Graphics {
    class Window {
    protected:
        SDL_Window *window = nullptr;
        SDL_Surface *surface = nullptr;

        glm::vec2 size;

    public:
        Window(const char *name, int width, int height, unsigned int flags);
        ~Window();

        virtual void make_current() = 0;
        virtual void update() = 0;

        virtual void set_size(glm::vec2 size);

        static bool update_all();
        static void terminate_all();
        static Window* get(SDL_Window* window);
        static void event(const SDL_Event& event);
    };
}

#endif //ODDITYENGINE_WINDOW_H
