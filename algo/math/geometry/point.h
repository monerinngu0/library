#pragma once
#pragma kbuild expand

#include <mone/detail/notice.h>

#include <cmath>
#include <istream>

namespace mone {

template<class T>
struct point {
    T x, y;

    point operator+(const point& p) const {
        return {x + p.x, y + p.y};
    }

    point operator-(const point& p) const {
        return {x - p.x, y - p.y};
    }

    friend std::istream& operator>>(std::istream& io, point& p) {
        return io >> p.x >> p.y;
    }
};

}