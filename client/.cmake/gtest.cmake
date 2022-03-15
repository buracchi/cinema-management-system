if (BUILD_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
    include(GoogleTest)
endif ()

# The package gtest provides CMake targets:
#
# find_package(GTest CONFIG REQUIRED)
# target_link_libraries(main PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
