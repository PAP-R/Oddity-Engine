#ifndef ODDITYENGINE_GRAPHICS_H
#define ODDITYENGINE_GRAPHICS_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui_internal.h"

#include "Window.h"
#include "graphics/render/Tracer.h"

namespace OddityEngine::Graphics {
    void init();
    void terminate();

    bool update();

    Window* create_window(const char *name, int width, int height);
    Tracer* create_tracer(Window* window, size_t width, float ratio);

    mat4 transform(vec3 translation = vec3(0), vec3 rotation = vec3(0), vec3 scale = vec3(1));
    mat4 rotate(vec3 rotation);
};


#endif //ODDITYENGINE_GRAPHICS_H
