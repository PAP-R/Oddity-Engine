#ifndef ODDITYENGINE_GRAPHICS_H
#define ODDITYENGINE_GRAPHICS_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace OddityEngine {
    namespace Graphics {
        void init();
        void terminate();

        bool update();


    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_GRAPHICS_H
