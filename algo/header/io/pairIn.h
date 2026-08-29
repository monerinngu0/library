#pragma once
#pragma kbuild expand

#include "mone/header/notice.h"

#include <istream>
#include <utility>

namespace mone { // pairIn

template<class T1, class T2>
std::istream& operator>>(std::istream& is, std::pair<T1, T2>& p) {
    is >> p.first >> p.second;
    return is;
}

} // namespace mone