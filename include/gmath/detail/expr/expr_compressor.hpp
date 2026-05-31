#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_COMPRESSOR_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_COMPRESSOR_HPP_

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

// =================================================================================================
// project files
// =================================================================================================

#include "eval.hpp"
#include "expr.hpp"
#include "term.hpp"
#include "traits.hpp"

namespace lsr::gmath::detail {

inline constexpr std::size_t EXPR_DEFAULT_MAX_DEPTH = 32;

// =================================================================================================
// expr depth
// =================================================================================================

template <std::size_t... Ns>
struct static_max;

template <std::size_t N>
struct static_max<N> : std::integral_constant<std::size_t, N> {};

template <std::size_t A, std::size_t B, std::size_t... Ns>
struct static_max<A, B, Ns...> : static_max<(A > B ? A : B), Ns...> {};

template <typename T>
struct expr_depth : std::integral_constant<std::size_t, 0> {};

template <typename Op, typename... Exprs>
struct expr_depth<expr<Op, Exprs...>>
    : std::integral_constant<std::size_t, 1 + static_max<expr_depth<Exprs>::value...>::value> {};

template <typename T>
inline constexpr std::size_t expr_depth_v = expr_depth<remove_cvref_t<T>>::value;

// =================================================================================================
// expression compression pass
// =================================================================================================

template <std::size_t MaxDepth = EXPR_DEFAULT_MAX_DEPTH>
struct expr_compressor {
    template <typename T>
    [[gnu::always_inline]]
    constexpr auto operator()(const term<T> &t) const noexcept {
        return t;
    }

    template <typename Op, typename... Exprs>
    [[gnu::always_inline]]
    constexpr auto operator()(const expr<Op, Exprs...> &e) const {
        auto compressed_children = std::apply(
            [this](const auto &...children) { return std::make_tuple((*this)(children)...); },
            e.m_exprs);

        auto rebuilt = std::apply(
            [](auto &&...children) {
                using rebuilt_expr_t = expr<Op, remove_cvref_t<decltype(children)>...>;

                return rebuilt_expr_t{Op{}, std::forward<decltype(children)>(children)...};
            },
            std::move(compressed_children));

        if constexpr (expr_depth_v<decltype(rebuilt)> > MaxDepth) {
            using value_t = remove_cvref_t<decltype(eval(rebuilt))>;

            return term<value_t>{eval(rebuilt)};
        } else {
            return rebuilt;
        }
    }
};

}  // namespace lsr::gmath::detail

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_COMPRESSOR_HPP_
