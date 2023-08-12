#ifndef ODDITYENGINE_GRAPHICS_H
#define ODDITYENGINE_GRAPHICS_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui_internal.h"

#include "Window.h"

namespace OddityEngine::Graphics {
    void init();
    void terminate();

    bool update();

    Window* create_window(const char *name, int width, int height);
};


#endif //ODDITYENGINE_GRAPHICS_H
