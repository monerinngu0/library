#pragma once
#pragma kbuild expand

#include "mone/notice.h"

#include <algorithm>
#include <cassert>
#include <limits>

#include "algo/header/math/modint.h"

namespace mone { // combination

template<class T>
[[nodiscard]] T perm(int n, int k) {
    if (n < 0 || k < 0 || k > n) {
        return T(0);
    }

    T result = 1;

    for (int i = 0; i < k; ++i) {
        result *= T(n - i);
    }

    return result;
}

template<class T>
[[nodiscard]] T comb(int n, int k) {
    if (n < 0 || k < 0 || k > n) {
        return T(0);
    }

    k = std::min(k, n - k);

    if constexpr (is_static_modint_v<T>) {
        T numerator = 1;
        T denominator = 1;

        for (int i = 1; i <= k; ++i) {
            numerator *= T(n - k + i);
            denominator *= T(i);
        }

        return numerator / denominator;
    } else {
        T result = 1;

        for (int i = 1; i <= k; ++i) {
            result *= T(n - k + i);
            result /= T(i);
        }

        return result;
    }
}

template<class T>
[[nodiscard]] T multichoose(int n, int k) {
    if (n < 0 || k < 0) {
        return T(0);
    }

    if (k == 0) {
        return T(1);
    }

    if (n == 0) {
        return T(0);
    }

    assert(n <= std::numeric_limits<int>::max() - k + 1);
    return comb<T>(n + k - 1, k);
}

} // namespace mone