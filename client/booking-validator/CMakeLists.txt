find_package(buracchi-common CONFIG REQUIRED)

add_executable(booking-validator "src/main.c")
target_link_libraries(booking-validator PRIVATE buracchi::common::utilities)
target_link_libraries(booking-validator PRIVATE cinema-management-service)
target_link_libraries(booking-validator INTERFACE $<BUILD_INTERFACE:coverage_config>)

if (${WIN32})
    find_path(TRE_INCLUDE_DIR tre/tre.h)
    SET(TRE_INCLUDE_DIR ${TRE_INCLUDE_DIR}/tre)
    find_library(TRE_LIBRARY tre)
    target_include_directories(booking-validator PRIVATE ${TRE_INCLUDE_DIR})
    target_link_libraries(booking-validator PRIVATE ${TRE_LIBRARY})
endif()

if (BUILD_TESTS)
    add_subdirectory("test")
endif ()
