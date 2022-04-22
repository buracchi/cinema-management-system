find_package(buracchi-common CONFIG REQUIRED)
find_package(unofficial-libmariadb CONFIG REQUIRED)

add_library(cinema-management-service STATIC
    "src/domain/cinema.c"
    "src/domain/employee.c"
    "src/domain/film.c"
    "src/domain/hall.c"
    "src/domain/screening.c"
    "src/domain/seat.c"
    "src/domain/shift.c"
    "src/cms.c"
    "src/booking.c"
    "src/utilities/dbutil.c"
    "src/utilities/dbutil.h"
    "src/prepared-statement.h")
target_include_directories(cinema-management-service PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries(cinema-management-service PRIVATE unofficial::libmariadb)
target_link_libraries(cinema-management-service PRIVATE buracchi::common::utilities)
target_link_libraries(cinema-management-service PRIVATE buracchi::common::containers)
target_link_libraries(cinema-management-service INTERFACE $<BUILD_INTERFACE:coverage_config>)

if (BUILD_TESTS)
    add_subdirectory("test")
endif ()
