#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <glm/glm.hpp>

namespace OddityEngine {
    class Window {
    protected:
        SDL_Window* _window = nullptr;
        SDL_Surface* _surface = nullptr;

        glm::ivec2 _size = {};

    public:
        Window(const char* name, int width, int height, unsigned int flags = 0);
        ~Window();

        void update();

        void set_size(glm::ivec2 size);

        static bool update_all();
        static void terminat_all();
        static Window* get(SDL_Window* window);
        static Window* get(Uint32 windowID);
    };
}


#endif //WINDOW_H
