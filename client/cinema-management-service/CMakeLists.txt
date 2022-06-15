find_package(buracchi-common CONFIG REQUIRED)
find_package(unofficial-libmariadb CONFIG REQUIRED)

add_library(cinema-management-service STATIC
    "src/cms.c"
    "src/cms-operation.h"
    "src/booking.c"
    "src/cinema_management.c"
    "src/employee_management.c"
    "src/screenings_scheduling.c"
    "src/report.c"
    "src/utilities/dbutil.c"
    "src/utilities/dbutil.h")
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
