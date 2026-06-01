#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_TERM_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_TERM_HPP_

#include <functional>
#include <type_traits>
#include <utility>

#include "fwd.hpp"

namespace lsr::gmath::detail::expr {

template <typename T>
struct term {
   private:
    using value_t = std::remove_cv_t<T>;

    value_t m_value;

   public:
    template <typename U, std::enable_if_t<std::is_constructible_v<value_t, U &&>, int> = 0>
    constexpr explicit term(U &&value) noexcept(std::is_nothrow_constructible_v<value_t, U &&>)
        : m_value{std::forward<U>(value)} {}

    constexpr term(const term &) noexcept = default;
    constexpr term(term &&) noexcept = default;
    constexpr term &operator=(const term &) noexcept = default;
    constexpr term &operator=(term &&) noexcept = default;

    [[nodiscard]]
    constexpr const value_t &get() const noexcept {
        return m_value;
    }
};

template <typename T>
struct term<T &> {
   private:
    std::reference_wrapper<const T> m_ref;

   public:
    constexpr explicit term(T &value) noexcept : m_ref{value} {}

    constexpr term(const term &) noexcept = default;
    constexpr term(term &&) noexcept = default;

    constexpr term &operator=(const term &) noexcept = default;
    constexpr term &operator=(term &&) noexcept = default;

    [[nodiscard]]
    constexpr const T &get() const noexcept {
        return m_ref.get();
    }
};

}  // namespace lsr::gmath::detail::expr

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_DETAIL_EXPR_TERM_HPP_
