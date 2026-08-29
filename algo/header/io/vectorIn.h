#pragma once
#pragma kbuild expand

#include <istream>
#include <vector>

namespace mone { // vectorIn

template<class T>
std::istream& operator>>(std::istream& is, std::vector<T>& v) {
    for (auto& x : v) {
        is >> x;
    }
    return is;
}

} // namespace mone