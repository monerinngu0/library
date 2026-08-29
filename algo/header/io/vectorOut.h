#pragma once
#pragma kbuild expand

#include "mone/header/notice.h"

#include <ostream>
#include <vector>

namespace mone { // vectorOut

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    for (const auto& x : v) {
        os << x << ' ';
    }
    return os;
}

} // namespace mone