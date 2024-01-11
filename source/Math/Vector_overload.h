#ifndef VECTOR_OVERLOAD_H
#define VECTOR_OVERLOAD_H

#include "Vector.h"
#include "Matrix.h"

#include <Util/Debug.h>

namespace OddityEngine {
    template<typename T>
    Matrix<T> transpose(Vector<T> vector) {
        auto matrix = Matrix<T>();
        matrix[0] = vector;
        return matrix;
    }


    //*** Vector<T> o T ***//
    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator + (Vector<T> vector, const S& other) {
        return vector += other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator += (Vector<T>& vector, const S& other) {
        for (int i = 0; i < vector.size(); i++) {
            vector[i] += other;
        }

        return vector;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator - (Vector<T> vector, const S& other) {
        return vector -= other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator -= (Vector<T>& vector, const S& other) {
        for (int i = 0; i < vector.size(); i++) {
            vector[i] -= other;
        }

        return vector;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator * (Vector<T> vector, const S& other) {
        return vector *= other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator *= (Vector<T>& vector, const S& other) {
        for (size_t i = 0; i < vector.size(); i++) {
            vector[i] *= other;
        }

        return vector;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator / (Vector<T> vector, const S& other) {
        return vector /= other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator /= (Vector<T>& vector, const S& other) {
        for (size_t i = 0; i < vector.size(); i++) {
            vector[i] /= other;
        }

        return vector;
    }


    //*** Vector<T> o Vector<T> ***//
    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator + (Vector<T> vector, const Vector<S>& other) {
        return vector += other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator += (Vector<T>& vector, const Vector<S>& other) {
        for (int i = 0; i < vector.size(); i++) {
            vector[i] += other[i % other.size()];
        }

        return vector;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator - (Vector<T> vector, const Vector<S>& other) {
        return vector -= other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator -= (Vector<T>& vector, const Vector<S>& other) {
        for (int i = 0; i < vector.size(); i++) {
            vector[i] -= other[i % other.size()];
        }

        return vector;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator * (Vector<T> vector, const Vector<S>& other) {
        return vector *= other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator *= (Vector<T>& vector, const Vector<S>& other) {
        for (int i = 0; i < vector.size(); i++) {
            vector[i] *= other[i % other.size()];
        }

        return vector;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator / (Vector<T> vector, const Vector<S>& other) {
        return vector /= other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T>& operator /= (Vector<T>& vector, const Vector<S>& other) {
        for (int i = 0; i < vector.size(); i++) {
            vector[i] /= other[i % other.size()];
        }

        return vector;
    }


    //*** Vector<T> o Matrix<T> ***//
    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T> operator * (const Vector<T>& vector, const Matrix<T>& matrix) {
        if (matrix.rows() != 1) {
            Debug::error(fmt::format("Vector Matrix size mismatch: [{}][1] != [{}][{}]", vector.size(), matrix.rows(), matrix.columns()));
        }

        Matrix<T> result(vector.size(), matrix.columns());

        for (int x = 0; x < vector.size(); x++) {
            for (int y = 0; y < matrix.columns(); y++) {
                result[x][y] += vector[x] * matrix[0][y];
            }
        }

        return result;
    }


    //*** Matrix<T> o T ***//
    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator + (Matrix<T> matrix, const S& other) {
        return matrix += other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator += (Matrix<T>& matrix, const S& other) {
        for (int x = 0; x < matrix.rows(); x++) {
            for (int y = 0; y < matrix.columns(); y++) {
                matrix[x][y] += other;
            }
        }

        return matrix;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator - (Matrix<T> matrix, const S& other) {
        return matrix -= other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator -= (Matrix<T>& matrix, const S& other) {
        for (int x = 0; x < matrix.rows(); x++) {
            for (int y = 0; y < matrix.columns(); y++) {
                matrix[x][y] -= other;
            }
        }

        return matrix;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator * (Matrix<T> matrix, const S& other) {
        return matrix *= other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator *= (Matrix<T>& matrix, const S& other) {
        for (int x = 0; x < matrix.rows(); x++) {
            for (int y = 0; y < matrix.columns(); y++) {
                matrix[x][y] *= other;
            }
        }

        return matrix;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator / (Matrix<T> matrix, const S& other) {
        return matrix /= other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator /= (Matrix<T>& matrix, const S& other) {
        for (int x = 0; x < matrix.rows(); x++) {
            for (int y = 0; y < matrix.columns(); y++) {
                matrix[x][y] /= other;
            }
        }

        return matrix;
    }


    //*** Matrix<T> o Vector<T> ***//
    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator + (Matrix<T> matrix, const Vector<S>& other) {
        return matrix += other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator += (Matrix<T>& matrix, const Vector<S>& other) {
        for (int x = 0; x < matrix.rows(); x++) {
            for (int y = 0; y < matrix.columns(); y++) {
                matrix[x][y] += other[x % other.size()];
            }
        }

        return matrix;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator - (Matrix<T> matrix, const Vector<S>& other) {
        return matrix -= other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator -= (Matrix<T>& matrix, const Vector<S>& other) {
        for (int x = 0; x < matrix.rows(); x++) {
            for (int y = 0; y < matrix.columns(); y++) {
                matrix[x][y] -= other[x % other.size()];
            }
        }

        return matrix;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T> operator * (const Matrix<T>& matrix, const Vector<S>& other) {
        if (matrix.columns() != other.size()) {
            Debug::error(fmt::format("Matrix Vector size mismatch: [{}][{}] != [{}]", matrix.rows(), matrix.columns(), other.size()));
        }

        Vector<T> result(matrix.rows());

        for (int x = 0; x < matrix.rows(); x++) {
            for (int z = 0; z < matrix.columns(); z++) {
                result[x] += matrix[x][z] * other[z];
            }
        }

        return result;
    }


    //*** Matrix<T> o Matrix<T> ***//
    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator + (Matrix<T> matrix, const Matrix<S>& other) {
        return matrix += other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator += (Matrix<T>& matrix, const Matrix<S>& other) {
        for (int x = 0; x < matrix.rows(); x++) {
            for (int y = 0; y < matrix.columns(); y++) {
                matrix[x][y] += other[x % other.rows()][y % other.columns()];
            }
        }

        return matrix;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator - (Matrix<T> matrix, const Matrix<S>& other) {
        return matrix -= other;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T>& operator -= (Matrix<T>& matrix, const Matrix<S>& other) {
        for (int x = 0; x < matrix.rows(); x++) {
            for (int y = 0; y < matrix.columns(); y++) {
                matrix[x][y] -= other[x % other.rows()][y % other.columns()];
            }
        }

        return matrix;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T> operator * (Matrix<T>& matrix, const Matrix<S>& other) {
        if (matrix.columns() != other.rows()) {
            Debug::error(fmt::format("Matrix size mismatch: [{}][{}] != [{}][{}]", matrix.rows(), matrix.columns(), other.rows(), other.columns()));
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
}

#endif //VECTOR_OVERLOAD_H
