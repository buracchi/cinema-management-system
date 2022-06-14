find_package(buracchi-common CONFIG REQUIRED)
find_package(libfort CONFIG REQUIRED)

add_executable(admin-dashboard "src/main.c")
target_link_libraries(admin-dashboard PRIVATE cinema-management-service)
target_link_libraries(admin-dashboard PRIVATE cli-utilities)
target_link_libraries(admin-dashboard PRIVATE buracchi::common::utilities)
target_link_libraries(admin-dashboard PRIVATE libfort::fort)
target_link_libraries(admin-dashboard INTERFACE $<BUILD_INTERFACE:coverage_config>)

add_custom_command(TARGET admin-dashboard PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_CURRENT_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:admin-dashboard>)
