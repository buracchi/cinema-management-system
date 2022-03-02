add_executable (client "src/client.c")
target_link_libraries(client INTERFACE coverage_config)

if (BUILD_TESTING)
    add_subdirectory("test")
endif ()
