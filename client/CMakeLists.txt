cmake_minimum_required (VERSION 3.16)

option(BUILD_TESTS "Build test programs" ON)
option(ENABLE_TEST_COVERAGE "Enable coverage reporting" OFF)

include("${CMAKE_CURRENT_SOURCE_DIR}/.cmake/vcpkg.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/.cmake/gtest.cmake")

project ("client" VERSION 0.1)

set(CMAKE_C_STANDARD 11)
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
    # Notice that GTest::GTest and GTest::Main are deprecated since CMake 3.20 
    # and should be replaced with GTest::gtest and GTest::gtest_main but this
    # will make CMake 3.16.3 fails.
endif ()

# Code Coverage Configuration

add_library(coverage_config INTERFACE)

if (ENABLE_TEST_COVERAGE)
    target_compile_options(coverage_config INTERFACE -O0 -g --coverage)
    target_link_options(coverage_config INTERFACE --coverage)
endif ()

# Subprojects

add_subdirectory ("client")
