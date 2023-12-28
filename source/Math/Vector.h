#ifndef VECTOR_H
#define VECTOR_H

#include <cstring>

#include <Util/Debug.h>

#include "Matrix.h"

namespace OddityEngine::Math {
    template<typename T = float>
    class Vector : public Matrix<T> {
    protected:
        using Matrix<T>::data;
    public:
        Vector(const Matrix<T>& base) : Matrix<T>(base) {}

        explicit Vector(size_t initial_size = 1) : Matrix<T>(initial_size, 1) {}

        Vector(std::initializer_list<T> list) : Matrix<T>(list.size(), 1) {
            int i = 0;
            for (auto o : list) {
                (*this)[i++] = o;
            }
        }

        Vector copy() {
            return Matrix<T>::copy();
        }

        void resize(size_t size) {
            Matrix<T>::resize(size, 1);
        }

        T& operator [] (long long int index) {
            return Matrix<T>::operator[](index, 0);
        }

        Vector& operator = (const Vector& other) {
            this->resize(other.size());
            memcpy(this->data, other.data, this->_rows * this->_columns * sizeof(T));
            return *this;
        }
    };
}

#endif //VECTOR_H
