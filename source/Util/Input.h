#ifndef ODDITYENGINE_INPUT_H
#define ODDITYENGINE_INPUT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace OddityEngine {

    class Input {
    protected:
        static std::vector<Input*> instances;
        static std::vector<int> keys_status;
        bool enabled = false;

        virtual void update() = 0;
    public:
        Input();
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    };

} // OddityEngine

#endif //ODDITYENGINE_INPUT_H
