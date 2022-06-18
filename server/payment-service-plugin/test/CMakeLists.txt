add_executable(
        test_payment_service_plugin
        "test_payment_service_plugin.cxx"
)
target_link_libraries(test_payment_service_plugin PRIVATE GTest::gtest GTest::gtest_main)
target_link_libraries(test_payment_service_plugin PRIVATE payment-service-plugin)

if (WIN32)
    add_custom_command(
        TARGET test_payment_service_plugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:test_payment_service_plugin> $<TARGET_FILE_DIR:test_payment_service_plugin>
        COMMAND_EXPAND_LISTS
    )
endif ()

gtest_add_tests(TARGET test_payment_service_plugin)
