add_executable(
        test_booking-validator
        "test_client.cpp"
)
target_link_libraries(test_booking-validator PRIVATE GTest::gtest GTest::gtest_main)
target_include_directories(test_booking-validator PRIVATE "../src/")
gtest_add_tests(TARGET test_booking-validator)
