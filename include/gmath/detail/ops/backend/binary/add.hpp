#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_OPS_BACKEND_BINARY_ADD_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_OPS_BACKEND_BINARY_ADD_HPP_

#include <cstddef>
#include <type_traits>
#include <utility>

// =================================================================================================
// project files
// =================================================================================================

#include "gmath/detail/expr/traits.hpp"
#include "gmath/mat.hpp"

namespace lsr::gmath::detail::ops::backend::binary {

// =================================================================================================
// scalar detection
// =================================================================================================

template <typename T>
inline constexpr bool is_scalar_v = std::is_arithmetic_v<expr::remove_cvref_t<T>>;

// =================================================================================================
// scalar + scalar
// =================================================================================================

template <typename L, typename R, std::enable_if_t<is_scalar_v<L> && is_scalar_v<R>, int> = 0>
[[gnu::always_inline]]
constexpr auto add(L &&lhs, R &&rhs) noexcept {
    using value_t = std::common_type_t<expr::remove_cvref_t<L>, expr::remove_cvref_t<R>>;

    return static_cast<value_t>(std::forward<L>(lhs)) + static_cast<value_t>(std::forward<R>(rhs));
}

// =================================================================================================
// mat + mat
// =================================================================================================

template <typename T, typename U, std::size_t R, std::size_t C>
[[gnu::always_inline]]
constexpr auto add(const mat<T, R, C> &lhs, const mat<U, R, C> &rhs) noexcept {
    using value_t = std::common_type_t<T, U>;

    mat<value_t, R, C> out;

    for (auto col = 0; col < C; ++col) {
        for (auto row = 0; row < R; ++row) {
            out(row, col) =
                static_cast<value_t>(lhs(row, col)) + static_cast<value_t>(rhs(row, col));
        }
    }

    return out;
}

// =================================================================================================
// mat + scalar
// =================================================================================================

template <typename T, std::size_t R, std::size_t C, typename S,
          std::enable_if_t<is_scalar_v<S>, int> = 0>
[[gnu::always_inline]]
constexpr auto add(const mat<T, R, C> &lhs, S &&rhs) noexcept {
    using value_t = std::common_type_t<T, expr::remove_cvref_t<S>>;

    mat<value_t, R, C> out;

    for (auto col = 0; col < C; ++col) {
        for (auto row = 0; row < R; ++row) {
            out(row, col) =
                static_cast<value_t>(lhs(row, col)) + static_cast<value_t>(std::forward<S>(rhs));
        }
    }

    return out;
}

// =================================================================================================
// scalar + mat
// =================================================================================================

template <typename S, typename T, std::size_t R, std::size_t C,
          std::enable_if_t<is_scalar_v<S>, int> = 0>
[[gnu::always_inline]]
constexpr auto add(S &&lhs, const mat<T, R, C> &rhs) noexcept {
    using value_t = std::common_type_t<expr::remove_cvref_t<S>, T>;

    mat<value_t, R, C> out;

    for (auto col = 0; col < C; ++col) {
        for (auto row = 0; row < R; ++row) {
            out(row, col) =
                static_cast<value_t>(std::forward<S>(lhs)) + static_cast<value_t>(rhs(row, col));
        }
    }

    return out;
}

}  // namespace lsr::gmath::detail::ops::backend::binary

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_OPS_BACKEND_BINARY_ADD_HPP_
