#ifndef ODDITYENGINE_BUFFER_H
#define ODDITYENGINE_BUFFER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace OddityEngine {
    namespace Graphics {

        class Buffer {
        protected:
            GLuint ID;
            GLuint type;
            GLuint usage;
            size_t size = 0;

            void resize(size_t size);

        public:
            Buffer() = default;
            Buffer(GLuint type, GLuint usage);
            size_t add(size_t size, void* data);
            void set(size_t offset, size_t size, void* data);
            void remove(size_t offset, size_t size);
            GLuint get_type();

            operator GLuint() const;
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_BUFFER_H
