#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_OPS_VALIDITY_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_OPS_VALIDITY_HPP_

#include <cstddef>
#include <type_traits>
#include <utility>

// =================================================================================================
// project files
// =================================================================================================

#include "gmath/detail/expr/traits.hpp"
#include "gmath/mat.hpp"

namespace lsr::gmath::detail {

// =================================================================================================
// range element type
// =================================================================================================

template <typename T>
using range_element_t = remove_cvref_t<decltype(std::declval<T &>()[std::declval<std::size_t>()])>;

// =================================================================================================
// gmath scalar / value detection
// =================================================================================================

template <typename T>
inline constexpr bool is_gmath_scalar_v = std::is_arithmetic_v<remove_cvref_t<T>>;

template <typename T>
inline constexpr bool is_gmath_value_v = is_gmath_scalar_v<T> || lsr::gmath::is_mat_v<T>;

// =================================================================================================
// gmath range detection
// =================================================================================================

template <typename T, typename = void>
struct is_gmath_range : std::false_type {};

template <typename T>
struct is_gmath_range<T, std::enable_if_t<is_indexable_range_v<T>>>
    : std::bool_constant<is_gmath_value_v<range_element_t<T>>> {};

template <typename T>
inline constexpr bool is_gmath_range_v = is_gmath_range<remove_cvref_t<T>>::value;

// =================================================================================================
// expression operand detection
// =================================================================================================

template <typename T>
inline constexpr bool is_gmath_expr_operand_v =
    is_expr_node_v<T> || is_gmath_value_v<T> || is_gmath_range_v<T>;

// =================================================================================================
// non-scalar expression structure detection
// =================================================================================================
//
// This prevents generic operators from hijacking plain scalar operations:
//
//     1 + 2
//
// but still allows:
//
//     mat + mat
//     mat + scalar
//     scalar + mat
//     expr + mat
//     range<mat> + range<mat>
//     range<mat> + mat
//

template <typename T>
inline constexpr bool is_gmath_expr_structure_v =
    is_expr_node_v<T> || lsr::gmath::is_mat_v<T> || is_gmath_range_v<T>;

// =================================================================================================
// binary operator participation
// =================================================================================================

template <typename L, typename R>
inline constexpr bool is_binary_expr_operand_pair_v =
    is_gmath_expr_operand_v<L> && is_gmath_expr_operand_v<R> &&
    (is_gmath_expr_structure_v<L> || is_gmath_expr_structure_v<R>);

}  // namespace lsr::gmath::detail

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_OPS_VALIDITY_HPP_
