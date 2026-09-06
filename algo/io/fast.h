#pragma once
#pragma kbuild expand

#include <mone/detail/notice.h>

#include <iostream>

namespace mone::io {
    
// fast_io

struct FastIO {
    FastIO() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr);
    }
};

inline FastIO fast_io_initializer;

} // namespace mone::io