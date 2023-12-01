#include "Time.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace OddityEngine {
    namespace Time {
        double now = glfwGetTime();
        double start = now;
        double last = now;
        double delta;
        size_t frame = 0;

        void update() {
            now = glfwGetTime();
            delta = now - last;
            last = now;
            frame++;
        }

        double get() {
            return now;
        }

        double get_start() {
            return start;
        }

        double get_delta() {
            return delta;
        }

        size_t get_frame() {
            return frame;
        }
    } // Time
} // OddityEngine
