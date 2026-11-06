#pragma once
#pragma kbuild expand

#include <mone/detail/notice.h>

#include <vector>
#include <utility>

namespace mone {

template<class T, class Op>
class segtree {
public:
    segtree(int n, Op op, T e)
    : n(n), op(op), e(e) {
        sz = 1;
        while (sz < n) sz <<= 1;
        data.assign(sz * 2, e);
    }

    segtree(const std::vector<T>& v, Op op, T e)
    : segtree(static_cast<int>(v.size()), op, e) {
        for (int i = 0; i < n; i++) data[sz + i] = v[i];
        for (int i = sz - 1; i > 0; i--) update(i);
    }

    void set(int p, T x) {
        p += sz;
        data[p] = x;
        while (p >>= 1) update(p);
    }

    T get(int p) const {
        return data[sz + p];
    }

    T prod(int l, int r) const {
        T a = e, b = e;
        for (l += sz, r += sz; l < r; l >>= 1, r >>= 1) {
            if (l & 1) a = op(a, data[l++]);
            if (r & 1) b = op(data[--r], b);
        }
        return op(a, b);
    }

private:
    int n, sz;
    Op op;
    T e;
    std::vector<T> data;

    void update(int p) {
        data[p] = op(data[p * 2], data[p * 2 + 1]);
    }
};

template<class T, class Op>
segtree(int, Op, T) -> segtree<T, Op>;

template<class T, class Op>
segtree(const std::vector<T>&, Op, T) -> segtree<T, Op>;

}
