#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_OPTIMIZER_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_OPTIMIZER_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

// =================================================================================================
// project files
// =================================================================================================

#include "expr.hpp"
#include "term.hpp"
#include "traits.hpp"

namespace lsr::gmath::detail::expr {

// =================================================================================================
// optimizer traits
// =================================================================================================

template <typename, typename = void>
struct has_optimize_expr : std::false_type {};

template <typename Op, typename... Exprs>
struct has_optimize_expr<std::tuple<Op, Exprs...>,
                         std::void_t<decltype(Op::optimize_expr(std::declval<Exprs>()...))>>
    : std::true_type {};

template <typename Op, typename... Exprs>
inline constexpr bool has_optimize_expr_v = has_optimize_expr<std::tuple<Op, Exprs...>>::value;

// =================================================================================================
// expression optimizer pass
// =================================================================================================

struct expr_optimizer {
    template <typename T>
    [[gnu::always_inline]]
    constexpr auto operator()(const term<T> &t) const noexcept {
        return t;
    }

    template <typename Op, typename... Exprs>
    [[gnu::always_inline]]
    constexpr auto operator()(const expr<Op, Exprs...> &e) const {
        auto optimized_children = std::apply(
            [this](const auto &...children) { return std::make_tuple((*this)(children)...); },
            e.m_exprs);

        return std::apply(
            [](auto &&...children) {
                using optimized_expr_t = expr<Op, remove_cvref_t<decltype(children)>...>;

                if constexpr (has_optimize_expr_v<Op, decltype(children)...>) {
                    auto optimized =
                        Op::optimize_expr(std::forward<decltype(children)>(children)...);

                    static_assert(is_expr_node_v<decltype(optimized)>,
                                  "Op::optimize_expr must return an expression node");

                    return optimized;
                } else {
                    return optimized_expr_t{Op{}, std::forward<decltype(children)>(children)...};
                }
            },
            std::move(optimized_children));
    }
};

}  // namespace lsr::gmath::detail::expr

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_OPTIMIZER_HPP_
