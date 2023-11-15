#include "Input.h"

#include <vector>
#include <map>

namespace OddityEngine {
    std::vector<Input*> Input::instances;
    std::map<GLFWwindow*, int> Input::cursor_modes;

    Input::Input() {
        instances.emplace_back(this);
    }

    void Input::init(GLFWwindow* window) {
        cursor_modes.emplace(window, glfwGetInputMode(window, GLFW_CURSOR));
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
    }

    void Input::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS) {
            if (cursor_modes[window] == GLFW_CURSOR_NORMAL) {
                set_cursor_mode(GLFW_CURSOR_DISABLED);
                set_cursor_position(0, 0);
            }
            else {
                int width, height;
                glfwGetWindowSize(window, &width, &height);
                set_cursor_mode(GLFW_CURSOR_NORMAL);
                set_cursor_position(width / 2, height / 2);
            }
        }

        for (auto i : instances) {
            if (i->enabled) {
                i->key_update(key, action);
            }
        }
    }

    void Input::cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
        for (auto i : instances) {
            if (i->enabled) {
                i->mouse_update(xpos, ypos);
            }
        }
    }

    void Input::set_cursor_mode(int mode) {
        auto window = glfwGetCurrentContext();
        cursor_modes[window] = mode;
        glfwSetInputMode(window, GLFW_CURSOR, mode);
    }

    int Input::get_cursor_mode() {
        auto window = glfwGetCurrentContext();
        return cursor_modes[window];
    }

    void Input::set_cursor_position(double xpos, double ypos) {
        auto window = glfwGetCurrentContext();
        glfwSetCursorPos(window, xpos, ypos);
    }
} // OddityEngine