#ifndef ODDITYENGINE_GRAPHICS_H
#define ODDITYENGINE_GRAPHICS_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui_internal.h"

#include "Window.h"
#include "Tracer.h"

namespace OddityEngine::Graphics {
    void init();
    void terminate();

    bool update();

    Window* create_window(const char *name, int width, int height);
    Tracer* create_tracer(Window* window, size_t width, size_t height);
};


#endif //ODDITYENGINE_GRAPHICS_H
