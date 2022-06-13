find_package(buracchi-common CONFIG REQUIRED)
find_package(libfort CONFIG REQUIRED)

add_executable(screenings-booker
    "src/main.c"
    "src/utilities/io.h"
    "src/utilities/io.c"
    "src/utilities/dotenv.h"
    "src/utilities/dotenv.c"
    "src/utilities/strto.h"
    "src/utilities/strto.c"
    "src/make_booking.h"
    "src/make_booking/choose_cinema.c"
    "src/make_booking/choose_screenings.c"
    "src/make_booking/choose_seat.c"
    "src/make_booking/make_payment.c"
    "src/cancel-booking.c")
target_link_libraries(screenings-booker PRIVATE cinema-management-service)
target_link_libraries(screenings-booker PRIVATE buracchi::common::utilities)
target_link_libraries(screenings-booker PRIVATE libfort::fort)
target_link_libraries(screenings-booker INTERFACE $<BUILD_INTERFACE:coverage_config>)

if (${WIN32})
    find_path(TRE_INCLUDE_DIR tre/tre.h)
    SET(TRE_INCLUDE_DIR ${TRE_INCLUDE_DIR}/tre)
    find_library(TRE_LIBRARY tre)
    target_include_directories(screenings-booker PRIVATE ${TRE_INCLUDE_DIR})
    target_link_libraries(screenings-booker PRIVATE ${TRE_LIBRARY})
endif()

if (BUILD_TESTS)
    add_subdirectory("test")
endif ()
