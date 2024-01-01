#ifndef MATRIX_H
#define MATRIX_H

#include <cstring>
#include <sstream>
#include <fmt/core.h>

#include <Util/Debug.h>

#include "basics.h"

namespace OddityEngine {
    template<typename T = double>
    class Matrix {
    protected:
        size_t _rows = 0;
        size_t _columns = 1;
        size_t _size = 0;
        bool use_default = false;
        T default_value;
        T* data = nullptr;

    public:
        Matrix(const size_t rows, const size_t columns) {
            resize(rows, columns);
        }

        Matrix() : Matrix(1, 1) {
        }

        Matrix(const size_t rows, const size_t columns, T value, bool use_default = false) : Matrix(rows, columns) {
            this->use_default = use_default;
            this->default_value = value;
            const size_t bigger = rows > columns ? rows : columns;
            auto cr = static_cast<long double>(columns) / rows;
            cr = cr < 1 ? cr : 1;
            auto rc = static_cast<long double>(rows) / columns;
            rc = rc < 1 ? rc : 1;

            if (use_default) {
                for (int i = 0; i < rows * columns; i++) {
                    data[i] = value;
                }
            }
            else {
                for (int i = 0; i < bigger; i++) {
                    data[static_cast<size_t>(i * rc) * this->_columns + static_cast<size_t>(i * cr)] = value;
                }
            }
        }

        ~Matrix() {
          this->clear(data);
            if (data != nullptr) free(data);
        }

        void clear(T* data, size_t count = 0) {
            if (count == 0) count = this->size();
            for (int i = 0; i < count; ++i) {
                data[i].~T();
            }
        }

        /*** Shape ***/
        [[nodiscard]] size_t rows() const {
            return _rows;
        }

        [[nodiscard]] size_t columns() const {
            return _columns;
        }

        [[nodiscard]] size_t size() const {
            return _size;
        }

        Matrix& resize(const size_t& rows, const size_t& columns) {
            if (rows == 0 || columns == 0) return *this;
            size_t new_size = rows * columns;
            T* temp = this->data;
            if (this->size() != new_size) {
                this->data = static_cast<T*>(calloc(new_size, sizeof(T)));
                if (this->data == nullptr) {
                    Debug::error("Failed to allocate Matrix Memomry");
                }
            }

            if (this->use_default) {
                for (int x = 0; x < rows; x++) {
                    for (int y = 0; y < columns; y++) {
                        data[x * columns + y] = default_value;
                    }
                }
            }

            if (temp != nullptr) {
                auto temp_rows = Math::min(rows, this->rows());
                auto temp_columns = Math::min(columns, this->columns());

                for (int x = 0; x < temp_rows; x++) {
                    memcpy(this->data + x * columns, temp + x * this->columns(), temp_columns * sizeof(T));
                }

                if (temp != this->data) free(temp);
            }

            this->_rows = rows;
            this->_columns = columns;
            this->_size = rows * columns;

            return *this;
        }

        Matrix& resize_rows(const size_t rows) {
            return resize(rows, _columns);
        }

        Matrix& resize_columns(const size_t columns) {
            return resize(_rows, columns);
        }

        Matrix transpose() {
            Matrix result(this->_columns, this->_rows);
            for (int x = 0; x < this->_rows; x++) {
                for (int y = 0; y < this->_columns; y++) {
                    result[y][x] = (*this)[x][y];
                }
            }
            return result;
        }

        /*** Data access ***/


        T get(const int x, const int y) const {
            return data[(x % _rows) * _columns + (y % _columns)];
        }

        T* operator [] (int index) const {
            if (this->size() > 0) {
                return data + ((index % _rows) * _columns);
            }
            return nullptr;
        }

        bool operator == (const Matrix& other) const {
            if (this->_rows != other._rows || this->_columns != other._columns) {
                return false;
            }

            for (int x = 0; x < this->_rows; x++) {
                for (int y = 0; y < this->_columns; y++) {
                    if ((*this)[x][y] != other[x][y]) return false;
                }
            }

            return true;
        }

        bool operator != (const Matrix& other) {
            return !(*this == other);
        }

        template<typename S>
        bool size_check(const Matrix<S>& other) {
            return this->_rows == other.rows() && this->_columns == other.columns();
        }

        Matrix(const Matrix& other) {
            this->use_default = other.use_default;
            this->default_value = other.default_value;
            resize(other._rows, other._columns);
            memcpy(data, other.data, other.size() * sizeof(T));
        }

        Matrix(Matrix&& other) noexcept {
            this->use_default = other.use_default;
            this->default_value = other.default_value;
            resize(other._rows, other._columns);
            memcpy(data, other.data, other.size() * sizeof(T));
        }

        Matrix& operator=(const Matrix& other) {
            if (this == &other)
                return *this;
            this->use_default = other.use_default;
            this->default_value = other.default_value;
            resize(other._rows, other._columns);
            memcpy(data, other.data, other.size() * sizeof(T));
            return *this;
        }

        Matrix& operator=(Matrix&& other) noexcept {
            if (this == &other)
                return *this;
            this->use_default = other.use_default;
            this->default_value = other.default_value;
            resize(other._rows, other._columns);
            memcpy(data, other.data, other.size() * sizeof(T));
            return *this;
        }

        template<typename S>
        Matrix<S> operator () (Matrix<S>  args) {
            if (!size_check(args)) {
                Debug::error("Matrix size mismatch");
            }

            Matrix<S> result(this->rows(), this->columns());
            for (int x = 0; x < this->rows(); x++) {
                for (int y = 0; y < this->columns(); y++) {
                    result[x][y] = (*this)[x][y](args[x][y]);
                }
            }

            return result;
        }

        operator std::string () const {
            std::ostringstream str;
            str << (*this);
            return str.str();
        }

        static Matrix copy(const Matrix& other) {
            Matrix result;
            result.resize(other.rows(), other.columns());
            memcpy(result.data, other.data, other.size() * sizeof(T));
            return result;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& operator += (const Matrix& other) {
            for (int x = 0; x < _rows; x++) {
                for (int y = 0; y < _columns; y++) {
                    (*this)[x][y] += other[x % other._rows][y % other._columns];
                }
            }

            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& operator -= (const Matrix& other) {
            for (int x = 0; x < _rows; x++) {
                for (int y = 0; y < _columns; y++) {
                    (*this)[x][y] -= other[x % other._rows][y % other._columns];
                }
            }

            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& operator += (const S& other) {
            for (int x = 0; x < _rows; x++) {
                for (int y = 0; y < _columns; y++) {
                    (*this)[x][y] += other;
                }
            }

            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& operator -= (const S& other) {
            for (int x = 0; x < _rows; x++) {
                for (int y = 0; y < _columns; y++) {
                    (*this)[x][y] -= other;
                }
            }

            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& operator *= (const S& other) {
            for (int x = 0; x < _rows; x++) {
                for (int y = 0; y < _columns; y++) {
                    (*this)[x][y] *= other;
                }
            }

            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& operator /= (const S& other) {
            for (int x = 0; x < _rows; x++) {
                for (int y = 0; y < _columns; y++) {
                    (*this)[x][y] /= other;
                }
            }

            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        T mean() {
            T result = 0;

            for (auto v : (*this)) {
                result += v;
            }

            return result / static_cast<T>(size());
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& abs() {
            // for (auto &v : *this) {
            //     v = std::abs(v);
            // }

            for (int x = 0; x < this->rows(); x++) {
                for (int y = 0; y < this->columns(); y++) {
                    (*this)[x][y] = std::abs((*this)[x][y]);
                }
            }

            return *this;
        }

        /*** Iteration ***/
        class Iterator {
        protected:
            size_t index;
            T* data;

        public:
            Iterator(size_t index, T* data) : index(index), data(data) {}

            bool operator == (const Iterator& other) const {
                return index == other.index && data == other.data;
            }

            bool operator != (const Iterator& other) const {
                return index != other.index || data != other.data;
            }

            T& operator * () const {
                return *(data + index);
            }

            T* operator -> () const {
                return data + index;
            }

            Iterator& operator ++ () {
                index += 1;
                return *this;
            }

            Iterator operator ++ (int) {
                auto temp = *this;
                ++(*this);
                return temp;
            }

            Iterator& operator -- () {
                index -= 1;
                return *this;
            }

            Iterator operator -- (int) {
                auto temp = *this;
                --(*this);
                return temp;
            }
        };

        Iterator begin() {
            return Iterator(0, data);
        }

        Iterator end() {
            return Iterator(this->size(), data);
        }
    };

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
    Matrix<T>& operator + (Matrix<T> matrix, const Matrix<T>& other) {
        return matrix += other;
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
    Matrix<T>& operator - (Matrix<T> matrix, const Matrix<T>& other) {
        return matrix -= other;
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
    Matrix<T> operator * (const Matrix<T>& matrix, const Matrix<T>& other) {
        if (matrix.columns() != other.rows()) {
                Debug::error(fmt::format("Matrix size mismatch : [{}] != [{}]\n{:s}\n!=\n{:s}", matrix.columns(), other.rows(), std::string(matrix), std::string(other)));
        }

        Matrix<T> result(matrix.rows(), other.columns());

        for (int x = 0; x < matrix.rows(); x++) {
            for (int y = 0; y < other.columns(); y++) {
                for (int z = 0; z < matrix.columns(); z++) {
                    result[x][y] += matrix[x][z] * other[z][y];
                }
            }
        }

        return result;
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true,
    typename S, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator + (Matrix<T> matrix, const S& other) {
        return matrix += other;
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true,
    typename S, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator - (Matrix<T> matrix, const S& other) {
        return matrix -= other;
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true,
    typename S, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator * (Matrix<T> matrix, const S& other) {
        return matrix *= other;
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true,
    typename S, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator / (Matrix<T> matrix, const S& other) {
        return matrix /= other;
    }



    template<typename T>
    static std::ostream& operator << (std::ostream& os, const Matrix<T>& matrix) {
        for (int x = 0; x < matrix.rows(); x++) {
            if (x > 0) os << "\n";
            for (int y = 0; y < matrix.columns(); y++) {
                os << "[" << (matrix.get(x, y)) << "]";
            }
        }

        return os;
    }
}

#endif //MATRIX_H
