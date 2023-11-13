#include "Buffer.h"

namespace OddityEngine {
    namespace Graphics {
        void Buffer::resize(size_t size) {
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

        Buffer::Buffer(GLuint type, GLuint usage) : type(type), usage(usage), ID() {
            glGenBuffers(1, &ID);
        }

        size_t Buffer::add(size_t size, void *data) {
            glBindBuffer(type, ID);
            GLint buffersize = this->size;

            size_t total_size = buffersize + size;

            resize(total_size);

            glBindBuffer(type, ID);
            glNamedBufferSubData(ID, buffersize, size, data);

            return buffersize;
        }

        void Buffer::set(size_t offset, size_t size, void *data) {
            glBindBuffer(type, ID);
            glNamedBufferSubData(ID, offset, size, data);
        }

        void Buffer::remove(size_t offset, size_t size) {
            if (offset + size >= this->size) {
                resize(offset);
            }
        }

        GLuint Buffer::get_type() {
            return type;
        }

        Buffer::operator GLuint() const {
            return ID;
        }
    } // OddityEngine
} // Graphics