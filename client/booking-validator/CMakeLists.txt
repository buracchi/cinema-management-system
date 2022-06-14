find_package(buracchi-common CONFIG REQUIRED)

add_executable(booking-validator "src/main.c")
target_link_libraries(booking-validator PRIVATE cinema-management-service)
target_link_libraries(booking-validator PRIVATE cli-utilities)
target_link_libraries(booking-validator PRIVATE buracchi::common::argparser)
target_link_libraries(booking-validator PRIVATE buracchi::common::utilities)
target_link_libraries(booking-validator INTERFACE $<BUILD_INTERFACE:coverage_config>)

add_custom_command(TARGET booking-validator PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_CURRENT_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:booking-validator>)
