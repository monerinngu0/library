#pragma once
#pragma kbuild expand

#include <mone/detail/notice.h>

#include <vector>

namespace mone {

template<class T>
class fenwick {
public:
    fenwick(int n) : n(n), bit(n + 1) {}

    void add(int p, T x) {
        for (p++; p <= n; p += p & -p) bit[p] += x;
    }

    T sum(int r) const {
        T res{};
        for (; 0 < r; r -= r & -r) res += bit[r];
        return res;
    }

    T sum(int l, int r) const {
        return sum(r) - sum(l);
    }

private:
    int n;
    std::vector<T> bit;
};

}
