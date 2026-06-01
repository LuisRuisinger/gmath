cmake_minimum_required(VERSION 3.16)

project(gmath
        VERSION 0.1.0
        LANGUAGES CXX
)

# ================================================================================================
# Library
# ================================================================================================

include(GNUInstallDirs)

file(GLOB_RECURSE GMATH_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/gmath/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/gmath/*.h"
)

add_library(gmath INTERFACE)
add_library(lsr::gmath ALIAS gmath)

set(GMATH_INTERFACE_HEADERS)

foreach(header IN LISTS GMATH_HEADERS)
    list(APPEND GMATH_INTERFACE_HEADERS
            "$<BUILD_INTERFACE:${header}>"
    )
endforeach()

target_sources(gmath
        INTERFACE
        ${GMATH_INTERFACE_HEADERS}
)

target_include_directories(gmath
        INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_compile_features(gmath
        INTERFACE
        cxx_std_20
)

# ================================================================================================
# Options
# ================================================================================================

option(GMATH_BUILD_TESTS "Build gmath tests" ${PROJECT_IS_TOP_LEVEL})
option(GMATH_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
option(GMATH_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

# ================================================================================================
# clang-tidy
# ================================================================================================

if(GMATH_ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy REQUIRED)
endif()

# ================================================================================================
# Sanitizers
# ================================================================================================

if(GMATH_ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(gmath
                INTERFACE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
        )

        target_link_options(gmath
                INTERFACE
                -fsanitize=address,undefined
        )
    else()
        message(WARNING "GMATH_ENABLE_SANITIZERS is only configured for Clang/GCC")
    endif()
endif()

# ================================================================================================
# Tests
# ================================================================================================

if(GMATH_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ================================================================================================
# Install / package config
# ================================================================================================

include(CMakePackageConfigHelpers)

install(TARGETS gmath
        EXPORT gmath_targets
)

install(DIRECTORY include/
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(EXPORT gmath_targets
        FILE gmath_targets.cmake
        NAMESPACE lsr::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gmath"
)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gmathConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/gmathConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gmath"
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/gmathConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY SameMajorVersion
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/gmathConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/gmathConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gmath"
)