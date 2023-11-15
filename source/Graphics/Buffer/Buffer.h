#ifndef ODDITYENGINE_BUFFER_H
#define ODDITYENGINE_BUFFER_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace OddityEngine {
    namespace Graphics {
        enum BufferIndex {
            SCREEN,
            OBJECT,
            TRANSFORM,
            SHAPE,
            VERTEX,
            NORMAL,
            UV,
            MATERIAL,
        };

        template<typename T>
        class Buffer {
        public:
        protected:
            GLuint ID;
            GLuint type;
            GLuint usage;
            size_t size = 0;

            void resize(size_t size) {
                GLuint new_buffer;
                glGenBuffers(1, &new_buffer);

                glBindBuffer(GL_COPY_READ_BUFFER, ID);
                GLint buffersize = this->size;

                glBindBuffer(GL_COPY_WRITE_BUFFER, new_buffer);
                glNamedBufferData(new_buffer, size, nullptr, usage);

                glCopyNamedBufferSubData(ID, new_buffer, 0, 0, buffersize);

                glDeleteBuffers(1, &ID);

                ID = new_buffer;
                this->size = size;
            }

        public:
            explicit Buffer(GLuint type = GL_SHADER_STORAGE_BUFFER, GLuint usage = GL_DYNAMIC_DRAW) : type(type), usage(usage), ID() {
                glGenBuffers(1, &ID);
            }

            size_t add(size_t size, void* data) {
                glBindBuffer(type, ID);
                GLint buffersize = this->size;

                size_t total_size = buffersize + size;

                resize(total_size);

                glBindBuffer(type, ID);
                glNamedBufferSubData(ID, buffersize, size, data);

                return buffersize;
            }

            void set(size_t offset, size_t size, void* data) {
                glBindBuffer(type, ID);
                glNamedBufferSubData(ID, offset, size, data);
            }

            void remove(size_t offset, size_t size) {
                if (offset + size >= this->size) {
                    resize(offset);
                }
            }

            GLuint get_type() {
                return type;
            }

            operator GLuint() const {
                return ID;
            }

        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_BUFFER_H
