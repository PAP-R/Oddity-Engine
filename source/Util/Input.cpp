#include "Input.h"

#include <vector>

namespace OddityEngine {
    std::vector<Input*> Input::instances;
    std::vector<int> Input::keys_status(GLFW_KEY_LAST + 1);

    Input::Input() {
        instances.emplace_back(this);
    }

    void Input::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key >= 0) {
            keys_status[key] = action;
        }

        for (auto i : instances) {
            if (i->enabled) {
                i->update();
            }
        }
    }
} // OddityEngine