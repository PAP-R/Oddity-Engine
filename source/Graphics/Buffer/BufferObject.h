#ifndef ODDITYENGINE_BUFFEROBJECT_H
#define ODDITYENGINE_BUFFEROBJECT_H

#include "Buffer.h"

namespace OddityEngine::Graphics {
    template<typename T>
    class BufferObject {
    protected:
        GLuint index;
        Buffer<T>* buffer;

    public:
        BufferObject(Buffer<T>* buffer, const T& object) : buffer{buffer} {
            index = buffer->insert_back(object);
        }
    };
}

#endif //ODDITYENGINE_BUFFEROBJECT_H
