#include "Graphics.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

#include <Util/Debug.h>
#include "Window.h"

namespace OddityEngine {
    namespace Graphics {
        void init() {
            if (!glfwInit()) {
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