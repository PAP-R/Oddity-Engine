#ifndef BUFFER_H
#define BUFFER_H

#include <GL/glew.h>

#include <Math/basics.h>
#include <Util/Vector.h>

namespace OddityEngine::Graphics {
    template<typename T>
    class Buffer {
    protected:
        GLuint ID = 0;
        GLuint type;
        GLuint usage;
        GLsizei count = 0;
        Vector<GLsizei**> indices;

    public:
        explicit Buffer(const GLuint type = GL_SHADER_STORAGE_BUFFER, const GLuint usage = GL_DYNAMIC_DRAW) : type(type), usage(usage) {
            glGenBuffers(1, &ID);
        }

        void resize(GLsizei count) {
            GLuint new_buffer;
            glGenBuffers(1, &new_buffer);

            glBindBuffer(GL_COPY_READ_BUFFER, ID);
            glBindBuffer(GL_COPY_WRITE_BUFFER, new_buffer);

            glNamedBufferData(new_buffer, count * sizeof(T), nullptr, usage);

            glCopyNamedBufferSubData(ID, new_buffer, 0, 0, Math::min(count, this->count) * sizeof(T));

            glDeleteBuffers(1, &ID);

            ID = new_buffer;
            if (count < this->count) {
                for (GLsizei i = count; i < this->count; i++) {
                    delete *(indices[i]);
                    *(indices[i]) = nullptr;
                }
                indices.resize(count);
            }
            else {
                indices.resize(count);
                for (GLsizei i = this->count; i < count; i++) {
                    indices[i] = new GLsizei*(new GLsizei(i));
                }
            }

            this->count = count;
        }

        void clear() {
            resize(0);
        }

        void shift(const GLsizei count, const GLsizei start, const GLsizei shifted_start) {
            glBindBuffer(type, ID);
            glCopyNamedBufferSubData(ID, ID, start * sizeof(T), shifted_start * sizeof(T), count * sizeof(T));
        }

        Vector<GLsizei**> insert(const GLsizei start, const GLsizei count, const T* data) {
            resize(this->count + count);
            shift(this->count - start, start, start + count);
            glBindBuffer(type, ID);
            glNamedBufferSubData(ID, start * sizeof(T), count * sizeof(T), data);

            return indices.slice(start, start + count);
        }

        Vector<GLsizei**> insert_back(GLsizei count, const T* data) {
            auto offset = this->count;
            resize(this->count + count);

            glBindBuffer(type, ID);
            glNamedBufferSubData(ID, offset * sizeof(T), count * sizeof(T), data);

            return indices.slice(offset, offset + count);
        }

        void set(const GLsizei count, const GLsizei offset, const T* data) {
            glBindBuffer(type, ID);
            glNamedBufferSubData(ID, offset * sizeof(T), count * sizeof(T), data);
        }

        void remove(const GLsizei count, const GLsizei offset) {
            shift(this->count - (offset + count), offset + count, offset);
            resize(this->count - count);
        }

        [[nodiscard]] GLuint get_type() const {
            return type;
        }

        operator GLuint() const {
            return ID;
        }
    };
}

#endif //BUFFER_H
