#ifndef ODDITYENGINE_BUFFEROBJECT_H
#define ODDITYENGINE_BUFFEROBJECT_H

#include <cstdio>

#include <vector>

#include <Graphics/OpenGL/Buffer/Buffer.h>

namespace OddityEngine {
    namespace Graphics {
        template<typename T>
        class Bufferobject {
        protected:
            T object;
            size_t offset;
            size_t size;
            size_t index;

            Buffer* buffer;
        public:
            Bufferobject(Buffer* buffer, T object, size_t size = 0) : buffer(buffer), object(object) {
                this->size = size == 0 ? sizeof(object) : size;
                offset = buffer->add(this->size, &object);
                index = offset / this->size;
            }

            void set(T object) {
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
        static std::vector<Bufferobject<T>> create_buffer_object_list(Buffer * buffer, std::vector<T> list) {
            std::vector<Bufferobject<T>> objects;
            for (auto obj : list) {
                objects.emplace_back(buffer, obj);
            }

            return objects;
        }
    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_BUFFEROBJECT_H
