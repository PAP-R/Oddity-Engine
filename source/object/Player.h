#ifndef ODDITYENGINE_PLAYER_H
#define ODDITYENGINE_PLAYER_H

#include "external/glew/include/GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include "../base/interfaces/CallBack.h"
#include "source/base/objects/Camera.h"

class Player : virtual public CallBack{
    double time = glfwGetTime();
    float deltaTime = glfwGetTime();

    vec3 movement = vec3(0);

public:
    Camera *camera;

    float baseFOV = 90.0f;
    float maxFOV = 160.0f;
    float zoom = 2.0f;
    float zoomstep = 0.1f;
    bool zoomed = false;

    float speed = 3.0f;
    float mouseSpeed = 0.005f;

    void move();

    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void cursor_callback(GLFWwindow *window, double xpos, double ypos);
    void mouse_callback(GLFWwindow *window, int button, int action, int mods);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};


#endif //ODDITYENGINE_PLAYER_H
