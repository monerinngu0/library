#pragma once

#include "mone/notice.h"

#include <concepts>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>

namespace debug_internal {

template <class T>
concept StringLike =
    std::convertible_to<T, std::string_view>;

template <class T>
concept Range =
    std::ranges::range<T> && !StringLike<T>;

template <class T>
void print_value(const T& value) {
    using U = std::remove_cvref_t<T>;

    if constexpr (std::same_as<U, char>) {
        std::cerr << '\'' << value << '\'';
    } else if constexpr (StringLike<T>) {
        std::cerr << '"' << std::string_view(value) << '"';
    } else if constexpr (Range<T>) {
        std::cerr << '{';

        bool first = true;
        for (const auto& element : value) {
            if (!first) std::cerr << ", ";
            first = false;

            print_value(element);
        }

        std::cerr << '}';
    } else {
        std::cerr << value;
    }
}

inline std::string format_names(std::string_view names) {
    std::string result;

    for (std::size_t i = 0; i < names.size(); ++i) {
        if (names[i] == ',') {
            result += ", ";

            while (
                i + 1 < names.size() &&
                names[i + 1] == ' '
            ) {
                ++i;
            }
        } else {
            result += names[i];
        }
    }

    return result;
}

template <class... Ts>
void debug_impl(
    std::string_view names,
    const Ts&... values
) {
    std::cerr << '[' << format_names(names) << "]: ";

    bool first = true;

    auto print = [&](const auto& value) {
        if (!first) std::cerr << ' ';
        first = false;

        print_value(value);
    };

    (print(values), ...);

    std::cerr << '\n';
}

} // namespace debug_internal

#define debug(...) \
    ::debug_internal::debug_impl(#__VA_ARGS__, __VA_ARGS__)
