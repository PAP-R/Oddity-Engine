#ifndef ODDITYENGINE_CONTROL_H
#define ODDITYENGINE_CONTROL_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
using namespace glm;

#include "Camera.h"

namespace Control {
    void set_camera(Camera * cam);

    void set_callbacks(GLFWwindow* window);

    void update(float deltatime);

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
}

#endif //ODDITYENGINE_CONTROL_H
