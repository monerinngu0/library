#pragma once
#pragma kbuild expand

#include "mone/header/notice.h"

#include <cassert>
#include <cstdint>
#include <iterator>
#include <utility>

namespace mone {

class rng {
    std::uint64_t state_;

    static std::uint64_t splitmix64(std::uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

public:
    explicit rng(std::uint64_t seed = 0)
        : state_(splitmix64(seed)) {
        if (state_ == 0) {
            state_ = 0x9e3779b97f4a7c15ULL;
        }
    }

    [[nodiscard]] std::uint64_t next_u64() {
        state_ ^= state_ >> 12;
        state_ ^= state_ << 25;
        state_ ^= state_ >> 27;
        return state_ * 0x2545f4914f6cdd1dULL;
    }

    [[nodiscard]] std::uint32_t next_u32() {
        return static_cast<std::uint32_t>(next_u64() >> 32);
    }

    // [left, right)
    [[nodiscard]] int integer(int left, int right) {
        assert(left < right);

        const auto width =
            static_cast<std::uint64_t>(
                static_cast<std::int64_t>(right) - left
            );

        return left + static_cast<int>(next_u64() % width);
    }

    // [0, 1)
    [[nodiscard]] double real() {
        return static_cast<double>(next_u64() >> 11)
             * 0x1.0p-53;
    }

    [[nodiscard]] bool probability(double p) {
        return real() < p;
    }

    template <class RandomAccessIterator>
    void shuffle(RandomAccessIterator first, RandomAccessIterator last) {
        const auto size = std::distance(first, last);

        for (decltype(size) i = size; i > 1; --i) {
            const auto j = static_cast<decltype(size)>(
                next_u64() % static_cast<std::uint64_t>(i)
            );

            std::iter_swap(first + (i - 1), first + j);
        }
    }
};

}