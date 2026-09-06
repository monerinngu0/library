#pragma once
#pragma kbuild expand

#include <mone/detail/notice.h>

#include <cassert>

#include <algo/math/modint.h>

namespace mone {
    
// factorial

template<class T>
[[nodiscard]] T fact(int n) {
    assert(n >= 0);

    T result = 1;

    for (int i = 1; i <= n; ++i) {
        result *= T(i);
    }

    return result;
}

template<class Mint>
    requires is_static_modint_v<Mint>
[[nodiscard]] Mint inv_fact(int n) {
    return fact<Mint>(n).inv();
}

} // namespace mone