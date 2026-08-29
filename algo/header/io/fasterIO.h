#pragma once
#pragma kbuild expand

#include "mone/notice.h"

#include <iostream>

namespace mone { // fastIO

struct FastIO {
    FastIO() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr);
    }
};

inline FastIO fast_io_initializer;

} // namespace mone