#pragma once

#include "debug_concepts.h"

#include <iostream>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace mone::detail {

template <class T>
void print_value(const T& value);

template <Range T>
void print_range(const T& value) {
    std::cerr << '{';

    bool first = true;

    for (const auto& element : value) {
        if (!first) {
            std::cerr << ", ";
        }
        first = false;

        print_value(element);
    }

    std::cerr << '}';
}

template <TupleLike T>
void print_tuple(const T& value) {
    std::cerr << '(';

    bool first = true;

    std::apply(
        [&](const auto&... elements) {
            auto print = [&](const auto& element) {
                if (!first) {
                    std::cerr << ", ";
                }
                first = false;

                print_value(element);
            };

            (print(elements), ...);
        },
        value
    );

    std::cerr << ')';
}

template <Optional T>
void print_optional(const T& value) {
    if (value.has_value()) {
        std::cerr << "opt(";
        print_value(*value);
        std::cerr << ')';
    } else {
        std::cerr << "nullopt";
    }
}

template <Variant T>
void print_variant(const T& value) {
    if (value.valueless_by_exception()) {
        std::cerr << "valueless";
        return;
    }

    std::cerr << "var(";

    std::visit(
        [](const auto& element) {
            print_value(element);
        },
        value
    );

    std::cerr << ')';
}

template <Bitset T>
void print_bitset(const T& value) {
    std::cerr << "bit(" << value << ')';
}

template <Queue T>
void print_queue(T value) {
    std::cerr << "queue{";

    bool first = true;

    while (!value.empty()) {
        if (!first) {
            std::cerr << ", ";
        }
        first = false;

        print_value(value.front());
        value.pop();
    }

    std::cerr << '}';
}

template <Stack T>
void print_stack(T value) {
    std::cerr << "stack{";

    bool first = true;

    while (!value.empty()) {
        if (!first) {
            std::cerr << ", ";
        }
        first = false;

        print_value(value.top());
        value.pop();
    }

    std::cerr << '}';
}

template <PriorityQueue T>
void print_priority_queue(T value) {
    std::cerr << "priority_queue{";

    bool first = true;

    while (!value.empty()) {
        if (!first) {
            std::cerr << ", ";
        }
        first = false;

        print_value(value.top());
        value.pop();
    }

    std::cerr << '}';
}

template <class T>
void print_value(const T& value) {
    using U = std::remove_cvref_t<T>;

    if constexpr (std::same_as<U, char>) {
        std::cerr << '\'' << value << '\'';
    } else if constexpr (StringLike<T>) {
        std::cerr << '"' << std::string_view(value) << '"';
    } else if constexpr (Range<T>) {
        print_range(value);
    } else if constexpr (TupleLike<T>) {
        print_tuple(value);
    } else if constexpr (Optional<T>) {
        print_optional(value);
    } else if constexpr (Variant<T>) {
        print_variant(value);
    } else if constexpr (Bitset<T>) {
        print_bitset(value);
    } else if constexpr (Queue<T>) {
        print_queue(value);
    } else if constexpr (Stack<T>) {
        print_stack(value);
    } else if constexpr (PriorityQueue<T>) {
        print_priority_queue(value);
    } else {
        std::cerr << value;
    }
}

} // namespace mone::detail
