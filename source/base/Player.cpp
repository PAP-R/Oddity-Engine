#include "Player.h"

#include <stdio.h>

vec3 Player::direction() const {
    return {
            cos(angle.y) * sin(angle.x),
            sin(angle.y),
            cos(angle.y) * cos(angle.x)
            };
}

vec3 Player::right() const {
    return {
            sin(angle.x - 3.14f / 2.0f),
            0,
            cos(angle.x - 3.14f / 2.0f)
            };
}

vec3 Player::up() const {
    return cross(right(), direction());
}

void Player::move() {
    double currentTime = glfwGetTime();
    auto moveDeltaTime = float(currentTime - time);
    time = currentTime;

    position = position + (movement.x * direction() + movement.z * right()) * moveDeltaTime * speed;
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
                fov += 1;
                break;
            case GLFW_KEY_2:
                fov -= 1;
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
    angle.x -= mouseSpeed * deltaTime * xpos;
    angle.y -= mouseSpeed * deltaTime * ypos;
    angle.y = clamp(angle.y, radians(-90.0f), radians(90.0f));

    glfwSetCursorPos(window, 0, 0);
}