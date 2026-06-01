#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_OPS_FRONTEND_BINARY_ADD_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_OPS_FRONTEND_BINARY_ADD_HPP_

#include <type_traits>
#include <utility>

// =================================================================================================
// project files
// =================================================================================================

#include "gmath/detail/expr/expr.hpp"
#include "gmath/detail/expr/make_expr.hpp"
#include "gmath/detail/ops/validity.hpp"
#include "gmath/ops/backend/binary/add.hpp"

namespace lsr::gmath::detail::ops::frontend::binary {

// =================================================================================================
// add op
// =================================================================================================

struct add_op {
    template <typename L, typename R>
    [[gnu::always_inline]]
    constexpr auto operator()(L &&lhs, R &&rhs) const
        noexcept(noexcept(backend::binary::add(std::forward<L>(lhs), std::forward<R>(rhs)))) {
        return backend::binary::add(std::forward<L>(lhs), std::forward<R>(rhs));
    }

    // =============================================================================================
    // expression optimization hook
    // =============================================================================================

    template <typename L, typename R>
    [[gnu::always_inline]]
    static constexpr auto optimize_expr(L &&lhs, R &&rhs) {
        return expr::expr{add_op{}, std::forward<L>(lhs), std::forward<R>(rhs)};
    }
};

}  // namespace lsr::gmath::detail::ops::frontend::binary

namespace lsr::gmath {

// =================================================================================================
// operator+
// =================================================================================================
template <typename L, typename R,
          std::enable_if_t<detail::expr::is_binary_expr_operand_pair_v<L, R>, int> = 0>
[[nodiscard]] [[gnu::always_inline]]
constexpr auto operator+(L &&lhs, R &&rhs) {
    return detail::expr::make_expr(detail::ops::frontend::binary::add_op{}, std::forward<L>(lhs),
                                   std::forward<R>(rhs));
}

}  // namespace lsr::gmath

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_OPS_FRONTEND_BINARY_ADD_HPP_
