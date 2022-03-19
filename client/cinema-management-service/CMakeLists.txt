find_package(buracchi-libcommon CONFIG REQUIRED)
find_package(unofficial-libmariadb CONFIG REQUIRED)

add_library(cinema-management-service STATIC
    "src/domain/cinema.c"
    "src/domain/employee.c"
    "src/domain/film.c"
    "src/domain/hall.c"
    "src/domain/screening.c"
    "src/domain/shift.c"
    "src/cinema-management-service.c"
    "src/booking.c"
    "src/utilities/db.c"
    "src/utilities/db.h"
    "src/utilities/utilities.c"
    "src/utilities/utilities.h" "include/domain/seat.h" "src/domain/seat.c")
target_include_directories(cinema-management-service PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries(cinema-management-service PRIVATE unofficial::libmariadb)
target_link_libraries(cinema-management-service PRIVATE buracchi::libcommon::utilities)
target_link_libraries(cinema-management-service PRIVATE buracchi::libcommon::containers)
target_link_libraries(cinema-management-service INTERFACE $<BUILD_INTERFACE:coverage_config>)

if (BUILD_TESTS)
    add_subdirectory("test")
endif ()
