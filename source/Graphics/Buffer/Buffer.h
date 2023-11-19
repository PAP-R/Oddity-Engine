#ifndef ODDITYENGINE_BUFFER_H
#define ODDITYENGINE_BUFFER_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <algorithm>
#include <vector>

namespace OddityEngine {
    namespace Graphics {
        enum BufferIndex {
            OBJECT = 3,
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
                size_t buffersize = this->size;

                glBindBuffer(GL_COPY_WRITE_BUFFER, new_buffer);
                glNamedBufferData(new_buffer, size, nullptr, usage);

                glCopyNamedBufferSubData(ID, new_buffer, 0, 0, std::min(size, buffersize));

                glDeleteBuffers(1, &ID);

                ID = new_buffer;
                this->size = size;
            }

        public:
            explicit Buffer(GLuint type = GL_SHADER_STORAGE_BUFFER, GLuint usage = GL_DYNAMIC_DRAW) : type(type), usage(usage), ID() {
                glGenBuffers(1, &ID);
            }

            void clear() {
                resize(0);
            }

            size_t add(T* data, size_t size = 0) {
                if (size == 0) size = sizeof(T);

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

            std::vector<T> get_data() {
                std::vector<T> data(size / sizeof(T));

                glBindBuffer(type, ID);
                glGetBufferSubData(type, 0, size, data.data());

                return data;
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
