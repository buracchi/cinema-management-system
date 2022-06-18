cmake_minimum_required (VERSION 3.16)

option(BUILD_TESTS "Build test programs" ON)
option(ENABLE_TEST_COVERAGE "Enable coverage reporting" OFF)

include("${CMAKE_CURRENT_SOURCE_DIR}/.cmake/vcpkg.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/.cmake/gtest.cmake")
include(GNUInstallDirs)

project ("payment-service-plugin" VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
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

# Project

find_package(unofficial-libmariadb CONFIG REQUIRED)

add_library (payment-service SHARED "src/mock.cxx")
target_include_directories(payment-service PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries(payment-service PUBLIC unofficial::libmariadb)
target_compile_definitions(payment-service PRIVATE "EXPORTING_PAYMENT_SERVICE_PLUGIN")
set_target_properties(payment-service PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS payment-service LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if (BUILD_TESTS)
    add_subdirectory("test")
endif ()
