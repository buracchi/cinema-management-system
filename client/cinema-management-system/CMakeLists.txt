find_package(buracchi-libcommon CONFIG REQUIRED)
find_package(unofficial-libmariadb CONFIG REQUIRED)

add_library(cinema-management-system STATIC
    "src/domain/cinema.c"
    "src/domain/employee.c"
    "src/domain/film.c"
    "src/domain/hall.c"
    "src/domain/screening.c"
    "src/domain/shift.c"
)
target_include_directories(cinema-management-system PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries(cinema-management-system PRIVATE unofficial::libmariadb)
target_link_libraries(cinema-management-system PRIVATE buracchi::libcommon::utilities)
target_link_libraries(cinema-management-system INTERFACE $<BUILD_INTERFACE:coverage_config>)

if (BUILD_TESTS)
    add_subdirectory("test")
endif ()
