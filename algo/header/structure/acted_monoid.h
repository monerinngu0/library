#pragma once
#pragma kbuild expand

#include "mone/notice.h"

#include <cassert>
#include <concepts>
#include <limits>
#include <optional>

namespace mone { // acted_monoid

template<class T>
struct range_add_range_sum {
    using value_type = T;
    using action_type = T;

    [[nodiscard]] static constexpr value_type identity() {
        return value_type{};
    }

    [[nodiscard]] static constexpr value_type op(
        const value_type& a,
        const value_type& b
    ) {
        return a + b;
    }

    [[nodiscard]] static constexpr action_type identity_action() {
        return action_type{};
    }

    [[nodiscard]] static constexpr value_type mapping(
        const action_type& action,
        const value_type& value,
        int length
    ) {
        return value + action * T(length);
    }

    [[nodiscard]] static constexpr action_type composition(
        const action_type& newer,
        const action_type& older
    ) {
        return newer + older;
    }
};

template<class T>
struct range_add_range_minimum {
    using value_type = T;
    using action_type = T;

    [[nodiscard]] static constexpr value_type identity() {
        return std::numeric_limits<value_type>::max();
    }

    [[nodiscard]] static constexpr value_type op(
        const value_type& a,
        const value_type& b
    ) {
        return a < b ? a : b;
    }

    [[nodiscard]] static constexpr action_type identity_action() {
        return action_type{};
    }

    [[nodiscard]] static constexpr value_type mapping(
        const action_type& action,
        const value_type& value,
        int length
    ) {
        if (length == 0) {
            return identity();
        }

        return value + action;
    }

    [[nodiscard]] static constexpr action_type composition(
        const action_type& newer,
        const action_type& older
    ) {
        return newer + older;
    }
};

template<class T>
struct range_add_range_maximum {
    using value_type = T;
    using action_type = T;

    [[nodiscard]] static constexpr value_type identity() {
        return std::numeric_limits<value_type>::lowest();
    }

    [[nodiscard]] static constexpr value_type op(
        const value_type& a,
        const value_type& b
    ) {
        return a < b ? b : a;
    }

    [[nodiscard]] static constexpr action_type identity_action() {
        return action_type{};
    }

    [[nodiscard]] static constexpr value_type mapping(
        const action_type& action,
        const value_type& value,
        int length
    ) {
        if (length == 0) {
            return identity();
        }

        return value + action;
    }

    [[nodiscard]] static constexpr action_type composition(
        const action_type& newer,
        const action_type& older
    ) {
        return newer + older;
    }
};

template<class T>
struct range_assign_range_sum {
    using value_type = T;
    using action_type = std::optional<T>;

    [[nodiscard]] static constexpr value_type identity() {
        return value_type{};
    }

    [[nodiscard]] static constexpr value_type op(
        const value_type& a,
        const value_type& b
    ) {
        return a + b;
    }

    [[nodiscard]] static constexpr action_type identity_action() {
        return std::nullopt;
    }

    [[nodiscard]] static constexpr value_type mapping(
        const action_type& action,
        const value_type& value,
        int length
    ) {
        if (!action.has_value()) {
            return value;
        }

        return *action * T(length);
    }

    [[nodiscard]] static constexpr action_type composition(
        const action_type& newer,
        const action_type& older
    ) {
        return newer.has_value() ? newer : older;
    }
};

template<class T>
struct range_assign_range_minimum {
    using value_type = T;
    using action_type = std::optional<T>;

    [[nodiscard]] static constexpr value_type identity() {
        return std::numeric_limits<value_type>::max();
    }

    [[nodiscard]] static constexpr value_type op(
        const value_type& a,
        const value_type& b
    ) {
        return a < b ? a : b;
    }

    [[nodiscard]] static constexpr action_type identity_action() {
        return std::nullopt;
    }

    [[nodiscard]] static constexpr value_type mapping(
        const action_type& action,
        const value_type& value,
        int length
    ) {
        if (!action.has_value()) {
            return value;
        }

        if (length == 0) {
            return identity();
        }

        return *action;
    }

    [[nodiscard]] static constexpr action_type composition(
        const action_type& newer,
        const action_type& older
    ) {
        return newer.has_value() ? newer : older;
    }
};

template<class T>
struct range_assign_range_maximum {
    using value_type = T;
    using action_type = std::optional<T>;

    [[nodiscard]] static constexpr value_type identity() {
        return std::numeric_limits<value_type>::lowest();
    }

    [[nodiscard]] static constexpr value_type op(
        const value_type& a,
        const value_type& b
    ) {
        return a < b ? b : a;
    }

    [[nodiscard]] static constexpr action_type identity_action() {
        return std::nullopt;
    }

    [[nodiscard]] static constexpr value_type mapping(
        const action_type& action,
        const value_type& value,
        int length
    ) {
        if (!action.has_value()) {
            return value;
        }

        if (length == 0) {
            return identity();
        }

        return *action;
    }

    [[nodiscard]] static constexpr action_type composition(
        const action_type& newer,
        const action_type& older
    ) {
        return newer.has_value() ? newer : older;
    }
};

template<class T>
struct range_affine_range_sum {
    using value_type = T;

    struct action_type {
        T multiply;
        T add;
    };

    [[nodiscard]] static constexpr value_type identity() {
        return value_type{};
    }

    [[nodiscard]] static constexpr value_type op(
        const value_type& a,
        const value_type& b
    ) {
        return a + b;
    }

    [[nodiscard]] static constexpr action_type identity_action() {
        return {
            T(1),
            T(0)
        };
    }

    [[nodiscard]] static constexpr value_type mapping(
        const action_type& action,
        const value_type& value,
        int length
    ) {
        return action.multiply * value
             + action.add * T(length);
    }

    [[nodiscard]] static constexpr action_type composition(
        const action_type& newer,
        const action_type& older
    ) {
        return {
            newer.multiply * older.multiply,
            newer.multiply * older.add
                + newer.add
        };
    }
};

struct range_flip_range_ones {
    using value_type = int;
    using action_type = bool;

    [[nodiscard]] static constexpr value_type identity() {
        return 0;
    }

    [[nodiscard]] static constexpr value_type op(
        value_type a,
        value_type b
    ) {
        return a + b;
    }

    [[nodiscard]] static constexpr action_type identity_action() {
        return false;
    }

    [[nodiscard]] static constexpr value_type mapping(
        action_type action,
        value_type value,
        int length
    ) {
        return action ? length - value : value;
    }

    [[nodiscard]] static constexpr action_type composition(
        action_type newer,
        action_type older
    ) {
        return newer ^ older;
    }
};

struct range_flip_range_inversions {
    struct value_type {
        long long zeros;
        long long ones;
        long long inversions;

        friend constexpr bool operator==(
            const value_type&,
            const value_type&
        ) = default;
    };

    using action_type = bool;

    [[nodiscard]] static constexpr value_type identity() {
        return {
            0,
            0,
            0
        };
    }

    [[nodiscard]] static constexpr value_type op(
        const value_type& left,
        const value_type& right
    ) {
        return {
            left.zeros + right.zeros,

            left.ones + right.ones,

            left.inversions
                + right.inversions
                + left.ones * right.zeros
        };
    }

    [[nodiscard]] static constexpr action_type identity_action() {
        return false;
    }

    [[nodiscard]] static constexpr value_type mapping(
        action_type action,
        const value_type& value,
        int
    ) {
        if (!action) {
            return value;
        }

        return {
            value.ones,
            value.zeros,
            value.zeros * value.ones
                - value.inversions
        };
    }

    [[nodiscard]] static constexpr action_type composition(
        action_type newer,
        action_type older
    ) {
        return newer ^ older;
    }

    [[nodiscard]] static constexpr value_type make_value(
        int bit
    ) {
        assert(bit == 0 || bit == 1);

        return {
            bit == 0,
            bit == 1,
            0
        };
    }
};

template<std::integral T>
struct range_xor_range_xor {
    using value_type = T;
    using action_type = T;

    [[nodiscard]] static constexpr value_type identity() {
        return value_type{};
    }

    [[nodiscard]] static constexpr value_type op(
        value_type a,
        value_type b
    ) {
        return a ^ b;
    }

    [[nodiscard]] static constexpr action_type identity_action() {
        return action_type{};
    }

    [[nodiscard]] static constexpr value_type mapping(
        action_type action,
        value_type value,
        int length
    ) {
        return length & 1
            ? value ^ action
            : value;
    }

    [[nodiscard]] static constexpr action_type composition(
        action_type newer,
        action_type older
    ) {
        return newer ^ older;
    }
};

} // namespace mone