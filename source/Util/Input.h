#ifndef ODDITYENGINE_INPUT_H
#define ODDITYENGINE_INPUT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <map>

namespace OddityEngine {

    class Input {
    protected:
        static std::vector<Input*> instances;
        static std::vector<int> keys_status;
        static std::map<GLFWwindow*, int> cursor_modes;

        virtual void key_update(int key, int action) = 0;
        virtual void mouse_update(float xpos, float ypos) = 0;
    public:
        Input();

        bool enabled = false;

        static void init(GLFWwindow* window);

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        static void set_cursor_mode(int mode);
        static int get_cursor_mode();
        static void set_cursor_position(double xpos, double ypos);
    };

} // OddityEngine

#endif //ODDITYENGINE_INPUT_H
