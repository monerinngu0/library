#pragma once
#pragma kbuild expand

#include "mone/notice.h"

#include <cassert>
#include <concepts>
#include <cstddef>
#include <vector>

namespace mone { // SegmentTree

namespace data_structure_detail {

template<class Monoid>
concept monoid = requires(
    const typename Monoid::value_type& a,
    const typename Monoid::value_type& b
) {
    {
        Monoid::identity()
    } -> std::convertible_to<
        typename Monoid::value_type
    >;

    {
        Monoid::op(a, b)
    } -> std::convertible_to<
        typename Monoid::value_type
    >;
};

} // namespace data_structure_detail

template<data_structure_detail::monoid Monoid>
class segtree {
public:
    using value_type = typename Monoid::value_type;

    explicit segtree(int n = 0) {
        initialize(n);
    }

    explicit segtree(
        const std::vector<value_type>& values
    ) {
        initialize(
            static_cast<int>(values.size())
        );

        for (int i = 0; i < n_; ++i) {
            data_[size_ + i] = values[i];
        }

        for (int i = size_ - 1; i >= 1; --i) {
            update(i);
        }
    }

    void set(
        int index,
        const value_type& value
    ) {
        assert(0 <= index && index < n_);

        int node = index + size_;
        data_[node] = value;

        for (int i = 1; i <= log_; ++i) {
            update(node >> i);
        }
    }

    [[nodiscard]] value_type get(
        int index
    ) const {
        assert(0 <= index && index < n_);
        return data_[size_ + index];
    }

    [[nodiscard]] value_type prod(
        int l,
        int r
    ) const {
        assert(
            0 <= l &&
            l <= r &&
            r <= n_
        );

        value_type left = Monoid::identity();
        value_type right = Monoid::identity();

        l += size_;
        r += size_;

        while (l < r) {
            if (l & 1) {
                left = Monoid::op(
                    left,
                    data_[l++]
                );
            }

            if (r & 1) {
                right = Monoid::op(
                    data_[--r],
                    right
                );
            }

            l >>= 1;
            r >>= 1;
        }

        return Monoid::op(left, right);
    }

    [[nodiscard]] value_type all_prod() const {
        return data_[1];
    }

    template<class Predicate>
    [[nodiscard]] int max_right(
        int l,
        Predicate predicate
    ) const {
        assert(0 <= l && l <= n_);
        assert(predicate(Monoid::identity()));

        if (l == n_) {
            return n_;
        }

        l += size_;

        value_type accumulated =
            Monoid::identity();

        do {
            while ((l & 1) == 0) {
                l >>= 1;
            }

            const value_type next =
                Monoid::op(
                    accumulated,
                    data_[l]
                );

            if (!predicate(next)) {
                while (l < size_) {
                    l <<= 1;

                    const value_type child =
                        Monoid::op(
                            accumulated,
                            data_[l]
                        );

                    if (predicate(child)) {
                        accumulated = child;
                        ++l;
                    }
                }

                return l - size_;
            }

            accumulated = next;
            ++l;
        } while ((l & -l) != l);

        return n_;
    }

    template<class Predicate>
    [[nodiscard]] int min_left(
        int r,
        Predicate predicate
    ) const {
        assert(0 <= r && r <= n_);
        assert(predicate(Monoid::identity()));

        if (r == 0) {
            return 0;
        }

        r += size_;

        value_type accumulated =
            Monoid::identity();

        do {
            --r;

            while (r > 1 && (r & 1)) {
                r >>= 1;
            }

            const value_type next =
                Monoid::op(
                    data_[r],
                    accumulated
                );

            if (!predicate(next)) {
                while (r < size_) {
                    r = r * 2 + 1;

                    const value_type child =
                        Monoid::op(
                            data_[r],
                            accumulated
                        );

                    if (predicate(child)) {
                        accumulated = child;
                        --r;
                    }
                }

                return r + 1 - size_;
            }

            accumulated = next;
        } while ((r & -r) != r);

        return 0;
    }

    [[nodiscard]] int size() const noexcept {
        return n_;
    }

private:
    int n_ = 0;
    int size_ = 1;
    int log_ = 0;

    std::vector<value_type> data_;

    void initialize(int n) {
        assert(n >= 0);
        assert(n <= (1 << 30));

        n_ = n;
        size_ = 1;
        log_ = 0;

        while (size_ < n_) {
            size_ <<= 1;
            ++log_;
        }

        data_.assign(
            static_cast<std::size_t>(2) * size_,
            Monoid::identity()
        );
    }

    void update(int node) {
        data_[node] = Monoid::op(
            data_[2 * node],
            data_[2 * node + 1]
        );
    }
};

} // namespace mone