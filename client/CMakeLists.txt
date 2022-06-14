cmake_minimum_required (VERSION 3.21)

option(BUILD_TESTS "Build test programs" ON)
option(ENABLE_TEST_COVERAGE "Enable coverage reporting" OFF)

include("${CMAKE_CURRENT_SOURCE_DIR}/.cmake/vcpkg.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/.cmake/gtest.cmake")

project ("client" VERSION 0.1)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Guards

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
        FATAL_ERROR
        "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif ()

# Test

if (BUILD_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
endif ()

# Code Coverage Configuration

add_library(coverage_config INTERFACE)

if (ENABLE_TEST_COVERAGE)
    target_compile_options(coverage_config INTERFACE -O0 -g --coverage)
    target_link_options(coverage_config INTERFACE --coverage)
endif ()

# Subprojects

add_subdirectory ("cinema-management-service")
add_subdirectory ("admin-dashboard")
add_subdirectory ("booking-validator")
add_subdirectory ("screenings-booker")
