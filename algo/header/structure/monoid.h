#pragma once
#pragma kbuild expand

#include "mone/header/notice.h"

#include <concepts>
#include <limits>
#include <numeric>
#include <string>

namespace mone { // monoid

template<class T>
struct sum_monoid {
    using value_type = T;

    [[nodiscard]] static constexpr value_type identity() {
        return value_type{};
    }

    [[nodiscard]] static constexpr value_type op(
        const value_type& a,
        const value_type& b
    ) {
        return a + b;
    }
};

template<class T>
struct product_monoid {
    using value_type = T;

    [[nodiscard]] static constexpr value_type identity() {
        return value_type{1};
    }

    [[nodiscard]] static constexpr value_type op(
        const value_type& a,
        const value_type& b
    ) {
        return a * b;
    }
};

template<class T>
struct minimum_monoid {
    using value_type = T;

    [[nodiscard]] static constexpr value_type identity() {
        return std::numeric_limits<value_type>::max();
    }

    [[nodiscard]] static constexpr value_type op(
        const value_type& a,
        const value_type& b
    ) {
        return a < b ? a : b;
    }
};

template<class T>
struct maximum_monoid {
    using value_type = T;

    [[nodiscard]] static constexpr value_type identity() {
        return std::numeric_limits<value_type>::lowest();
    }

    [[nodiscard]] static constexpr value_type op(
        const value_type& a,
        const value_type& b
    ) {
        return a < b ? b : a;
    }
};

// Values are assumed to be non-negative.
template<std::integral T>
struct gcd_monoid {
    using value_type = T;

    [[nodiscard]] static constexpr value_type identity() {
        return value_type{};
    }

    [[nodiscard]] static constexpr value_type op(
        value_type a,
        value_type b
    ) {
        return std::gcd(a, b);
    }
};

// Values are assumed to be non-negative.
template<std::integral T>
struct lcm_monoid {
    using value_type = T;

    [[nodiscard]] static constexpr value_type identity() {
        return value_type{1};
    }

    [[nodiscard]] static constexpr value_type op(
        value_type a,
        value_type b
    ) {
        return std::lcm(a, b);
    }
};

template<std::integral T>
struct bitwise_and_monoid {
    using value_type = T;

    [[nodiscard]] static constexpr value_type identity() {
        return static_cast<value_type>(~value_type{});
    }

    [[nodiscard]] static constexpr value_type op(
        value_type a,
        value_type b
    ) {
        return a & b;
    }
};

template<std::integral T>
struct bitwise_or_monoid {
    using value_type = T;

    [[nodiscard]] static constexpr value_type identity() {
        return value_type{};
    }

    [[nodiscard]] static constexpr value_type op(
        value_type a,
        value_type b
    ) {
        return a | b;
    }
};

template<std::integral T>
struct bitwise_xor_monoid {
    using value_type = T;

    [[nodiscard]] static constexpr value_type identity() {
        return value_type{};
    }

    [[nodiscard]] static constexpr value_type op(
        value_type a,
        value_type b
    ) {
        return a ^ b;
    }
};

struct logical_and_monoid {
    using value_type = bool;

    [[nodiscard]] static constexpr value_type identity() {
        return true;
    }

    [[nodiscard]] static constexpr value_type op(
        value_type a,
        value_type b
    ) {
        return a && b;
    }
};

struct logical_or_monoid {
    using value_type = bool;

    [[nodiscard]] static constexpr value_type identity() {
        return false;
    }

    [[nodiscard]] static constexpr value_type op(
        value_type a,
        value_type b
    ) {
        return a || b;
    }
};

struct string_concatenation_monoid {
    using value_type = std::string;

    [[nodiscard]] static value_type identity() {
        return {};
    }

    [[nodiscard]] static value_type op(
        const value_type& a,
        const value_type& b
    ) {
        return a + b;
    }
};

} // namespace mone