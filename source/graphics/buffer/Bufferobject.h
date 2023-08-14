#ifndef ODDITYENGINE_BUFFEROBJECT_H
#define ODDITYENGINE_BUFFEROBJECT_H

#include "Buffer.h"

namespace OddityEngine::Graphics::Buffer {
    template<typename T>
    class Bufferobject {
    private:
        T object;
        size_t index;
        size_t size;

        Buffer* buffer;

    public:
        Bufferobject(Buffer* buffer, T object, size_t size = 0) : buffer(buffer), object(object) {
            this->size = size == 0 ? sizeof(object) : size;
            index = buffer->add(this->size, &object);
        }

        void set(T object) {
            buffer->set(index, this->size, &object);
            this->object = object;
        }

        T get() {
            return object;
        }
    };
}


#endif //ODDITYENGINE_BUFFEROBJECT_H
