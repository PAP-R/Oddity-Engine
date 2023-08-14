#ifndef ODDITYENGINE_BUFFER_H
#define ODDITYENGINE_BUFFER_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
using namespace glm;

namespace OddityEngine::Graphics::Buffer {
    class Buffer {
    private:
        GLuint ID;
        GLuint type;
        GLuint usage;
        size_t size = 0;

    public:
        Buffer(GLuint type, GLuint usage);
        size_t add(size_t size, void* data);
        void set(size_t offset, size_t size, void* data);
        GLuint get_type();

        operator unsigned int() const;
    };
}


#endif //ODDITYENGINE_BUFFER_H
