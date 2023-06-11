#include <cstdio>
#include "Control.h"

#define SPEED 5.0f
#define SENSITIVITY 0.2f

namespace Control {
    Camera* camera;
    vec3 deltapos;
    vec3 deltaangle;

    void set_camera(Camera * cam) {
        camera = cam;
    }

    void set_callbacks(GLFWwindow* window) {
        deltapos = vec3(0);
        deltaangle = vec3(0);

        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
    }

    void update(float deltatime) {
        camera->position += vec3(camera->right().x * deltapos.x + camera->direction().x * deltapos.z, camera->up().y * -deltapos.y, camera->right().z * deltapos.x + camera->direction().z * deltapos.z) * SPEED * deltatime;

        camera->angle += deltaangle * SENSITIVITY * deltatime;
        camera->angle.x = fmodf(camera->angle.x, two_pi<float>());
        camera->angle.y = fmodf(camera->angle.y, two_pi<float>());
        camera->angle.x = clamp(camera->angle.x, radians(-90.0f), radians(90.0f));
        deltaangle = vec3(0);
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_A:
                    deltapos.x -= 1;
                    break;
                case GLFW_KEY_D:
                    deltapos.x += 1;
                    break;
                case GLFW_KEY_S:
                    deltapos.z += 1;
                    break;
                case GLFW_KEY_W:
                    deltapos.z -= 1;
                    break;
                case GLFW_KEY_SPACE:
                    deltapos.y += 1;
                    break;
                case GLFW_KEY_LEFT_CONTROL:
                    deltapos.y -= 1;
                    break;
                case GLFW_KEY_UP:
                    deltaangle.x -= 1;
                    break;
                case GLFW_KEY_DOWN:
                    deltaangle.x += 1;
                    break;
                case GLFW_KEY_LEFT:
                    deltaangle.y += 1;
                    break;
                case GLFW_KEY_RIGHT:
                    deltaangle.y -= 1;
                    break;
                case GLFW_KEY_R:
                    camera->angle = vec3(0);
                    break;
            }
        }
        if (action == GLFW_RELEASE) {
            switch (key) {
                case GLFW_KEY_A:
                    deltapos.x += 1;
                    break;
                case GLFW_KEY_D:
                    deltapos.x -= 1;
                    break;
                case GLFW_KEY_S:
                    deltapos.z -= 1;
                    break;
                case GLFW_KEY_W:
                    deltapos.z += 1;
                    break;
                case GLFW_KEY_SPACE:
                    deltapos.y -= 1;
                    break;
                case GLFW_KEY_LEFT_CONTROL:
                    deltapos.y += 1;
                    break;
                case GLFW_KEY_UP:
                    deltaangle.x += 1;
                    break;
                case GLFW_KEY_DOWN:
                    deltaangle.x -= 1;
                    break;
                case GLFW_KEY_LEFT:
                    deltaangle.y -= 1;
                    break;
                case GLFW_KEY_RIGHT:
                    deltaangle.y += 1;
                    break;
            }
        }
    }

    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
//        printf("[%f, %f]\n", camera->angle.x, camera->angle.y);
        deltaangle.x += ypos;
        deltaangle.y -= xpos;
        glfwSetCursorPos(window, 0, 0);
    }
}
