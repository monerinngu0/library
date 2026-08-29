#pragma once
#pragma kbuild expand

#include "mone/notice.h"

#include <ostream>
#include <utility>

namespace mone { // pairOut

template<class T1, class T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p) {
    os << p.first << ' ' << p.second;
    return os;
}

} // namespace mone