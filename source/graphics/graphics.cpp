#pragma once

#include "graphics.h"


#include <string>
#include <chrono>


#include "vulkan.h"
#include "../character/movement.h"


namespace Graphics {
    void Engine::start(int width, int height, std::string title) {
        Engine::framebufferResized = false;
        initWindow(width, height, title);
        vk.start(window);
    }

    void Engine::stop() {
        Engine::vk.stop();
    }

    bool Engine::update(Movement movement) {
        glfwPollEvents();
        vk.update(&framebufferResized, movement);
        return !glfwWindowShouldClose(window);
    }

    void Engine::fullScreen() {
        if (fullscreen) {
            glfwGetWindowPos(window, &posx, &posy);
            glfwGetWindowSize(window, &width, &height);

            glfwSetWindowMonitor(window, monitor, 0, 0, standardmode->width, standardmode->height, standardmode->refreshRate);
            fullscreen = false;
        }
        else {
            glfwSetWindowMonitor(window, NULL, posx, posy, width, height, standardmode->refreshRate);
            fullscreen = true;
        }

        printf("Fullscreen Toggle\n");
    }

    void Engine::setKeyCallback(GLFWkeyfun callback) {
        glfwSetKeyCallback(window, callback);
    }

    void Engine::setCursorPositionCallback(GLFWcursorposfun callback) {
        glfwSetCursorPosCallback(window, callback);
    }

    void Engine::setMouseButtonCallback(GLFWmousebuttonfun callback) {
        glfwSetMouseButtonCallback(window, callback);
    }

    void Engine::setScrollCallback(GLFWscrollfun callback) {
        glfwSetScrollCallback(window, callback);
    }

    void Engine::initWindow(int width, int height, std::string title) {
        this->width = width;
        this->height = height;

        glfwInit();

        monitor = glfwGetPrimaryMonitor();
        standardmode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Engine::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }
}