option(BUILD_TESTING "Build test" ON)

if (BUILD_TESTING)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif ()

# The package gtest provides CMake targets:
#
# find_package(GTest CONFIG REQUIRED)
# target_link_libraries(main PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

include(GoogleTest)
