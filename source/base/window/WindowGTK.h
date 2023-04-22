#ifndef ODDITYENGINE_WINDOWGTK_H
#define ODDITYENGINE_WINDOWGTK_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

class WindowGTK {
    vec2 position;
    vec2 size;
public:
    WindowGTK(const char *name, size_t width, size_t height, int x=0, int y=0);
};


#endif //ODDITYENGINE_WINDOWGTK_H
