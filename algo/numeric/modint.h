#pragma once
#pragma kbuild expand

#include <mone/detail/notice.h>

#include <cassert>
#include <concepts>
#include <cstdint>
#include <utility>
#include <istream>
#include <ostream>

namespace mone {

template<int MOD>
class static_modint {
    static_assert(1 <= MOD && MOD <= 1'073'741'824);
public:
    using value_type = int;
    using mint = static_modint;

    static_modint() = default;

    template<std::integral T>
    static_modint(T x) {
        x %= MOD;
        if (x < 0) x += MOD;
        value = x;
    }

    static constexpr value_type mod() {
        return MOD;
    }

    value_type val() const {
        return value;
    }

    static mint raw(value_type x) {
        assert(0 <= x && x < MOD);

        mint res;
        res.value = x;
        return res;
    }

    mint operator+() const {
        return *this;
    }

    mint operator-() const {
        return value == 0 ? *this : raw(MOD - value);
    }

    mint& operator++() {
        value++;
        if (value == MOD) value = 0;
        return *this;
    }

    mint& operator--() {
        if (value == 0) value = MOD;
        value--;
        return *this;
    }

    mint operator++(int) {
        mint res = *this;
        ++*this;
        return res;
    }

    mint operator--(int) {
        mint res = *this;
        --*this;
        return res;
    }

    mint& operator+=(mint x) {
        value += x.value;
        if (value >= MOD) value -= MOD;
        return *this;
    }

    mint& operator-=(mint x) {
        if (value < x.value) value += MOD;
        value -= x.value;
        return *this;
    }

    mint& operator*=(mint x) {
        value = static_cast<std::int64_t>(value) * x.value % MOD;
        return *this;
    }

    mint& operator/=(mint x) {
        return *this *= x.inv();
    }

    mint pow(std::int64_t n) const {
        assert(n >= 0);
        mint a = *this;
        mint res = 1;
        while (n) {
            if (n & 1) res *= a;
            a *= a;
            n >>= 1;
        }
        return res;
    }

    mint inv() const {
        std::int64_t a = value;
        std::int64_t b = MOD;
        std::int64_t x = 1;
        std::int64_t y = 0;

        while (b != 0) {
            std::int64_t q = a / b;

            a -= q * b;
            std::swap(a, b);

            x -= q * y;
            std::swap(x, y);
        }
        assert(a == 1);

        x %= MOD;
        if (x < 0) x += MOD;

        return mint::raw(x);
    }

    friend mint operator+(mint a, mint b) {
        return a += b;
    }
    
    friend mint operator-(mint a, mint b) {
        return a -= b;
    }

    friend mint operator*(mint a, mint b) {
        return a *= b;
    }

    friend mint operator/(mint a, mint b) {
        return a /= b;
    }

    friend bool operator==(mint a, mint b) {
        return a.value == b.value;
    }

    friend std::istream& operator>>(std::istream& is, mint& x) {
        std::int64_t v;
        if (is >> v) x = mint(v);
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, mint x) {
        return os << x.val();
    }

private:
    value_type value = 0;

};

}
