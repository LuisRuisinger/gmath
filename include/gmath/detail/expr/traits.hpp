#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_TRAITS_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_TRAITS_HPP_

#include <span>
#include <type_traits>

// =================================================================================================
// project files
// =================================================================================================

#include "fwd.hpp"

namespace lsr::gmath::detail::expr {

// =================================================================================================
// type cleanup
// =================================================================================================

template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

// =================================================================================================
// term detection
// =================================================================================================

template <typename T>
struct is_term_impl : std::false_type {};

template <typename T>
struct is_term_impl<term<T>> : std::true_type {};

template <typename T>
inline constexpr bool is_term_v = is_term_impl<remove_cvref_t<T>>::value;

// =================================================================================================
// expr detection
// =================================================================================================

template <typename T>
struct is_expr_impl : std::false_type {};

template <typename Op, typename... Exprs>
struct is_expr_impl<expr<Op, Exprs...>> : std::true_type {};

template <typename T>
inline constexpr bool is_expr_v = is_expr_impl<remove_cvref_t<T>>::value;

// =================================================================================================
// generic expression node detection
//
// An expression node is either:
//
//     term<T>
//     expr<Op, Exprs...>
// =================================================================================================

template <typename T>
inline constexpr bool is_expr_node_v = is_term_v<T> || is_expr_v<T>;

// =================================================================================================
// unary detection
// =================================================================================================

template <typename T>
struct is_unary_expr_impl : std::false_type {};

template <typename Op, typename E>
struct is_unary_expr_impl<expr<Op, E>> : std::true_type {};

template <typename T>
inline constexpr bool is_unary_expr_v = is_unary_expr_impl<remove_cvref_t<T>>::value;

// =================================================================================================
// binary detection
// =================================================================================================

template <typename T>
struct is_binary_expr_impl : std::false_type {};

template <typename Op, typename L, typename R>
struct is_binary_expr_impl<expr<Op, L, R>> : std::true_type {};

template <typename T>
inline constexpr bool is_binary_expr_v = is_binary_expr_impl<remove_cvref_t<T>>::value;

// =================================================================================================
// tenary detection
// =================================================================================================

template <typename T>
struct is_ternary_expr_impl : std::false_type {};

template <typename Op, typename A, typename B, typename C>
struct is_ternary_expr_impl<expr<Op, A, B, C>> : std::true_type {};

template <typename T>
inline constexpr bool is_ternary_expr_v = is_ternary_expr_impl<remove_cvref_t<T>>::value;

// =================================================================================================
// stateless op detection
//
// Operators should be empty function objects, e.g.
//
//     struct add_op {
//         constexpr auto operator()(...) const;
//     };
// =================================================================================================

template <typename T>
struct is_stateless : std::bool_constant<std::is_empty_v<remove_cvref_t<T>> &&
                                         std::is_trivial_v<remove_cvref_t<T>> &&
                                         std::is_standard_layout_v<remove_cvref_t<T>>> {};

template <typename T>
inline constexpr bool is_stateless_v = is_stateless<T>::value;

// =================================================================================================
// indexable range detection
//
// This detects sized random-access-ish containers/views with:
//
//     std::size(x)
//     x[i]
//
// Examples:
//
//     std::array<T, N>
//     std::vector<T>
//     std::span<T>
//     raw arrays
// =================================================================================================

template <typename T, typename = void>
struct is_indexable_range_impl : std::false_type {};

template <typename T>
struct is_indexable_range_impl<
    T, std::void_t<decltype(std::size(std::declval<T &>())),
                   decltype(std::declval<T &>()[std::declval<std::size_t>()])>> : std::true_type {};

template <typename T>
inline constexpr bool is_indexable_range_v = is_indexable_range_impl<remove_cvref_t<T>>::value;

// =================================================================================================
// view-like detection
//
// View-like operands are cheap view objects. They should usually be copied into
// expression terms instead of referenced.
//
// Example:
//
//     std::span<const vec4f>
//
// The span object is copied, but it still views external storage.
// =================================================================================================

template <typename T>
struct is_view_like_impl : std::false_type {};

template <typename T, std::size_t Extent>
struct is_view_like_impl<std::span<T, Extent>> : std::true_type {};

template <typename T>
inline constexpr bool is_view_like_v = is_view_like_impl<remove_cvref_t<T>>::value;

// =================================================================================================
// range expression detection
// =================================================================================================

template <typename Node>
struct is_range_expr;

template <typename T>
struct is_range_expr<term<T>>
    : std::bool_constant<is_indexable_range_v<decltype(std::declval<const term<T> &>().get())>> {};

template <typename Op, typename... Exprs>
struct is_range_expr<expr<Op, Exprs...>>
    : std::bool_constant<(is_range_expr<Exprs>::value || ...)> {};

template <typename Node>
inline constexpr bool is_range_expr_v = is_range_expr<remove_cvref_t<Node>>::value;

}  // namespace lsr::gmath::detail::expr

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_TRAITS_HPP_
