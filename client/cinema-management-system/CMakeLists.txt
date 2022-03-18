find_package(buracchi-libcommon CONFIG REQUIRED)
find_package(unofficial-libmariadb CONFIG REQUIRED)

add_library(cinema-management-system STATIC "src/main.c")
target_include_directories(cinema-management-system PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries(cinema-management-system PRIVATE unofficial::libmariadb)
target_link_libraries(cinema-management-system PRIVATE buracchi::libcommon::utilities)
target_link_libraries(cinema-management-system INTERFACE $<BUILD_INTERFACE:coverage_config>)

if (BUILD_TESTS)
    add_subdirectory("test")
endif ()
