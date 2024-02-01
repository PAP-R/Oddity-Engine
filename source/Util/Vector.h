#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <sstream>

#include <type_traits>

#include <vector>
#include <functional>

#include <Util/Debug.h>
#include <fmt/core.h>

namespace OddityEngine {
    template<typename T = float>
    class Vector;

    template<typename>
    struct is_vector_helper : std::false_type::type {};

    template<typename V>
    struct is_vector_helper<Vector<V>> : public std::true_type::type {};

    template<typename V>
    struct is_vector : public is_vector_helper<V>::type {};

    template<typename V>
    inline constexpr bool is_vector_v = is_vector<V>::value;

    template<typename T, size_t dimensions>
    struct dimensional_matrix {
        typedef Vector<typename dimensional_matrix<T, dimensions - 1>::type> type;
    };

    template<typename T>
    struct dimensional_matrix<T, 0> {
        typedef T type;
    };

    template<typename T = float, size_t dimensions = 2>
    using Matrix = typename dimensional_matrix<T, dimensions>::type;

    template<typename T>
    class Vector : public std::vector<T> {
    protected:
        bool use_default = false;
        T default_value = T();
        void resize() {};
        friend Vector<Vector>;

        template<typename S>
        friend class Vector;

    public:
        explicit Vector(const size_t size = 0) : std::vector<T>(size) {}

        template<typename ... Args, typename S = T, std::enable_if_t<is_vector_v<S>, bool> = true>
        Vector(const size_t size, const size_t next, Args ... args) : Vector(size, S(next, args...)) {}

        Vector(const size_t size, const T& value) : std::vector<T>(size, value) {
            this->use_default = true;
            this->default_value = value;
        }

        template<typename S, std::enable_if_t<!std::is_same_v<T, S>, bool> = true>
        Vector(const Vector<S>& other)
            : std::vector<T>(other.size()) {
            for (size_t i = 0; i < other.size(); i++) {
                (*this)[i] = T(other[i]);
            }
        }

        Vector(std::initializer_list<T> list) : std::vector<T>(list) {}

        using std::vector<T>::vector;

        using std::vector<T>::size;

        template<typename S = T, std::enable_if_t<!is_vector_v<S>, bool> = true>
        size_t size(const size_t dimension) const {
            if (dimension == 0) {
                return this->size();
            }

            return 0;
        }

        template<typename S = T, std::enable_if_t<is_vector_v<S>, bool> = true>
        size_t size(const size_t dimension) const {
            if (dimension == 0) {
                return this->size();
            }

            if (this->size() > 0) {
                return (*this)[0].size(dimension - 1);
            }

            return 0;
        }

        template<typename S = T, std::enable_if_t<!is_vector_v<S>, bool> = true>
        static size_t dimensions() {
            return 1;
        }

        template<typename S = T, std::enable_if_t<is_vector_v<S>, bool> = true>
        static size_t dimensions() {
            return T::dimensions() + 1;
        }

        template<typename ... Sizes>//, std::enable_if_t<std::is_arithmetic_v<Sizes> && !std::is_floating_point_v<Sizes>>>
        Vector& resize(size_t size, Sizes ... sizes) {
            const size_t old_size = this->size();
            std::vector<T>::resize(size);

            if (use_default) {
                for (size_t i = old_size; i < size; i++) {
                    (*this)[i] = default_value;
                }
            }

            if constexpr (is_vector_v<T>) {
                for (auto &v : *this) {
                    v.resize(sizes...);
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

        template<typename S = T, std::enable_if_t<is_vector_v<S>, bool> = true>
        std::remove_all_extents_t<T> mean() {
            std::remove_all_extents_t<T> result = 0;
            for (auto &v : *this) {
                result += v.mean();
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

        template<typename S = T, std::enable_if_t<is_vector_v<S>, bool> = true>
        Vector& abs() {
            for (auto &v : *this) {
                v.abs();
            }

            return *this;
        }

        using std::vector<T>::insert;

        void insert(size_t idx, const T& value) {
            std::vector<T>::insert(this->begin() + idx, value);
        }

        template<typename ... Args>
        void emplace(size_t idx, const Args& ... args) {
            std::vector<T>::emplace(this->begin() + idx, args...);
        }

        template<typename S = T>
        [[nodiscard]] Vector<S> slice(size_t start, size_t end) const {
            return Vector<S>(this->begin() + start, this->begin() + end);
        }

        template<typename S = T>
        [[nodiscard]] Vector<S> slice(size_t start) const {
            return Vector<S>(this->begin() + start, this->end());
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        [[nodiscard]] Vector<Vector<S>> auto_slice() const {
            if ((*this)[0] == (*this)[1]) {
                return Vector<Vector<S>>({slice(0, (*this)[0] + 1)});
            }

            size_t start = 2;
            size_t count = (*this)[1];
            Vector<Vector<S>> result(count);

            for (size_t i = 0; i < count; i++) {
                size_t end = start + (*this)[start];
                if ((*this)[start] - 2 == (*this)[start + 1]) start += 2;
                result[i] = slice<S>(start, end);
                start += result[i].size();
            }

            return result;
        }

        template<typename S = T, std::enable_if_t<is_vector_v<S>, bool> = true>
        [[nodiscard]] Vector<Vector> auto_slice() const {
            Vector<Vector> result;
            result.reserve(this->size());

            for (auto v : (*this)) {
                result.push_back(v.auto_slice());
            }

            return result;
        }

        template<typename S = T, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
        [[nodiscard]] Vector sub_slice(size_t start) const {
            if ((*this)[start] < 1) {
                return slice(start, start + (*this)[start + 1] + 1);
            }

            size_t count = (*this)[start + 1];
            Vector result = {1, static_cast<float>(count)};
            for (size_t i = 0; i < count; i++) {
                auto temp = sub_slice(start + result.size());
                result.insert(result.end(), temp.begin(), temp.end());
            }

            return result;
        }

        template<typename S = T, std::enable_if_t<is_vector_v<S>, bool> = true>
        [[nodiscard]] auto strip() const {
            if constexpr (this->size() == 1) {
                return (*this)[0].strip();
            }

            return *this;
        }

        template<typename S = T, std::enable_if_t<std::is_same_v<S, std::string>, bool> = true>
        [[nodiscard]] Vector<float> to_long_float() const {
            Vector<float> result(this->size());
            for (size_t i = 0; i < this->size(); i++) {
                result[i] = std::stold((*this)[i]);
            }
            return result;
        }

        [[nodiscard]] Vector<> to_csv() const {
            Vector<> result;

            this->to_csv(&result);

            return result;
        }

        [[nodiscard]] std::string to_csv_string() const {
            std::stringstream result;
            auto csv = this->to_csv();

            for (size_t i = 0; i < csv.size(); i++) {
                if (i > 0) result << ',';
                result << csv[i];
            }

            return result.str();
        }

        void to_csv_more(Vector<>* result) const {
            size_t start = result->size();
            result->push_back(dimensions());
            for (size_t i = 0; i < (*result)[start]; i++) {
                result->push_back(this->size(i));
            }

            this->to_csv_less(result);
        }

        void to_csv(Vector<>* result) const {
            // if constexpr (std::is_class_v<T>) {
            //     result->push_back(0);
            // }
            // if (this->size() == 0) {
            //     result->push_back(1);
            //     result->push_back(0);
            //     return;
            // }

            result->push_back(this->csv_count());
            result->push_back(this->size());
            // result->push_back(100);

            if constexpr (is_vector_v<T>) {
                for (auto v : (*this)) {
                    v.to_csv(result);
                }
            }
            else if constexpr (!std::is_class_v<T>) {
                for (auto v : (*this)) {
                    result->push_back(v);
                }
            }
            else if constexpr (requires(const T& t){t.csv_count(result);}) {
                for (auto v : (*this)) {
                    v.to_csv(result);
                }
            }
            else if constexpr (requires(const T& t){t.to_csv();}) {
                for (auto v : (*this)) {
                    auto temp = v.to_csv();
                    for (auto t : temp) {
                        result->push_back(t);
                    }
                }
            }
            else {
                for (auto v : (*this)) {
                    result->push_back(v);
                }
            }

            // result->push_back(101);
        }

        [[nodiscard]] size_t csv_count() const {
            if constexpr (std::is_class_v<T> && requires(const T& t){t.csv_count();}) {
                size_t count = 0;

                for (auto v : (*this)) {
                    count += v.csv_count();
                }

                return count + 2;
            }

            return this->size() + 2;
        }

        operator std::string () const {
            std::ostringstream str;
            str << (*this);
            return str.str();
        }
    };

    template<typename T>
    static std::ostream& operator << (std::ostream& os, const Vector<T>& vector) {
        size_t indent = 0;
        for (size_t i = 0; i < vector.size(); i++) {
            if (is_vector_v<T> && i > 0) {
                os << "\n";
            }
            os << "[ " << vector[i] << " ]";
        }

        return os;
    }
}

#endif //VECTOR_H
