#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EVAL_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EVAL_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

// =================================================================================================
// project files
// =================================================================================================

#include "expr.hpp"
#include "fwd.hpp"
#include "term.hpp"
#include "traits.hpp"

namespace lsr::gmath::detail {

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

template <typename T>
[[gnu::always_inline]]
constexpr decltype(auto) eval(const term<T> &t) noexcept {
    return t.get();
}

template <typename Op, typename... Exprs>
[[gnu::always_inline]]
constexpr decltype(auto) eval(const expr<Op, Exprs...> &e) {
    return std::apply(
        [&e](const auto &...exprs) -> decltype(auto) { return e.m_op(eval(exprs)...); }, e.m_exprs);
}

}  // namespace lsr::gmath::detail

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EVAL_HPP_
