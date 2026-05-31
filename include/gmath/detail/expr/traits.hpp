#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_TRAITS_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_TRAITS_HPP_

#include <type_traits>

// =================================================================================================
// project files
// =================================================================================================

#include "fwd.hpp"

namespace lsr::gmath::detail {

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

//
// unary expression detection
//
template <typename T>
struct is_unary_expr_impl : std::false_type {};

template <typename Op, typename E>
struct is_unary_expr_impl<expr<Op, E>> : std::true_type {};

template <typename T>
inline constexpr bool is_unary_expr_v = is_unary_expr_impl<remove_cvref_t<T>>::value;

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

}  // namespace lsr::gmath::detail

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_TRAITS_HPP_
