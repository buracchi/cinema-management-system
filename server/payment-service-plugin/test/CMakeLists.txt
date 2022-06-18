add_executable(
        test_payment_service
        "test_payment_service.cxx"
)
target_link_libraries(test_payment_service PRIVATE GTest::gtest GTest::gtest_main)
target_link_libraries(test_payment_service PRIVATE payment-service)

if (WIN32)
    add_custom_command(
        TARGET test_payment_service POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:test_payment_service> $<TARGET_FILE_DIR:test_payment_service>
        COMMAND_EXPAND_LISTS
    )
endif ()

gtest_add_tests(TARGET test_payment_service)
