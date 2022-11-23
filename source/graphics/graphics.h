#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <optional>

#include "graphic_libs.h"
#include "vulkan.h"

namespace Graphics {

    #define MAX_FRAMES_IN_FLIGHT 2
    #define VALIDATE true

    class Engine {
    private:
        GLFWwindow* window;
        GLFWmonitor* monitor;

        const GLFWvidmode* standardmode;


        int posx = 0;
        int posy = 0;
        int width = 400;
        int height = 400;

        bool framebufferResized = false;

        bool fullscreen = true;


    public:
        Vulkan vk;

        void start(int width, int height, std::string title);

        void stop();

        bool update(Movement movement);

        void fullScreen();

        void setKeyCallback(GLFWkeyfun callback);

        void setCursorPositionCallback(GLFWcursorposfun callback);

        void setMouseButtonCallback(GLFWmousebuttonfun callback);

        void setScrollCallback(GLFWscrollfun callback);

    private:
        void initWindow(int width, int height, std::string title);

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    };

}

#endif