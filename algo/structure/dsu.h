#pragma once
#pragma kbuild expand

#include <mone/detail/notice.h>

#include <cassert>
#include <utility>
#include <vector>

namespace mone {

// dsu

class dsu {
public:
    explicit dsu(int n = 0)
        : parent_or_size_(n, -1)
    {
        assert(n >= 0);
    }

    [[nodiscard]] int size() const noexcept {
        return static_cast<int>(parent_or_size_.size());
    }

    int merge(int a, int b) {
        assert(valid(a));
        assert(valid(b));

        a = leader(a);
        b = leader(b);

        if (a == b) return a;

        // union by size
        if (-parent_or_size_[a] < -parent_or_size_[b]) {
            std::swap(a, b);
        }

        parent_or_size_[a] += parent_or_size_[b];
        parent_or_size_[b] = a;

        return a;
    }

    [[nodiscard]] bool same(int a, int b) {
        assert(valid(a));
        assert(valid(b));
        return leader(a) == leader(b);
    }

    [[nodiscard]] int leader(int a) {
        assert(valid(a));

        if (parent_or_size_[a] < 0) {
            return a;
        }

        return parent_or_size_[a] =
            leader(parent_or_size_[a]);
    }

    [[nodiscard]] int component_size(int a) {
        assert(valid(a));
        return -parent_or_size_[leader(a)];
    }

    [[nodiscard]] std::vector<std::vector<int>> groups() {
        const int n = size();

        std::vector<int> leader_buf(n);
        std::vector<int> group_size(n);

        for (int i = 0; i < n; ++i) {
            leader_buf[i] = leader(i);
            ++group_size[leader_buf[i]];
        }

        std::vector<std::vector<int>> result(n);

        for (int i = 0; i < n; ++i) {
            result[i].reserve(group_size[i]);
        }

        for (int i = 0; i < n; ++i) {
            result[leader_buf[i]].push_back(i);
        }

        std::vector<std::vector<int>> groups;
        groups.reserve(n);

        for (auto& group : result) {
            if (!group.empty()) {
                groups.push_back(std::move(group));
            }
        }

        return groups;
    }

private:
    [[nodiscard]] bool valid(int a) const noexcept {
        return 0 <= a && a < size();
    }

    std::vector<int> parent_or_size_;
};

} // namespace mone