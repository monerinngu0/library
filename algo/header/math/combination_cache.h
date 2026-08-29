#pragma once
#pragma kbuild expand

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <vector>

#include "algo/header/math/modint.h"

namespace mone { // combination_cache

template<class Mint>
    requires is_static_modint_v<Mint>
class cache {
public:
    [[nodiscard]] static Mint fact(int n) {
        ensure(n);
        return factorials_[n];
    }

    [[nodiscard]] static Mint inv_fact(int n) {
        ensure(n);
        return inverse_factorials_[n];
    }

    [[nodiscard]] static Mint perm(int n, int k) {
        if (n < 0 || k < 0 || k > n) {
            return 0;
        }

        ensure(n);
        return factorials_[n] * inverse_factorials_[n - k];
    }

    [[nodiscard]] static Mint comb(int n, int k) {
        if (n < 0 || k < 0 || k > n) {
            return 0;
        }

        ensure(n);

        return factorials_[n]
             * inverse_factorials_[k]
             * inverse_factorials_[n - k];
    }

    [[nodiscard]] static Mint multichoose(int n, int k) {
        if (n < 0 || k < 0) {
            return 0;
        }

        if (k == 0) {
            return 1;
        }

        if (n == 0) {
            return 0;
        }

        assert(n <= std::numeric_limits<int>::max() - k + 1);
        return comb(n + k - 1, k);
    }

private:
    inline static std::vector<Mint> factorials_{1};
    inline static std::vector<Mint> inverse_factorials_{1};

    static void ensure(int n) {
        assert(n >= 0);
        assert(static_cast<std::uint64_t>(n) < Mint::mod());

        if (n < static_cast<int>(factorials_.size())) {
            return;
        }

        const int old_size = static_cast<int>(factorials_.size());
        std::uint64_t new_size = factorials_.size();

        while (new_size <= static_cast<std::uint64_t>(n)) {
            new_size = std::min(
                new_size * 2,
                static_cast<std::uint64_t>(Mint::mod())
            );
        }

        factorials_.resize(static_cast<std::size_t>(new_size));
        inverse_factorials_.resize(static_cast<std::size_t>(new_size));

        for (int i = old_size; i < static_cast<int>(new_size); ++i) {
            factorials_[i] = factorials_[i - 1] * Mint(i);
        }

        inverse_factorials_[new_size - 1] =
            factorials_[new_size - 1].inv();

        for (int i = static_cast<int>(new_size) - 1; i >= old_size; --i) {
            inverse_factorials_[i - 1] =
                inverse_factorials_[i] * Mint(i);
        }
    }
};

} // namespace mone