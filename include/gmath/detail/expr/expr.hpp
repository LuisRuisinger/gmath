#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

// =================================================================================================
// project files
// =================================================================================================

#include "fwd.hpp"
#include "traits.hpp"

namespace lsr::gmath::detail {

template <typename Op, typename... Exprs>
struct expr {
    static_assert(sizeof...(Exprs) > 0, "expr must contain at least one child expression");
    static_assert(is_stateless_v<Op>, "expression operators must be stateless function objects");
    static_assert((is_expr_node_v<Exprs> && ...), "expr children must be expression nodes");

    [[no_unique_address]] Op m_op;
    std::tuple<Exprs...>     m_exprs;

    constexpr explicit expr(Op op,
                            Exprs... exprs) noexcept(std::is_nothrow_move_constructible_v<Op> &&
                                                     (std::is_nothrow_move_constructible_v<Exprs> &&
                                                      ...))
        : m_op{std::move(op)}, m_exprs{std::move(exprs)...} {}

    constexpr expr(const expr &) noexcept = default;
    constexpr expr(expr &&) noexcept = default;
    constexpr expr &operator=(const expr &) noexcept = default;
    constexpr expr &operator=(expr &&) noexcept = default;

    [[nodiscard]]
    constexpr auto operator()() const;
};

template <typename Op, typename... Exprs>
expr(Op, Exprs &&...) -> expr<remove_cvref_t<Op>, remove_cvref_t<Exprs>...>;

}  // namespace lsr::gmath::detail

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_HPP_
