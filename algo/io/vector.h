#pragma once
#pragma kbuild expand

#include <mone/detail/notice.h>
#include <algo/io/detail/fwd.h>

// vector_io

template<class T>
std::istream& operator>>(std::istream& is, std::vector<T>& v) {
    for (auto& x : v) {
        is >> x;
    }
    return is;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    for (const auto& x : v) {
        os << x << ' ';
    }
    return os;
}