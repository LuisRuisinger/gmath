#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_MAT_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_MAT_HPP_

#include <array>
#include <type_traits>

// =================================================================================================
// project files
// =================================================================================================

#include "detail/expr/expr.hpp"

namespace lsr::gmath {

// =================================================================================================
// mat storage traits
// =================================================================================================

template <typename T, std::size_t R, std::size_t C>
struct mat_storage_traits {
    static_assert(R > 0, "mat row count must be greater than zero");
    static_assert(C > 0, "mat column count must be greater than zero");
    static_assert(std::is_arithmetic_v<T>, "mat element type must be arithmetic");

    using value_type = T;

    static constexpr std::size_t ROWS = R;
    static constexpr std::size_t COLS = C;
    static constexpr std::size_t ROW_STORAGE = [](std::size_t n) constexpr {
        return n + (n & 1);
    }(R);
    static constexpr std::size_t COL_STORAGE = C;
    static constexpr std::size_t SIZE = R * C;
    static constexpr std::size_t STORAGE_SIZE = ROW_STORAGE * COL_STORAGE;
    static constexpr std::size_t STORAGE_INDEX(std::size_t row, std::size_t col) noexcept {
        return col * ROW_STORAGE + row;
    }
    static constexpr std::size_t ALIGNMENT = std::clamp(alignof(T) * R, alignof(T) * R, 128);
};

// =================================================================================================
// mat
// =================================================================================================

template <typename T, std::size_t R, std::size_t C>
class alignas(mat_storage_traits<T, R, C>::ALIGNMENT) mat {
   public:
    using value_type = T;

    static constexpr std::size_t ROWS = R;
    static constexpr std::size_t COLS = C;
    static constexpr std::size_t ROW_STORAGE = mat_storage_traits<T, R, C>::ROW_STORAGE;
    static constexpr std::size_t COL_STORAGE = mat_storage_traits<T, R, C>::COL_STORAGE;
    static constexpr std::size_t STORAGE_SIZE = mat_storage_traits<T, R, C>::STORAGE_SIZE;

   private:
    std::array<T, STORAGE_SIZE> m_data{};

   public:
    [[nodiscard]]
    constexpr T *data() noexcept {
        return m_data.data();
    }

    [[nodiscard]]
    constexpr const T *data() const noexcept {
        return m_data.data();
    }

    [[nodiscard]]
    constexpr T &operator()(std::size_t row, std::size_t col) noexcept {
        return m_data[mat_storage_traits<T, R, C>::STORAGE_INDEX(row, col)];
    }

    [[nodiscard]]
    constexpr const T &operator()(std::size_t row, std::size_t col) const noexcept {
        return m_data[mat_storage_traits<T, R, C>::STORAGE_INDEX(row, col)];
    }

    [[nodiscard]]
    constexpr T &operator[](std::size_t i) noexcept
        requires(C == 1)
    {
        return (*this)(i, 0);
    }

    [[nodiscard]]
    constexpr const T &operator[](std::size_t i) const noexcept
        requires(C == 1)
    {
        return (*this)(i, 0);
    }
};

}  // namespace lsr::gmath

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_MAT_HPP_
