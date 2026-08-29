#pragma once
#pragma kbuild expand

#include <cassert>
#include <concepts>
#include <cstdint>
#include <istream>
#include <ostream>
#include <type_traits>

namespace mone { // modint

template<std::uint32_t Mod>
class static_modint {
    static_assert(1 <= Mod);
    static_assert(Mod <= 2'000'001'000U);

public:
    using value_type = std::uint32_t;
    using mint = static_modint;

    constexpr static_modint() noexcept = default;

    template<std::integral T>
    constexpr static_modint(T value) noexcept : value_(normalize(value)) {}

    [[nodiscard]] static constexpr value_type mod() noexcept {
        return Mod;
    }

    [[nodiscard]] constexpr value_type val() const noexcept {
        return value_;
    }

    [[nodiscard]] static constexpr mint raw(value_type value) noexcept {
        assert(value < Mod);

        mint result;
        result.value_ = value;
        return result;
    }

    template<std::integral T>
    explicit constexpr operator T() const noexcept {
        return static_cast<T>(value_);
    }

    [[nodiscard]] constexpr mint operator+() const noexcept {
        return *this;
    }

    [[nodiscard]] constexpr mint operator-() const noexcept {
        return value_ == 0 ? *this : raw(Mod - value_);
    }

    constexpr mint& operator++() noexcept {
        ++value_;

        if (value_ == Mod) {
            value_ = 0;
        }

        return *this;
    }

    constexpr mint& operator--() noexcept {
        if (value_ == 0) {
            value_ = Mod;
        }

        --value_;
        return *this;
    }

    constexpr mint operator++(int) noexcept {
        mint result = *this;
        ++*this;
        return result;
    }

    constexpr mint operator--(int) noexcept {
        mint result = *this;
        --*this;
        return result;
    }

    constexpr mint& operator+=(mint rhs) noexcept {
        value_ += rhs.value_;

        if (value_ >= Mod) {
            value_ -= Mod;
        }

        return *this;
    }

    constexpr mint& operator-=(mint rhs) noexcept {
        if (value_ < rhs.value_) {
            value_ += Mod;
        }

        value_ -= rhs.value_;
        return *this;
    }

    constexpr mint& operator*=(mint rhs) noexcept {
        value_ = static_cast<value_type>(
            static_cast<std::uint64_t>(value_) * rhs.value_ % Mod
        );

        return *this;
    }

    constexpr mint& operator/=(mint rhs) noexcept {
        return *this *= rhs.inv();
    }

    [[nodiscard]] constexpr mint pow(long long exponent) const noexcept {
        assert(exponent >= 0);

        mint base = *this;
        mint result = 1;

        while (exponent > 0) {
            if (exponent & 1LL) {
                result *= base;
            }

            base *= base;
            exponent >>= 1;
        }

        return result;
    }

    [[nodiscard]] constexpr mint inv() const noexcept {
        std::int64_t a = value_;
        std::int64_t b = Mod;
        std::int64_t x = 1;
        std::int64_t y = 0;

        while (b != 0) {
            const std::int64_t quotient = a / b;

            a -= quotient * b;
            const std::int64_t next_a = a;
            a = b;
            b = next_a;

            x -= quotient * y;
            const std::int64_t next_x = x;
            x = y;
            y = next_x;
        }

        assert(a == 1);

        x %= static_cast<std::int64_t>(Mod);

        if (x < 0) {
            x += Mod;
        }

        return mint::raw(static_cast<value_type>(x));
    }

    friend constexpr mint operator+(mint lhs, mint rhs) noexcept {
        return lhs += rhs;
    }

    friend constexpr mint operator-(mint lhs, mint rhs) noexcept {
        return lhs -= rhs;
    }

    friend constexpr mint operator*(mint lhs, mint rhs) noexcept {
        return lhs *= rhs;
    }

    friend constexpr mint operator/(mint lhs, mint rhs) noexcept {
        return lhs /= rhs;
    }

    friend constexpr bool operator==(mint, mint) noexcept = default;

    template<class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(
        std::basic_ostream<CharT, Traits>& stream,
        mint value
    ) {
        return stream << value.val();
    }

    template<class CharT, class Traits>
    friend std::basic_istream<CharT, Traits>& operator>>(
        std::basic_istream<CharT, Traits>& stream,
        mint& value
    ) {
        std::int64_t input;

        if (stream >> input) {
            value = mint(input);
        }

        return stream;
    }

private:
    value_type value_ = 0;

    template<std::integral T>
    [[nodiscard]] static constexpr value_type normalize(T value) noexcept {
        if constexpr (std::signed_integral<T>) {
            std::int64_t result =
                static_cast<std::int64_t>(value) % Mod;

            if (result < 0) {
                result += Mod;
            }

            return static_cast<value_type>(result);
        } else {
            return static_cast<value_type>(
                static_cast<std::uint64_t>(value) % Mod
            );
        }
    }
};

namespace modint_detail {

template<class T>
struct is_static_modint : std::false_type {};

template<std::uint32_t Mod>
struct is_static_modint<static_modint<Mod>> : std::true_type {};

} // namespace modint_detail

template<class T>
inline constexpr bool is_static_modint_v =
    modint_detail::is_static_modint<std::remove_cv_t<T>>::value;

using modint998244353 = static_modint<998244353>;
using modint1000000007 = static_modint<1'000'000'007>;

} // namespace mone