#pragma once
#pragma kbuild expand

#include <mone/detail/notice.h>
#include <algo/io/detail/fwd.h>

// pair_io

template<class T, class U>
std::istream& operator>>(std::istream& is, std::pair<T, U>& p) {
    return is >> p.first >> p.second;
}

template<class T, class U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& p) {
    return os << p.first << ' ' << p.second;
}