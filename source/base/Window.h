#ifndef ODDITYENGINE_WINDOW_H
#define ODDITYENGINE_WINDOW_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
using namespace glm;

class Window {
private:
    GLFWwindow *window;
    vec2 pos;
    vec2 size;

public:
    Window(const char * name, size_t width = 400, size_t height = 400, int x = 0, int y = 0);

    ~Window();
};


#endif //ODDITYENGINE_WINDOW_H
