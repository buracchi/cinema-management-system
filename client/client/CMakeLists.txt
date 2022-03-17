find_package(buracchi-libcommon CONFIG REQUIRED)
find_package(unofficial-libmariadb CONFIG REQUIRED)

add_executable (client "src/client.c")
target_link_libraries(client PRIVATE unofficial::libmariadb)
target_link_libraries(client PRIVATE buracchi::libcommon::utilities)
target_link_libraries(client INTERFACE $<BUILD_INTERFACE:coverage_config>)

if (BUILD_TESTS)
    add_subdirectory("test")
endif ()
