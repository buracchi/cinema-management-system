find_package(buracchi-common CONFIG REQUIRED)
find_package(libfort CONFIG REQUIRED)

add_executable(screenings-booker
               "src/main.c"
               "src/make_booking/make_booking.h"
               "src/make_booking/choose_cinema.c"
               "src/make_booking/choose_screening.c"
               "src/make_booking/choose_seat.c"
               "src/make_booking/make_payment.c"
               "src/cancel_booking/cancel_booking.h"
               "src/cancel_booking/cancel_booking.c")
target_link_libraries(screenings-booker PRIVATE cinema-management-service)
target_link_libraries(screenings-booker PRIVATE cli-utilities)
target_link_libraries(screenings-booker PRIVATE buracchi::common::utilities)
target_link_libraries(screenings-booker PRIVATE libfort::fort)
target_link_libraries(screenings-booker INTERFACE $<BUILD_INTERFACE:coverage_config>)

add_custom_command(TARGET screenings-booker PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_CURRENT_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:screenings-booker>)
