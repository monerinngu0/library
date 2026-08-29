#pragma once
#pragma kbuild expand

#include "mone/notice.h"

#include <cassert>
#include <concepts>
#include <cstddef>
#include <vector>

namespace mone { // LazySegmentTree

namespace data_structure_detail {

template<class ActedMonoid>
concept acted_monoid = requires(
    const typename ActedMonoid::value_type& a,
    const typename ActedMonoid::value_type& b,
    const typename ActedMonoid::action_type& f,
    const typename ActedMonoid::action_type& g,
    int length
) {
    {
        ActedMonoid::identity()
    } -> std::convertible_to<
        typename ActedMonoid::value_type
    >;

    {
        ActedMonoid::op(a, b)
    } -> std::convertible_to<
        typename ActedMonoid::value_type
    >;

    {
        ActedMonoid::identity_action()
    } -> std::convertible_to<
        typename ActedMonoid::action_type
    >;

    {
        ActedMonoid::mapping(f, a, length)
    } -> std::convertible_to<
        typename ActedMonoid::value_type
    >;

    {
        ActedMonoid::composition(f, g)
    } -> std::convertible_to<
        typename ActedMonoid::action_type
    >;
};

} // namespace data_structure_detail

template<data_structure_detail::acted_monoid ActedMonoid>
class lazy_segtree {
public:
    using value_type =
        typename ActedMonoid::value_type;

    using action_type =
        typename ActedMonoid::action_type;

    explicit lazy_segtree(int n = 0) {
        initialize(n);
    }

    explicit lazy_segtree(
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

        for (int i = log_; i >= 1; --i) {
            push(node >> i);
        }

        data_[node] = value;

        for (int i = 1; i <= log_; ++i) {
            update(node >> i);
        }
    }

    [[nodiscard]] value_type get(int index) {
        assert(0 <= index && index < n_);

        int node = index + size_;

        for (int i = log_; i >= 1; --i) {
            push(node >> i);
        }

        return data_[node];
    }

    [[nodiscard]] value_type prod(
        int l,
        int r
    ) {
        assert(
            0 <= l &&
            l <= r &&
            r <= n_
        );

        if (l == r) {
            return ActedMonoid::identity();
        }

        l += size_;
        r += size_;

        for (int i = log_; i >= 1; --i) {
            if (((l >> i) << i) != l) {
                push(l >> i);
            }

            if (((r >> i) << i) != r) {
                push((r - 1) >> i);
            }
        }

        value_type left =
            ActedMonoid::identity();

        value_type right =
            ActedMonoid::identity();

        while (l < r) {
            if (l & 1) {
                left = ActedMonoid::op(
                    left,
                    data_[l++]
                );
            }

            if (r & 1) {
                right = ActedMonoid::op(
                    data_[--r],
                    right
                );
            }

            l >>= 1;
            r >>= 1;
        }

        return ActedMonoid::op(left, right);
    }

    [[nodiscard]] value_type all_prod() const {
        return data_[1];
    }

    void apply(
        int index,
        const action_type& action
    ) {
        assert(0 <= index && index < n_);

        int node = index + size_;

        for (int i = log_; i >= 1; --i) {
            push(node >> i);
        }

        data_[node] = ActedMonoid::mapping(
            action,
            data_[node],
            1
        );

        for (int i = 1; i <= log_; ++i) {
            update(node >> i);
        }
    }

    void apply(
        int l,
        int r,
        const action_type& action
    ) {
        assert(
            0 <= l &&
            l <= r &&
            r <= n_
        );

        if (l == r) {
            return;
        }

        l += size_;
        r += size_;

        for (int i = log_; i >= 1; --i) {
            if (((l >> i) << i) != l) {
                push(l >> i);
            }

            if (((r >> i) << i) != r) {
                push((r - 1) >> i);
            }
        }

        const int left = l;
        const int right = r;

        while (l < r) {
            if (l & 1) {
                all_apply(l++, action);
            }

            if (r & 1) {
                all_apply(--r, action);
            }

            l >>= 1;
            r >>= 1;
        }

        for (int i = 1; i <= log_; ++i) {
            if (
                ((left >> i) << i) != left
            ) {
                update(left >> i);
            }

            if (
                ((right >> i) << i) != right
            ) {
                update((right - 1) >> i);
            }
        }
    }

    template<class Predicate>
    [[nodiscard]] int max_right(
        int l,
        Predicate predicate
    ) {
        assert(0 <= l && l <= n_);

        assert(
            predicate(
                ActedMonoid::identity()
            )
        );

        if (l == n_) {
            return n_;
        }

        l += size_;

        for (int i = log_; i >= 1; --i) {
            push(l >> i);
        }

        value_type accumulated =
            ActedMonoid::identity();

        do {
            while ((l & 1) == 0) {
                l >>= 1;
            }

            const value_type next =
                ActedMonoid::op(
                    accumulated,
                    data_[l]
                );

            if (!predicate(next)) {
                while (l < size_) {
                    push(l);
                    l <<= 1;

                    const value_type child =
                        ActedMonoid::op(
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
    ) {
        assert(0 <= r && r <= n_);

        assert(
            predicate(
                ActedMonoid::identity()
            )
        );

        if (r == 0) {
            return 0;
        }

        r += size_;

        for (int i = log_; i >= 1; --i) {
            push((r - 1) >> i);
        }

        value_type accumulated =
            ActedMonoid::identity();

        do {
            --r;

            while (r > 1 && (r & 1)) {
                r >>= 1;
            }

            const value_type next =
                ActedMonoid::op(
                    data_[r],
                    accumulated
                );

            if (!predicate(next)) {
                while (r < size_) {
                    push(r);
                    r = r * 2 + 1;

                    const value_type child =
                        ActedMonoid::op(
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
    std::vector<action_type> lazy_;
    std::vector<int> lengths_;

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
            ActedMonoid::identity()
        );

        lazy_.assign(
            size_,
            ActedMonoid::identity_action()
        );

        lengths_.assign(
            static_cast<std::size_t>(2) * size_,
            0
        );

        for (int i = 0; i < n_; ++i) {
            lengths_[size_ + i] = 1;
        }

        for (int i = size_ - 1; i >= 1; --i) {
            lengths_[i] =
                lengths_[2 * i] +
                lengths_[2 * i + 1];
        }
    }

    void update(int node) {
        data_[node] = ActedMonoid::op(
            data_[2 * node],
            data_[2 * node + 1]
        );
    }

    void all_apply(
        int node,
        const action_type& action
    ) {
        data_[node] = ActedMonoid::mapping(
            action,
            data_[node],
            lengths_[node]
        );

        if (node < size_) {
            lazy_[node] =
                ActedMonoid::composition(
                    action,
                    lazy_[node]
                );
        }
    }

    void push(int node) {
        all_apply(
            2 * node,
            lazy_[node]
        );

        all_apply(
            2 * node + 1,
            lazy_[node]
        );

        lazy_[node] =
            ActedMonoid::identity_action();
    }
};

} // namespace mone