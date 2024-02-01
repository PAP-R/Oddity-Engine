#include "Time.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace OddityEngine {
    namespace Time {
        float now = glfwGetTime();
        float start = now;
        float last = now;
        float delta;
        size_t frame = 0;

        void update() {
            now = glfwGetTime();
            delta = now - last;
            last = now;
            frame++;
        }

        float get() {
            return now;
        }

        float get_start() {
            return start;
        }

        float get_delta() {
            return delta;
        }

        size_t get_frame() {
            return frame;
        }

        float get_fps() {
            return 1 / get_delta();
        }
    } // Time
} // OddityEngine
