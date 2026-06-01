#ifndef LRUISINGER_GMATH_INCLUDE_GMATH_VEC_HPP_
#define LRUISINGER_GMATH_INCLUDE_GMATH_VEC_HPP_

#include <type_traits>

// =================================================================================================
// project files
// =================================================================================================

#include "detail/expr/expr.hpp"

namespace lsr::gmath {

// =================================================================================================
// vec storage traits
// =================================================================================================

template <typename T, std::size_t N>
struct vec_storage_traits {
    static_assert(N > 0, "vec dimension must be greater than zero");
    static_assert(std::is_arithmetic_v<T>, "vec element type must be arithmetic");

    static constexpr std::size_t LOGICAL_SIZE = N;
    static constexpr std::size_t STORAGE_SIZE = [](std::size_t n) constexpr {
        return ((n + 1) >> 1) << 1;
    }(N);
    static constexpr std::size_t ALIGNMENT = alignof(T) * N;
};

// =================================================================================================
// vec
// =================================================================================================

template <typename T, std::size_t N>
class alignas(vec_storage_traits<T, N>::alignment) vec {
   public:
    using value_type = T;

    static constexpr std::size_t size = N;
    static constexpr std::size_t storage_size = vec_storage_traits<T, N>::storage_size;

   private:
    std::array<T, storage_size> m_data{};

   public:
};

template <typename T, std::size_t N>
using vec = mat<T, N, 1>;

template <typename T>
using vec2 = mat<T, 2, 1>;

template <typename T>
using vec3 = mat<T, 3, 1>;

template <typename T>
using vec4 = mat<T, 4, 1>;

}  // namespace lsr::gmath

#endif  // LRUISINGER_GMATH_INCLUDE_GMATH_VEC_HPP_
