#ifndef BUFFER_H
#define BUFFER_H

#include <GL/glew.h>

#include <Math/basics.h>
#include <Util/Vector.h>

namespace OddityEngine::Graphics {
    enum BufferIndex {
        OBJECT = 3,
        TRANSFORM,
        SHAPE,
        VERTEX,
        NORMAL,
        UV,
        MATERIAL,
        TEXTURE_TRANSFORM,
        LAYER,
        NETWORK,
        PHYSICS,
        TIME,
        RENDER,
    };

    template<typename T>
    class Buffer {
    protected:
        GLuint ID = 0;
        GLuint type;
        GLuint usage;
        GLsizei count = 0;
        Vector<GLsizei*> indices;

    public:
        explicit Buffer(const GLuint type = GL_SHADER_STORAGE_BUFFER, const GLuint usage = GL_DYNAMIC_DRAW) : type(type), usage(usage) {
            glGenBuffers(1, &ID);
        }

        ~Buffer() {
            for (auto i : indices) {
                delete(i);
            }

            glDeleteBuffers(1, &ID);
        }

        void resize(GLsizei count) {
            if(count == this->count) {
                return;
            }

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
                    delete indices[i];
                }
                indices.resize(count);
            }
            else {
                indices.resize(count);
                for (GLsizei i = this->count; i < count; i++) {
                    indices[i] = new GLsizei(i);
                }
            }

            this->count = count;
        }

        void multiply(GLsizei count, GLsizei multiples) {
            resize(count * multiples);

            glBindBuffer(type, ID);

            for (GLsizei i = 1; i < count; i++) {
                glCopyNamedBufferSubData(ID, ID, 0, i * count * sizeof(T), count * sizeof(T));
            }
        }

        void clear() {
            resize(0);
        }

        void shift(const GLsizei count, const GLsizei start, const GLsizei shifted_start) {
            glBindBuffer(type, ID);
            glCopyNamedBufferSubData(ID, ID, start * sizeof(T), shifted_start * sizeof(T), count * sizeof(T));
        }

        Vector<GLsizei*> insert(const GLsizei start, const GLsizei count, const T* data) {
            resize(this->count + count);
            shift(this->count - start, start, start + count);
            glBindBuffer(type, ID);
            glNamedBufferSubData(ID, start * sizeof(T), count * sizeof(T), data);

            return indices.slice(start, start + count);
        }

        GLsizei* insert(const GLsizei start, const T& data) {
            return insert(start, 1, &data)[0];
        }

        Vector<GLsizei*> insert_back(GLsizei count, const T* data) {
            auto offset = this->count;
            resize(this->count + count);

            glBindBuffer(type, ID);
            glNamedBufferSubData(ID, offset * sizeof(T), count * sizeof(T), data);

            return indices.slice(offset, offset + count);
        }

        GLsizei* insert_back(const T& data) {
            return insert_back(1, &data)[0];
        }

        void set(const GLsizei start, GLsizei count, const T* data) {
            if (start + count > this->count) {
                insert_back(start + count - this->count, data + this->count);
                count = this->count - start;
            }
            glBindBuffer(type, ID);
            glNamedBufferSubData(ID, start * sizeof(T), count * sizeof(T), data);
        }

        void set(const GLsizei index, T* data) {
            if (index >= this->count) {
                this->resize(index + 1);
            }
            set(index, 1, data);
        }

        T get(const GLsizei index) {
            T result;

            glBindBuffer(type, ID);
            glGetNamedBufferSubData(ID, index * sizeof(T), sizeof(T), &result);

            return result;
        }

        Vector<T> get() {
            Vector<T> result(count);

            glBindBuffer(type, ID);
            glGetNamedBufferSubData(ID, 0, count * sizeof(T), result.data());

            return result;
        }

        void remove(const GLsizei count, const GLsizei offset) {
            shift(this->count - (offset + count), offset + count, offset);
            resize(this->count - count);
        }

        [[nodiscard]] GLuint get_type() const {
            return type;
        }

        GLuint get_ID() const {
            return ID;
        }

        operator GLuint() const {
            return get_ID();
        }

        void bind_base(GLuint index) {
            glBindBufferBase(this->type, index, ID);
        }
    };
}

#endif //BUFFER_H
