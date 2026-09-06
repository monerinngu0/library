#pragma once
#pragma kbuild expand

#include <mone/detail/notice.h>

#include <cmath>

namespace mone {

// geometry

template <class T>
struct point {
    T x, y;

    point operator-(const point& p) const {
        return {x - p.x, y - p.y};
    }
};

template <class T>
T dot(const point<T>& a, const point<T>& b) {
    return a.x * b.x + a.y * b.y;
}

template <class T>
T cross(const point<T>& a, const point<T>& b) {
    return a.x * b.y - a.y * b.x;
}

template <class T>
double norm(const point<T>& a) {
    return std::hypot(a.x, a.y);
}

}  // namespace mone