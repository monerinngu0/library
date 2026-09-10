#pragma once
#pragma kbuild expand

#include <mone/detail/notice.h>

#include <cmath>

namespace mone {

// geometry/point

template <class T>
struct point {
    T x, y;

    point operator+(const point& p) const {
        return {x + p.x, y + p.y};
    }

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

template <class T>
T norm_sq(const point<T>& a) {
    return dot(a, a);
}

template <class T>
int ccw(const point<T>& a, const point<T>& b, const point<T>& c) {
    T cr = cross(b - a, c - a);
    return (cr > 0) - (cr < 0);
}

template <class T>
bool on_segment(const point<T>& p, const point<T>& a, const point<T>& b) {
    return cross(b - a, p - a) == 0 &&
           dot(p - a, p - b) <= 0;
}

template <class T>
bool segments_intersect(
    const point<T>& a, const point<T>& b,
    const point<T>& c, const point<T>& d
) {
    int ab_c = ccw(a, b, c);
    int ab_d = ccw(a, b, d);
    int cd_a = ccw(c, d, a);
    int cd_b = ccw(c, d, b);

    if (ab_c == 0 && on_segment(c, a, b)) return true;
    if (ab_d == 0 && on_segment(d, a, b)) return true;
    if (cd_a == 0 && on_segment(a, c, d)) return true;
    if (cd_b == 0 && on_segment(b, c, d)) return true;

    return ab_c != ab_d && cd_a != cd_b;
}

}  // namespace mone
