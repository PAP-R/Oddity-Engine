#include "Buffer.h"

#include <stdexcept>
#include <cstring>

namespace OddityEngine::Graphics::Buffer {
    Buffer::Buffer(GLuint type, GLuint usage) : type(type), usage(usage) {
        glGenBuffers(1, &this->ID);
    }

    size_t Buffer::add(size_t size, void *data) {
        glBindBuffer(this->type, this->ID);

        GLint buffersize = 0;
        glGetBufferParameteriv(this->type, GL_BUFFER_SIZE, &buffersize);

        size_t total_size = buffersize + size;

        void *backup = malloc(total_size);
        if (backup == nullptr) {
            throw std::runtime_error("Failed to allocate buffer backup");
        }

        glGetBufferSubData(this->type, 0, buffersize, backup);

        std::memcpy((char*)backup + buffersize, data, size);

        glBufferData(this->type, total_size, backup, this->usage);

        free(backup);

        return buffersize;
    }

    void Buffer::set(size_t offset, size_t size, void *data) {
        glNamedBufferSubData(this->ID, offset, size, data);
    }

    GLuint Buffer::get_type() {
        return this->type;
    }

    Buffer::operator unsigned int() const {
        return this->ID;
    }



}
