#include "Graphics.h"

#include <SDL2/SDL.h>


#include "Util/Debug.h"
#include "Window.h"

namespace OddityEngine {
    namespace Graphics {
        void init() {
            if (SDL_Init(SDL_INIT_VIDEO)) {
                Debug::error("Failed to initialize GLFW");
            }
        }

        void terminate() {
            glfwTerminate();
        }

        bool update() {
            return Window::update_all();
        }
    } // OddityEngine
} // Graphics