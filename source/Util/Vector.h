#ifndef VECTOR_H
#define VECTOR_H

#include "Matrix.h"

namespace OddityEngine {
    template<typename T = double>
    class Vector : public Matrix<T>{
    protected:
        using Matrix<T>::resize;
        using Matrix<T>::resize_columns;
    public:
        Vector() = default;

        explicit Vector(size_t size) : Matrix<T>(size, 1) {
        }

        Vector(size_t size, const T& value, bool use_default = false) : Matrix<T>(size, 1, value, use_default) {}

        Vector(std::initializer_list<T> list) : Vector(list.size()) {
            int i = 0;
            for (auto o : list) {
                (*this)[i++] = o;
            }
        }

        Vector& resize(size_t size) {
            Matrix<T>::resize_rows(size);
            return *this;
        }

        T& operator [] (size_t index) const {
            return Matrix<T>::data[index];
        }

        void insert(size_t index, const T& value) {
            if (index > Matrix<T>::size()) {
                return;
            }
            this->resize(Matrix<T>::size() + 1);
            if (Matrix<T>::size() > 1) {
                for (int i = Matrix<T>::size() - 1; i > index; i--) {
                    (*this)[i] = (*this)[i - 1];
                }
            }

            (*this)[index] = value;
        }

        Vector(const Vector& other) : Matrix<T>(std::move(other)) {
        }

        Vector(Vector&& other) noexcept : Matrix<T>(std::move(other)) {
        }

        Vector& operator=(const Vector& other) {
            if (this == &other)
                return *this;
            Matrix<T>::operator =(other);
            Matrix<T>::resize(Matrix<T>::size(), 1);
            return *this;
        }

        Vector& operator=(Vector&& other) noexcept {
            if (this == &other)
                return *this;
            Matrix<T>::operator = (std::move(other));
            Matrix<T>::resize(Matrix<T>::size(), 1);
            return *this;
        }

        Vector(const Matrix<T>& other) : Matrix<T>{other} {
            Matrix<T>::resize(Matrix<T>::size(), 1);
        }

        Vector(Matrix<T>&& other) noexcept : Matrix<T>{std::move(other)} {
            Matrix<T>::resize(Matrix<T>::size(), 1);
        }

        Vector& operator=(const Matrix<T>& other) {
            if (this == &other)
                return *this;
            Matrix<T>::operator =(other);
            Matrix<T>::resize(Matrix<T>::size(), 1);
            return *this;
        }

        Vector& operator=(Matrix<T>&& other) noexcept {
            if (this == &other)
                return *this;
            Matrix<T>::operator =(std::move(other));
            Matrix<T>::resize(Matrix<T>::size(), 1);
            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Vector& operator *= (const Vector& other) {
            if (this->rows() != other.rows()) {
                Debug::error(fmt::format("Vector size mismatch : [{}] != [{}]", this->rows(), other.rows()));
            }

            for (int x = 0; x < Matrix<T>::rows(); x++) {
                for (int y = 0; y < Matrix<T>::columns(); y++) {
                    (*this)[x] *= other[x];
                }
            }

            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Vector& operator /= (const Vector& other) {
            if (this->rows() != other.rows()) {
                Debug::error(fmt::format("Vector size mismatch : [{}] != [{}]", this->rows(), other.rows()));
            }

            for (int x = 0; x < Matrix<T>::rows(); x++) {
                for (int y = 0; y < Matrix<T>::columns(); y++) {
                    (*this)[x] /= other[x];
                }
            }

            return *this;
        }
    };

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
    Vector<T> operator * (Vector<T> vector, const Vector<T>& other) {
        return vector *= other;
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
    Vector<T> operator * (Matrix<T> matrix, const Vector<T>& other) {
        try {
            return matrix * static_cast<Matrix<T>>(other);
        }
        catch (...) {
        }

        if (matrix.rows() != other.rows()) {
            Debug::error(fmt::format("Matrix Vector size mismatch : [{}] != [{}]\n{:s}\n!=\n{:s}", matrix.rows(), other.rows(), std::string(matrix), std::string(other)));
        }

        for (int x = 0; x < matrix.rows(); x++) {
            for (int y = 0; y < matrix.columns(); y++) {
                matrix[x][y] *= other[x];
            }
        }

        return matrix;
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
    Vector<T> operator / (Vector<T> vector, const Vector<T>& other) {
        return vector /= other;
    }
}

#endif //VECTOR_H
