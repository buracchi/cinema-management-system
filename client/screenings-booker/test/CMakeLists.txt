add_executable(
        test_screenings-booker
        "test_client.cpp"
)
target_link_libraries(test_screenings-booker PRIVATE GTest::gtest GTest::gtest_main)
target_include_directories(test_screenings-booker PRIVATE "../src/")
gtest_add_tests(TARGET test_screenings-booker)
