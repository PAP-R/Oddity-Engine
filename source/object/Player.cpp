#include "Player.h"
#include "source/base/tools/Debug.h"
#include <fmt/core.h>
using namespace fmt;

#include <stdio.h>

void Player::move(float deltaTime) {
    camera->fov = baseFOV / (zoomed ? zoom : 1);
    camera->fov = camera->fov > maxFOV ? maxFOV : camera->fov;
    Debug::add_text(fmt::format("Zoom: {}\nFOV: {}\n", zoom, camera->fov));

    camera->position = camera->position + (movement.x * camera->direction() + movement.z * camera->right()) * deltaTime * speed;
}

void Player::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_W:
                movement += vec3(1.0f, 0.0f, 0.0f);
                break;
            case GLFW_KEY_S:
                movement -= vec3(1.0f, 0.0f, 0.0f);
                break;
            case GLFW_KEY_D:
                movement += vec3(0.0f, 0.0f, 1.0f);
                break;
            case GLFW_KEY_A:
                movement -= vec3(0.0f, 0.0f, 1.0f);
                break;
            case GLFW_KEY_1:
                camera->fov += 1;
                break;
            case GLFW_KEY_2:
                camera->fov -= 1;
                break;
            case GLFW_KEY_3:
                mouseSpeed += 0.001f;
                printf("MouseSpeed: %f\n", mouseSpeed);
                break;
            case GLFW_KEY_4:
                mouseSpeed -= 0.001f;
                printf("MouseSpeed: %f\n", mouseSpeed);
                break;
        }
    }

    if(action == GLFW_RELEASE) {
        switch(key) {
            case GLFW_KEY_W:
                movement -= vec3(1.0f, 0.0f, 0.0f);
                break;
            case GLFW_KEY_S:
                movement += vec3(1.0f, 0.0f, 0.0f);
                break;
            case GLFW_KEY_D:
                movement -= vec3(0.0f, 0.0f, 1.0f);
                break;
            case GLFW_KEY_A:
                movement += vec3(0.0f, 0.0f, 1.0f);
                break;
        }
    }
}

void Player::cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    camera->angle.x -= mouseSpeed * deltaTime * xpos;
    camera->angle.x = fmodf(camera->angle.x, two_pi<float>());
    camera->angle.y -= mouseSpeed * deltaTime * ypos;
    camera->angle.y = fmodf(camera->angle.y, two_pi<float>());
    camera->angle.y = clamp(camera->angle.y, radians(-90.0f), radians(90.0f));

    glfwSetCursorPos(window, 0, 0);
}

void Player::mouse_callback(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch(button) {
            case GLFW_MOUSE_BUTTON_RIGHT:
                zoomed = true;
        }
    }

    else if (action == GLFW_RELEASE) {
        switch(button) {
            case GLFW_MOUSE_BUTTON_RIGHT:
                zoomed = false;
        }
    }
}

void Player::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (zoomed) {
        zoom += yoffset * zoomstep;
        float minZoom = baseFOV / maxFOV;
        zoom = zoom < minZoom ? minZoom : zoom;
    }
    else {
        speed += yoffset;
    }
}