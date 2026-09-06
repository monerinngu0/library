#pragma once

#include <numeric>
#include <type_traits>

namespace mone {

template <class T>
constexpr T gcd(T a) {
    return a;
}

template <class T, class... Ts>
constexpr T gcd(T a, T b, Ts... xs) {
    T g = std::gcd(a, b);
    if constexpr (sizeof...(xs) == 0) {
        return g;
    } else {
        return gcd(g, xs...);
    }
}

template <class T>
constexpr T lcm(T a) {
    return a;
}

template <class T, class... Ts>
constexpr T lcm(T a, T b, Ts... xs) {
    T l = std::lcm(a, b);
    if constexpr (sizeof...(xs) == 0) {
        return l;
    } else {
        return lcm(l, xs...);
    }
}

} // namespace mone