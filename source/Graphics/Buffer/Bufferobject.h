#ifndef ODDITYENGINE_BUFFEROBJECT_H
#define ODDITYENGINE_BUFFEROBJECT_H

#include <cstdio>

#include <vector>

#include "Buffer.h"

namespace OddityEngine {
    namespace Graphics {
        template<typename T>
        class Bufferobject {
        protected:
            T object;
            size_t offset;
            size_t size;
            size_t index;

            Buffer<T>* buffer = nullptr;
        public:
            Bufferobject() : size(sizeof(T)) {}
            Bufferobject(Buffer<T>* buffer) : buffer(buffer), size(sizeof(T)) {
                if (buffer)
                    offset = buffer->add(this->size, &object);
                index = offset / this->size;
            }
            Bufferobject(T object, size_t size = 0) : object(object), size(size == 0 ? sizeof(T) : size) {}
            Bufferobject(Buffer<T>* buffer, T object, size_t size = 0) : Bufferobject(object, size) {
                if (buffer)
                    offset = buffer->add(this->size, &object);
                this->buffer = buffer;
                index = offset / this->size;
            }

            void set_buffer(Buffer<T>* buffer) {
                if (this->buffer)
                    this->buffer->remove(offset, size);
                if (buffer) {
                    offset = buffer->add(size, &object);
                    index = offset / size;
                }
                this->buffer = buffer;
            }

            void set(T object) {
                if (buffer)
                    buffer->set(offset, size, &object);
                this->object = object;
            }

            T get() {
                return object;
            }

            size_t get_index() {
                return index;
            }
        };

        template<typename T>
        static std::vector<Bufferobject<T>>* create_buffer_object_list(Buffer<T> * buffer, std::vector<T> list) {
            auto objects = new std::vector<Bufferobject<T>>;
            for (auto obj : list) {
                objects->emplace_back(buffer, obj);
            }

            return objects;
        }
    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_BUFFEROBJECT_H
