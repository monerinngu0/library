#pragma once
#pragma kbuild expand

#include <cassert>
#include <concepts>
#include <cstddef>
#include <vector>

namespace mone { // FenwickTree

template<class T>
class fenwick {
public:
    explicit fenwick(int n = 0) {
        assert(n >= 0);

        data_.assign(
            static_cast<std::size_t>(n) + 1,
            T{}
        );
    }

    explicit fenwick(
        const std::vector<T>& values
    ) : data_(values.size() + 1, T{}) {
        const int n = size();

        for (int i = 0; i < n; ++i) {
            data_[i + 1] += values[i];

            const int parent =
                i + 1 + ((i + 1) & -(i + 1));

            if (parent <= n) {
                data_[parent] += data_[i + 1];
            }
        }
    }

    void add(int index, const T& value) {
        assert(0 <= index && index < size());

        for (
            ++index;
            index <= size();
            index += index & -index
        ) {
            data_[index] += value;
        }
    }

    [[nodiscard]] T sum(int r) const {
        assert(0 <= r && r <= size());

        T result{};

        for (; r > 0; r -= r & -r) {
            result += data_[r];
        }

        return result;
    }

    [[nodiscard]] T sum(int l, int r) const {
        assert(
            0 <= l &&
            l <= r &&
            r <= size()
        );

        return sum(r) - sum(l);
    }

    [[nodiscard]] T get(int index) const {
        assert(0 <= index && index < size());
        return sum(index, index + 1);
    }

    [[nodiscard]] int size() const noexcept {
        return static_cast<int>(data_.size()) - 1;
    }

    [[nodiscard]] int lower_bound(
        const T& target
    ) const
        requires std::totally_ordered<T>
    {
        return prefix_partition_point(
            [&](const T& prefix) {
                return prefix < target;
            }
        );
    }

    [[nodiscard]] int upper_bound(
        const T& target
    ) const
        requires std::totally_ordered<T>
    {
        return prefix_partition_point(
            [&](const T& prefix) {
                return prefix <= target;
            }
        );
    }

    [[nodiscard]] int size() const noexcept {
        return static_cast<int>(
            data_.size()
        ) - 1;
    }

private:
    std::vector<T> data_;

    template<class Predicate>
    [[nodiscard]] int prefix_partition_point(
        Predicate can_skip
    ) const {
        int index = 0;
        T prefix{};

        int step = 1;

        while (step <= size() / 2) {
            step <<= 1;
        }

        while (step > 0) {
            const int next = index + step;

            if (next <= size()) {
                T candidate = prefix;
                candidate += data_[next];

                if (can_skip(candidate)) {
                    prefix = candidate;
                    index = next;
                }
            }

            step >>= 1;
        }

        return index;
    }
};

} // namespace mone