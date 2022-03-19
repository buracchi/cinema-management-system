add_executable(
        test_cinema-management-service
        "test_client.cpp"
)
target_link_libraries(test_cinema-management-service PRIVATE GTest::gtest GTest::gtest_main)
target_include_directories(test_cinema-management-service PRIVATE "../src/")
gtest_add_tests(TARGET test_cinema-management-service)
