#ifndef ODDITYENGINE_BUFFEROBJECT_H
#define ODDITYENGINE_BUFFEROBJECT_H

#include "Buffer.h"

namespace OddityEngine::Graphics::Buffer {
    template<typename T>
    class Bufferobject {
    private:
        T object;
        size_t offset;
        size_t index;
        size_t size;

        Buffer* buffer;

    public:
        Bufferobject(Buffer* buffer, T object, size_t size = 0) : buffer(buffer), object(object) {
            this->size = size == 0 ? sizeof(object) : size;
            this->offset = buffer->add(this->size, &object);
            this->index = this->offset / this->size;
            this->object = object;
        }

        void set(T object) {
            buffer->set(offset, this->size, &object);
            this->object = object;
        }

        T get() {
            return object;
        }

        size_t get_index() {
            return this->index;
        }
    };

    template<typename T>
    std::vector<Bufferobject<T>> create_object_list(Buffer* buffer, std::vector<T> list) {
        std::vector<Bufferobject<T>> objects;
        for (auto obj : list) {
            objects.emplace_back(buffer, obj);
        }

        return objects;
    }
}


#endif //ODDITYENGINE_BUFFEROBJECT_H
