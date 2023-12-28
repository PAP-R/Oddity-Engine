#ifndef MATRIX_H
#define MATRIX_H

#include <cstring>

#include <Util/Debug.h>

namespace OddityEngine::Math {
    template<typename T = float>
    class Matrix {
    protected:
        size_t _rows = 0;
        size_t _columns = 0;
        T* data = nullptr;

    public:
        [[nodiscard]] size_t rows() const {
            return _rows;
        }

        [[nodiscard]] size_t columns() const {
            return _columns;
        }

        [[nodiscard]] size_t size() const {
            return _rows * _columns;
        }

        explicit Matrix(const size_t rows = 1, const size_t columns = 1) {
            resize(rows, columns);
        }

        ~Matrix() {
            std::free(data);
        }

        void resize(size_t rows = 0, size_t columns = 0) {
            rows = rows == 0 ? this->_rows : rows;
            columns = columns == 0 ? this->_columns : columns;

            auto size_old = this->_rows * this->_columns;
            auto size_new = rows * columns;

            if (size_old != size_new) {
                data = static_cast<T*>(realloc(data, size_new * sizeof(T)));
                if (data == nullptr) {
                    Debug::error("Failed to allocate Memory");
                }
            }

            this->_rows = rows;
            this->_columns = columns;
        }

        Matrix copy() const {
            Matrix result(this->_rows, this->_columns);
            memcpy(result.data, this->data, this->_rows * this->_columns * sizeof(T));
            return result;
        }

        void copy_to(T* dest) const {
            memcpy(dest, data, this->size() * sizeof(T));
        }

        T& operator [] (const int x, const int y) const {
            return data[(x % _rows) * _columns + (y % _columns)];
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix operator + (const S& other) const {
            auto result = copy();
            result += other;

            return result;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& operator += (const S& other) const {
            for (int x = 0; x < _rows; x++) {
                for (int y = 0; y < _columns; y++) {
                    (*this)[x, y] += other;
                }
            }

            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix operator - (const S& other) const {
            auto result = copy();
            result -= other;

            return result;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& operator -= (const S& other) const {
            for (int x = 0; x < _rows; x++) {
                for (int y = 0; y < _columns; y++) {
                    (*this)[x, y] -= other;
                }
            }

            return this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix operator * (const S& other) const {
            auto result = copy();
            result *= other;

            return result;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& operator *= (const S& other) {
            for (int x = 0; x < _rows; x++) {
                for (int y = 0; y < _columns; y++) {
                    (*this)[x, y] *= other;
                }
            }

            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix operator / (const S& other) const {
            auto result = copy();
            result /= other;

            return result;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& operator /= (const S& other) {
            for (int x = 0; x < _rows; x++) {
                for (int y = 0; y < _columns; y++) {
                    (*this)[x, y] /= other;
                }
            }

            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix operator + (const Matrix<S>& other) const {
            auto result = copy();
            return result += other;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& operator += (const Matrix<S>& other) {
            if (this->rows() != other.rows() && this->columns() != other.columns()) {
                Debug::error("Mismatched Matrix sizes");
            }

            for (int x = 0; x < _rows; x++) {
                for (int y = 0; y < _columns; y++) {
                    (*this)[x, y] += other[x, y];
                }
            }

            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix operator - (const Matrix<S>& other) const {
            auto result = this->copy();
            return result -= other;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& operator -= (const Matrix<S>& other) {
            if (this->rows() != other.rows() && this->columns() != other.columns()) {
                Debug::error("Mismatched Matrix sizes");
            }

            for (int x = 0; x < _rows; x++) {
                for (int y = 0; y < _columns; y++) {
                    (*this)[x, y] -= other[x, y];
                }
            }

            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix operator * (const Matrix<S>& other) const {
            if (this->columns() != other.rows()) {
                Debug::error("Mismatched Matrix sizes");
            }

            Matrix result(this->rows(), other.columns());

            for (int x = 0; x < this->rows(); x++) {
                for (int y = 0; y < other.columns(); y++) {
                    for (int z = 0; z < this->columns(); z++) {
                        result[x, y] += (*this)[x, z] * other[z, y];
                    }
                }
            }

            return result;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& operator *= (const Matrix<S>& other) {
            *this = *this * other;
            return *this;
        }

        Matrix& operator = (const Matrix& other) {
            if (this == &other) {
                return *this;
            }

            this->resize(other.rows(), other.columns());
            other.copy_to(this->data);

            return *this;
        }
    };
}

#endif //MATRIX_H
