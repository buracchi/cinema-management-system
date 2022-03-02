add_executable(
        test_client
        "test_client.cpp"
)
target_link_libraries(test_client PRIVATE GTest::gtest GTest::gtest_main)
target_include_directories(test_client PRIVATE "../src/")
gtest_add_tests(TARGET test_client)
