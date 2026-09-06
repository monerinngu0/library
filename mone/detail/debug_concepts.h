#pragma once

#include <concepts>
#include <optional>
#include <ranges>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <variant>
#include <bitset>
#include <queue>
#include <stack>

namespace mone::detail {

template <class T>
concept StringLike =
    std::convertible_to<T, std::string_view>;

template <class T>
concept Range =
    std::ranges::range<T> &&
    !StringLike<T>;

template <class T>
concept TupleLike = requires {
    typename std::tuple_size<std::remove_cvref_t<T>>::type;
};

template <class T>
struct is_optional : std::false_type {};

template <class T>
struct is_optional<std::optional<T>> : std::true_type {};

template <class T>
concept Optional =
    is_optional<std::remove_cvref_t<T>>::value;

template <class T>
struct is_variant : std::false_type {};

template <class... Ts>
struct is_variant<std::variant<Ts...>> : std::true_type {};

template <class T>
concept Variant =
    is_variant<std::remove_cvref_t<T>>::value;

template <class T>
struct is_bitset : std::false_type {};

template <std::size_t N>
struct is_bitset<std::bitset<N>> : std::true_type {};

template <class T>
concept Bitset =
    is_bitset<std::remove_cvref_t<T>>::value;

template <class T>
struct is_queue : std::false_type {};

template <class T, class Container>
struct is_queue<std::queue<T, Container>> : std::true_type {};

template <class T>
concept Queue =
    is_queue<std::remove_cvref_t<T>>::value;


template <class T>
struct is_stack : std::false_type {};

template <class T, class Container>
struct is_stack<std::stack<T, Container>> : std::true_type {};

template <class T>
concept Stack =
    is_stack<std::remove_cvref_t<T>>::value;


template <class T>
struct is_priority_queue : std::false_type {};

template <class T, class Container, class Compare>
struct is_priority_queue<
    std::priority_queue<T, Container, Compare>
> : std::true_type {};

template <class T>
concept PriorityQueue =
    is_priority_queue<std::remove_cvref_t<T>>::value;

} // namespace mone::detail
