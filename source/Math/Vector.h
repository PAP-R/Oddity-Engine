#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <functional>

#include <Util/Debug.h>
#include <fmt/core.h>

namespace OddityEngine {
    template<typename T = double>
    class Vector : public std::vector<T> {
    protected:
        bool use_default = false;
        T default_value = T();

    public:
        explicit Vector(const size_t size = 0) : std::vector<T>(size) {}

        Vector(const size_t size, const T& value) : std::vector<T>(size) {
            this->use_default = true;
            this->default_value = value;
            for (size_t x = 0; x < this->size(); x++) {
                (*this)[x] = value;
            }
        }

        Vector(std::initializer_list<T> list) : std::vector<T>(list) {}

        Vector& resize(size_t size) {
            const size_t old_size = this->size();
            std::vector<T>::resize(size);

            if (use_default) {
                for (size_t i = old_size; i < size; i++) {
                    (*this)[i] = default_value;
                }
            }

            return *this;
        }

        template<typename S, std::enable_if_t<std::is_same_v<T, std::function<S(S)>>, bool> = true>
        Vector<S> operator () (Vector<S> other) {
            if (this->size() != other.size()) {
                Debug::error(fmt::format("Vector size mismatch: [{}] != [{}]", this->size(), other.size()));
            }

            Vector<S> result(this->size());

            for (size_t i = 0; i < this->size(); i++) {
                result[i] = (*this)[i](other[i]);
            }

            return result;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        T mean() {
            T result = 0;

            for (auto v : (*this)) {
                result += v;
            }

            return result / static_cast<T>(this->size());
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        Vector& abs() {
            for (size_t i = 0; i < this->size(); i++) {
                    (*this)[i] = std::abs((*this)[i]);
            }

            return *this;
        }

        void insert(size_t idx, const T& value) {
            std::vector<T>::insert(this->begin() + idx, value);
        }

        template<typename ... Args>
        void emplace(size_t idx, const Args& ... args) {
            std::vector<T>::emplace(this->begin() + idx, args...);
        }
    };

    template<typename T>
    static std::ostream& operator << (std::ostream& os, const Vector<T>& vector) {
        for (size_t i = 0; i < vector.size(); i++) {
            if (i > 0) os << "\n";
            os << "[" << vector[i] << "]";
        }

        return os;
    }
}

#endif //VECTOR_H
