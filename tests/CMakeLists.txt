add_executable(gmath_tests
        gmath_empty.cpp
)

target_link_libraries(gmath_tests
        PRIVATE
        lsr::gmath
)

target_compile_features(gmath_tests
        PRIVATE
        cxx_std_20
)

add_test(
        NAME gmath_tests
        COMMAND gmath_tests
)