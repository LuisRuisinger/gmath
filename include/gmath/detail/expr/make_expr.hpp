#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_MAKE_EXPR_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_MAKE_EXPR_HPP_

#include <type_traits>
#include <utility>

// =================================================================================================
// project files
// =================================================================================================

#include "expr.hpp"
#include "term.hpp"
#include "traits.hpp"

namespace lsr::gmath::detail {

template <typename T>
struct wrapped_operand {
   private:
    using node_t = remove_cvref_t<T>;

   public:
    using type = std::conditional_t<
        is_expr_node_v<node_t>, node_t,
        std::conditional_t<std::is_lvalue_reference_v<T>, term<T>, term<node_t>>>;
};

template <typename T>
using wrapped_operand_t = typename wrapped_operand<T>::type;

template <typename T>
[[gnu::always_inline]]
constexpr auto wrap_operand(T &&value) {
    using wrapped_t = wrapped_operand_t<T &&>;

    return wrapped_t{std::forward<T>(value)};
}

template <typename Op, typename... Args>
[[gnu::always_inline]]
constexpr auto make_expr(Op &&, Args &&...args) {
    return expr<remove_cvref_t<Op>, wrapped_operand_t<Args &&>...>{
        remove_cvref_t<Op>{}, wrap_operand(std::forward<Args>(args))...};
}

}  // namespace lsr::gmath::detail

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_MAKE_EXPR_HPP_
