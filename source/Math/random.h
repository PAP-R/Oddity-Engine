#ifndef RANDOM_H
#define RANDOM_H

#include <type_traits>

#include <random>

namespace OddityEngine::Math {
    inline std::random_device r;
    inline std::default_random_engine random_engine(r());

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T> && std::is_floating_point_v<T>, bool> = true>
    T random(const T from, const T to) {
        std::uniform_real_distribution<T>  dist(from, to);
        return dist(random_engine);
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T> && !std::is_floating_point_v<T>, bool> = true>
    T random(const T from, const T to) {
        std::uniform_int_distribution<T>  dist(from, to);
        return dist(random_engine);
    }
}

#endif //RANDOM_H
