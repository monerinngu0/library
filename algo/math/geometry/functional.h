#pragma once
#pragma kbuild expand

#include <mone/detail/notice.h>

#include <cmath>

#include <algo/math/geometry/point.h>
#include <algo/math/geometry/algorithm.h>

namespace mone {

template <class T>
bool arg_less(const point<T>& a, const point<T>& b) {
    auto half = [](const point<T>& p) {
        return p.y < 0 || (p.y == 0 && p.x < 0);
    };

    bool ha = half(a), hb = half(b);
    if (ha != hb) return ha < hb;

    return cross(a, b) > 0;
}

}