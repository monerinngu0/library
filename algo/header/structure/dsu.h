#pragma once
#pragma kbuild expand

#include "mone/header/notice.h"

#include <cassert>
#include <utility>
#include <vector>

namespace mone { // DisjointSetUnion

class dsu {
public:
    explicit dsu(int n = 0) {
        assert(n >= 0);
        parent_or_size_.assign(n, -1);
    }

    int merge(int a, int b) {
        assert(0 <= a && a < size());
        assert(0 <= b && b < size());

        a = leader(a);
        b = leader(b);

        if (a == b) {
            return a;
        }

        if (-parent_or_size_[a] < -parent_or_size_[b]) {
            std::swap(a, b);
        }

        parent_or_size_[a] += parent_or_size_[b];
        parent_or_size_[b] = a;
        return a;
    }

    [[nodiscard]] bool same(int a, int b) {
        assert(0 <= a && a < size());
        assert(0 <= b && b < size());
        return leader(a) == leader(b);
    }

    [[nodiscard]] int leader(int a) {
        assert(0 <= a && a < size());

        if (parent_or_size_[a] < 0) {
            return a;
        }

        return parent_or_size_[a] =
            leader(parent_or_size_[a]);
    }

    [[nodiscard]] int component_size(int a) {
        assert(0 <= a && a < size());
        return -parent_or_size_[leader(a)];
    }

    [[nodiscard]] int size() const noexcept {
        return static_cast<int>(
            parent_or_size_.size()
        );
    }

    [[nodiscard]] std::vector<std::vector<int>> groups() {
        const int n = size();
        std::vector<int> leaders(n);
        std::vector<int> group_sizes(n);

        for (int i = 0; i < n; ++i) {
            leaders[i] = leader(i);
            ++group_sizes[leaders[i]];
        }

        std::vector<std::vector<int>> result(n);

        for (int i = 0; i < n; ++i) {
            result[i].reserve(group_sizes[i]);
        }

        for (int i = 0; i < n; ++i) {
            result[leaders[i]].push_back(i);
        }

        std::vector<std::vector<int>> nonempty_groups;

        for (auto& group : result) {
            if (!group.empty()) {
                nonempty_groups.push_back(
                    std::move(group)
                );
            }
        }

        return nonempty_groups;
    }

private:
    std::vector<int> parent_or_size_;
};

} // namespace mone
