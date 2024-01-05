#ifndef POINTER_H
#define POINTER_H

#include <memory>
#include <Util/Debug.h>
#include <Math/basics.h>

namespace OddityEngine {
    static size_t counter = 0;
    template<typename T>
    class Pointer {
    protected:
        T* _data = nullptr;
        size_t* _size = nullptr;
        size_t* _count = nullptr;
        size_t* pointer_count = nullptr;

    public:
        template<typename S, std::enable_if_t<std::is_base_of_v<T, S>, bool> = true>
        friend class Pointer<T>;

        Pointer() {
            _size = static_cast<size_t*>(malloc(sizeof(size_t)));
            *_size = sizeof(T);
            _count = static_cast<size_t*>(malloc(sizeof(size_t)));
            *_count = 0;
            pointer_count = static_cast<size_t*>(malloc(sizeof(size_t)));
            *pointer_count = 1;
        }

        Pointer(const T& data, const size_t size = sizeof(T), const size_t count = 1) : Pointer() {
            resize(count, size);
            set(data);
        }

        template<typename S = T, std::enable_if_t<std::is_base_of_v<T, S> && !std::is_same_v<T, S>, bool> = true>
        Pointer(const S& data, const size_t size = sizeof(S), const size_t count = 1) : Pointer() {
            resize(count, size);
            set(data);
        }

        Pointer(const void* data, const size_t size = sizeof(T), const size_t count = 1) : Pointer() {
            resize(count, size);
            set(data);
        }

        template<typename S = T, std::enable_if_t<std::is_base_of_v<T, S> && !std::is_same_v<T, S>, bool> = true>
        Pointer(const S* data, const size_t size = sizeof(S), const size_t count = 1) : Pointer() {
            resize(count, size);
            set(data);
        }

        template<typename S, std::enable_if_t<std::is_base_of_v<T, S>, bool> = true>
        Pointer(const Pointer<S>& other)
            : _data{other._data},
              _size{other._size},
              _count{other._count},
              pointer_count{other.pointer_count} {
            *pointer_count += 1;
        }

        // template<typename S, std::enable_if_t<std::is_base_of_v<T, S>, bool> = true>
        // Pointer(const Pointer<S>&& other)
        //     : _data{other._data},
        //       _size{other._size},
        //       _count{other._count},
        //       pointer_count{other.pointer_count} {
        //     *pointer_count += 1;
        // }

        template<typename S, std::enable_if_t<std::is_base_of_v<T, S>, bool> = true>
        Pointer& operator = (const Pointer<S>& other) {
            this->~Pointer();
            if (*this == other)
                return *this;
            _data = other._data;
            _size = other._size;
            _count = other._count;
            pointer_count = other.pointer_count;
            *pointer_count += 1;
            return *this;
        }

        // template<typename S, std::enable_if_t<std::is_base_of_v<T, S>, bool> = true>
        // Pointer& operator = (const Pointer<S>&& other) {
        //     this->~Pointer();
        //     if (*this == other)
        //         return *this;
        //     _data = other._data;
        //     _size = other._size;
        //     _count = other._count;
        //     pointer_count = other.pointer_count;
        //     *pointer_count += 1;
        //     return *this;
        // }

        ~Pointer() {
            counter += 1;
            if (pointer_count != nullptr) {
                *pointer_count -= 1;
                if (*pointer_count == 0) {
                    if (_data != nullptr) {
                        free(_data);
                        _data = nullptr;
                    }
                    free(_size);
                    _size = nullptr;
                    free(_count);
                    _count = nullptr;
                    free(pointer_count);
                    pointer_count = nullptr;
                }
            }
        }

        Pointer copy() const {
            counter += 1;
            if (assigned())
                return {*this->_data, size(), count()};
            return {};
        }

        T* data() {
            return _data;
        }

        size_t size() const {
            return *_size;
        }

        size_t count() const {
            return *_count;
        }

        void resize(size_t count, size_t size = 0) {
            if (size == 0) size = *_size;

            if (*this->_size == size && *this->_count == count) return;

            auto temp = this->_data;

            this->_data = static_cast<T*>(calloc(count, size));
            if (this->_data == nullptr) {
                Debug::error("Failed to allocate memory for Pointer");
            }

            if (temp != nullptr) {
                memcpy(this->_data, temp, Math::min(*this->_size * *this->_count, size * count));
                free(temp);
            }

            memset(this->_data + *_count, 0, count * size - *this->_size * *this->_count);

            *this->_size = size;
            *this->_count = count;
        }

        void set(const T& data) {
            if (!assigned()) return;
            *(_data) = data;
        }

        void set(const void* data, size_t size = 0, size_t offset = 0) {
            if (!assigned() || data == nullptr) return;
            if (size == 0) size = *this->_size * *this->_count;
            memcpy(_data, data, size);
        }

        T& operator * () const {
            return *_data;
        }

        T* operator -> () const {
            return _data;
        }

        T& operator [] (size_t index) const {
            if (index >= *_count) {
                Debug::error("Index out of range");
            }
            return _data[index];
        }

        bool assigned() const {
            return (_data != nullptr);
        }

        template<typename S = T, std::enable_if_t<std::is_same_v<T, S> || std::is_base_of_v<T, S>, bool> = true>
        bool operator == (const Pointer<S>& other) const {
            if (_data == other._data && _size == other._size && _count == other._count) {
                return true;
            }

            return false;
        }

        template<typename S = T, std::enable_if_t<std::is_same_v<T, S> || std::is_base_of_v<T, S>, bool> = true>
        bool operator != (const Pointer<S>& other) const {
            return !((*this) == other);
        }
    };
}

#endif //POINTER_H
