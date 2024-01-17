#ifndef VECTOR_OVERLOAD_H
#define VECTOR_OVERLOAD_H

#include "Vector.h"

#include <Util/Debug.h>

namespace OddityEngine {
    template<typename T>
    Matrix<T> transpose(Vector<T> vector) {
        auto matrix = Matrix<T>(1);
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
        if (matrix.size(0) != 1) {
            Debug::error(fmt::format("Vector Matrix size mismatch: [{}][1] != [{}][{}]", vector.size(), matrix.size(0), matrix.size(1)));
        }

        Matrix<T> result(vector.size(), matrix.size(1));

        for (int x = 0; x < vector.size(); x++) {
            for (int y = 0; y < matrix.size(1); y++) {
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
        for (int x = 0; x < matrix.size(0); x++) {
            for (int y = 0; y < matrix.size(1); y++) {
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
        for (int x = 0; x < matrix.size(0); x++) {
            for (int y = 0; y < matrix.size(1); y++) {
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
        for (int x = 0; x < matrix.size(0); x++) {
            for (int y = 0; y < matrix.size(1); y++) {
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
        for (int x = 0; x < matrix.size(0); x++) {
            for (int y = 0; y < matrix.size(1); y++) {
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
        for (int x = 0; x < matrix.size(0); x++) {
            for (int y = 0; y < matrix.size(1); y++) {
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
        for (int x = 0; x < matrix.size(0); x++) {
            for (int y = 0; y < matrix.size(1); y++) {
                matrix[x][y] -= other[x % other.size()];
            }
        }

        return matrix;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Vector<T> operator * (const Matrix<T>& matrix, const Vector<S>& other) {
        if (matrix.size(1) != other.size()) {
            Debug::error(fmt::format("Matrix Vector size mismatch: [{}][{}] != [{}]", matrix.size(0), matrix.size(1), other.size()));
        }

        Vector<T> result(matrix.size(0));

        for (int x = 0; x < matrix.size(0); x++) {
            for (int z = 0; z < matrix.size(1); z++) {
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
        for (int x = 0; x < matrix.size(0); x++) {
            for (int y = 0; y < matrix.size(1); y++) {
                matrix[x][y] += other[x % other.size(0)][y % other.size(1)];
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
        for (int x = 0; x < matrix.size(0); x++) {
            for (int y = 0; y < matrix.size(1); y++) {
                matrix[x][y] -= other[x % other.size(0)][y % other.size(1)];
            }
        }

        return matrix;
    }

    template<typename T, typename S, std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<S>, bool> = true>
    Matrix<T> operator * (Matrix<T>& matrix, const Matrix<S>& other) {
        if (matrix.size(1) != other.size(0)) {
            Debug::error(fmt::format("Matrix size mismatch: [{}][{}] != [{}][{}]", matrix.size(0), matrix.size(1), other.size(0), other.size(1)));
        }

        Matrix<T> result(matrix.size(0), other.size(1));

        for (int x = 0; x < matrix.size(0); x++) {
            for (int y = 0; y < other.size(1); y++) {
                for (int z = 0; z < matrix.size(1); z++) {
                    result[x][y] += matrix[x][z] * other[z][y];
                }
            }
        }

        return result;
    }
}

#endif //VECTOR_OVERLOAD_H
