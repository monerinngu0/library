#pragma once
#pragma kbuild expand

#include <mone/detail/notice.h>

#include <vector>
#include <utility>

namespace mone {

class dsu {
public:
    dsu(int n) : p(n, -1) {}

    int find(int x) {
        if (p[x] < 0) return 0;
        return p[x] = find(p[x]);
    }

    bool same(int a, int b) {
        return find(a) == find(b);
    }

    int size(int x) {
        return -p[find(x)];
    }

    bool merge(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b) return false;

        if (p[a] > p[b]) std::swap(a, b);

        p[a] += p[b];
        p[b] = a;

        return true;
    }

private:
    std::vector<int> p;
};

}
