#ifndef BASICS_H
#define BASICS_H

#include <type_traits>

namespace OddityEngine::Math {
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
    T min(T first, T second) {
        return first < second ? first : second;
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
    T max(T first, T second) {
        return first > second ? first : second;
    }
}

#endif //BASICS_H
