#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EVAL_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EVAL_HPP_

#include <cassert>
#include <cstddef>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

// =================================================================================================
// project files
// =================================================================================================

#include "expr.hpp"
#include "fwd.hpp"
#include "term.hpp"
#include "traits.hpp"

namespace lsr::gmath::detail::expr {

inline constexpr std::size_t EXPR_NO_EXTENT = static_cast<std::size_t>(-1);

// =================================================================================================
// scalar eval type
// =================================================================================================

template <typename Node>
struct eval_type;

template <typename T>
struct eval_type<term<T>> {
    using type = decltype(std::declval<const term<T> &>().get());
};

template <typename Op, typename... Exprs>
struct eval_type<expr<Op, Exprs...>> {
    using type = std::invoke_result_t<Op, typename eval_type<Exprs>::type...>;
};

template <typename Node>
using eval_type_t = typename eval_type<remove_cvref_t<Node>>::type;

// =================================================================================================
// scalar evaluation
// =================================================================================================

template <typename T>
[[gnu::always_inline]]
constexpr decltype(auto) eval_scalar(const term<T> &t) noexcept {
    return t.get();
}

template <typename Op, typename... Exprs>
[[gnu::always_inline]]
constexpr decltype(auto) eval_scalar(const expr<Op, Exprs...> &e) {
    return std::apply(
        [&e](const auto &...exprs) -> decltype(auto) { return e.m_op(eval_scalar(exprs)...); },
        e.m_exprs);
}

// =================================================================================================
// indexed evaluation
// =================================================================================================

template <typename T>
[[gnu::always_inline]]
constexpr decltype(auto) eval_at(const term<T> &t, std::size_t i) {
    const auto &value = t.get();

    if constexpr (is_indexable_range_v<decltype(value)>) {
        return value[i];
    } else {
        static_cast<void>(i);
        return value;
    }
}

template <typename Op, typename... Exprs>
[[gnu::always_inline]]
constexpr decltype(auto) eval_at(const expr<Op, Exprs...> &e, std::size_t i) {
    return std::apply(
        [&e, i](const auto &...exprs) -> decltype(auto) { return e.m_op(eval_at(exprs, i)...); },
        e.m_exprs);
}

// =================================================================================================
// indexed eval type
// =================================================================================================

template <typename Node>
using eval_at_type_t =
    decltype(eval_at(std::declval<const remove_cvref_t<Node> &>(), std::declval<std::size_t>()));

// =================================================================================================
// expression size
// =================================================================================================

template <typename T>
[[nodiscard]]
constexpr std::size_t expr_size(const term<T> &t) {
    const auto &value = t.get();

    if constexpr (is_indexable_range_v<decltype(value)>) {
        return std::size(value);
    } else {
        return EXPR_NO_EXTENT;
    }
}

template <typename Op, typename... Exprs>
[[nodiscard]]
constexpr std::size_t expr_size(const expr<Op, Exprs...> &e) {
    std::size_t result = EXPR_NO_EXTENT;

    std::apply(
        [&](const auto &...exprs) {
            (([&] {
                 const std::size_t child_size = expr_size(exprs);

                 if constexpr (child_size == EXPR_NO_EXTENT) {
                     return;
                 }

                 if constexpr (result == EXPR_NO_EXTENT) {
                     result = child_size;
                     return;
                 }

                 assert(result == child_size && "range expression size mismatch");
             }()),
             ...);
        },
        e.m_exprs);

    return result;
}

// =================================================================================================
// indexed materialization
// =================================================================================================

template <typename Out, typename Node>
void eval_into(Out &out, const Node &e) {
    const std::size_t output_size = std::size(out);
    const std::size_t input_size = expr_size(e);
    const std::size_t n = input_size == EXPR_NO_EXTENT ? output_size : input_size;

    assert(output_size == n && "output size does not match expression size");

    for (std::size_t i = 0; i < n; ++i) {
        out[i] = eval_at(e, i);
    }
}

// =================================================================================================
// unified evaluation
// =================================================================================================

template <typename Node>
[[gnu::always_inline]]
constexpr decltype(auto) eval(const Node &e) {
    if constexpr (is_range_expr_v<Node>) {
        using value_t = remove_cvref_t<eval_at_type_t<Node>>;

        std::vector<value_t> out(expr_size(e));
        eval_into(out, e);

        return out;
    } else {
        return eval_scalar(e);
    }
}

}  // namespace lsr::gmath::detail::expr

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EVAL_HPP_
