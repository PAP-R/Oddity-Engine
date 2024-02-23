#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>

namespace OddityEngine::Graphics {
    struct alignas(16) Object_struct {
        GLuint shapes_start = 0;
        GLuint shapes_count = 0;
    };

    class Object {

    };
}

#endif //OBJECT_H
