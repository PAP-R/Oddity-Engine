#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <sstream>

#include <Math/basics.h>
#include "Vector.h"

namespace OddityEngine {
    template<typename T = float>
    class Matrix : public Vector<Vector<T>> {
    protected:
        bool use_default = false;
        T default_value = T();

    public:
        Matrix(const size_t rows = 0, const size_t columns = 0) {
            resize(rows, columns);
        }

        Matrix(const size_t rows, const size_t columns, const T& value) : Matrix(rows, columns) {
            this->use_default = true;
            this->default_value = value;
            for (size_t x = 0; x < this->rows(); x++) {
                for (size_t y = 0; y < (*this)[x].size(); y++) {
                    (*this)[x][y] = value;
                }
            }
        }

        /*** Shape ***/
        [[nodiscard]] constexpr size_t rows() const {
            return Vector<Vector<T>>::size();
        }

        [[nodiscard]] constexpr size_t columns() const {
            if (rows() == 0) {
                return 0;
            }
            return (*this)[0].size();
        }

        [[nodiscard]] constexpr size_t size() const {
            return rows() * columns();
        }

        Matrix& resize(const size_t& rows, const size_t& columns) {
            Vector<Vector<T>>::resize(rows);
            for (size_t r = 0; r < rows; r++) {
                (*this)[r].resize(columns);
            }

            return *this;
        }

        Matrix& resize_rows(const size_t rows) {
            return resize(rows, columns());
        }

        Matrix& resize_columns(const size_t columns) {
            return resize(rows(), columns);
        }

        Matrix transpose() {
            Matrix result(this->_columns, this->_rows);
            for (size_t x = 0; x < this->_rows; x++) {
                for (size_t y = 0; y < this->_columns; y++) {
                    result[y][x] = (*this)[x][y];
                }
            }
            return result;
        }

        template<typename S>
        bool size_check(const Matrix<S>& other) {
            return this->rows() == other.rows() && this->columns() == other.columns();
        }

        operator std::string () const {
            std::ostringstream str;
            str << (*this);
            return str.str();
        }

        template<typename S, typename Args, std::enable_if_t<std::is_same_v<T, std::function<S(Args)>>, bool> = true>
        Matrix<S> operator () (Matrix<Args>  args) {
            static_assert(!size_check(args));

            Matrix<S> result(this->rows(), this->columns());
            for (size_t x = 0; x < this->rows(); x++) {
                for (size_t y = 0; y < this->columns(); y++) {
                    result[x][y] = (*this)[x][y](args[x][y]);
                }
            }

            return result;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        T mean() {
            T result = 0;

            for (auto row : (*this)) {
                for (auto v : row) {
                    result += v;
                }
            }

            return result / static_cast<T>(size());
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Matrix& abs() {
            for (size_t x = 0; x < this->rows(); x++) {
                for (size_t y = 0; y < this->columns(); y++) {
                    (*this)[x][y] = std::abs((*this)[x][y]);
                }
            }

            return *this;
        }
    };

    template<typename T>
    static std::ostream& operator << (std::ostream& os, const Matrix<T>& matrix) {
        os << 2 << "," << matrix.rows() << "," << matrix.columns() << ",\n";
        for (size_t x = 0; x < matrix.rows(); x++) {
            for (size_t y = 0; y < matrix.columns(); y++) {
                os << matrix[x][y] << ",";
            }
            os << "\n";
        }

        return os;
    }
}

#endif //MATRIX_H
