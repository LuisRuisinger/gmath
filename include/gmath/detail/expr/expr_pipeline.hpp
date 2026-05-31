#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_PIPELINE_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_PIPELINE_HPP_

// =================================================================================================
// project files
// =================================================================================================

#include "eval.hpp"
#include "expr.hpp"
#include "expr_compressor.hpp"
#include "expr_optimizer.hpp"

namespace lsr::gmath::detail {

template <typename Op, typename... Exprs>
[[gnu::always_inline]]
constexpr auto expr<Op, Exprs...>::operator()() const {
    auto optimized = expr_optimizer{}(*this);
    auto compressed = expr_compressor{}(optimized);

    return eval(compressed);
}

}  // namespace lsr::gmath::detail

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_PIPELINE_HPP_
