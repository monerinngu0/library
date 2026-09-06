#pragma once

#include "detail/debug_print.h"

#include <iostream>
#include <string>
#include <string_view>

namespace mone::detail {

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
        if (!first) {
            std::cerr << ' ';
        }
        first = false;

        print_value(value);
    };

    (print(values), ...);

    std::cerr << '\n';
}

} // namespace mone::detail

#define debug(...) \
    ::mone::detail::debug_impl(#__VA_ARGS__, __VA_ARGS__)
