find_package(buracchi-common CONFIG REQUIRED)
find_package(libfort CONFIG REQUIRED)

add_executable(admin-dashboard
    "src/main.c"
    "src/generate_report/generate_report.h"
    "src/generate_report/choose_generate_report_action.c"
    "src/generate_report/show_cinema_without_enough_ushers.c"
    "src/generate_report/show_screenings_without_projectionist.c"
    "src/generate_report/show_monthly_booking_state.c"
    "src/manage_cinema/manage_cinema.h"
    "src/manage_cinema/choose_manage_cinema_action.c"
    "src/manage_cinema/show_cinema.c"
    "src/manage_cinema/insert_cinema.c"
    "src/manage_cinema/delete_cinema.c"
    "src/manage_cinema/show_halls.c"
    "src/manage_cinema/insert_hall.c"
    "src/manage_cinema/delete_hall.c"
    "src/manage_employees/manage_employees.h"
    "src/manage_employees/choose_manage_employees_action.c"
    "src/manage_employees/show_employees.c"
    "src/manage_employees/insert_employee.c"
    "src/manage_employees/delete_employee.c"
    "src/manage_projections/manage_projections.h"
    "src/manage_projections/choose_manage_projections_action.c"
    "src/manage_projections/show_movies.c"
    "src/manage_projections/show_projectionists.c"
    "src/manage_projections/show_projections.c"
    "src/manage_projections/insert_projection.c"
    "src/manage_projections/delete_projection.c"
    "src/manage_projections/assign_projectionist.c"
    "src/manage_shifts/manage_shifts.h"
    "src/manage_shifts/choose_manage_shifts_action.c"
    "src/manage_shifts/show_shifts.c"
    "src/manage_shifts/insert_shift.c"
    "src/manage_shifts/delete_shift.c")
target_link_libraries(admin-dashboard PRIVATE cinema-management-service)
target_link_libraries(admin-dashboard PRIVATE cli-utilities)
target_link_libraries(admin-dashboard PRIVATE buracchi::common::utilities)
target_link_libraries(admin-dashboard PRIVATE libfort::fort)
target_link_libraries(admin-dashboard INTERFACE $<BUILD_INTERFACE:coverage_config>)

add_custom_command(TARGET admin-dashboard PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_CURRENT_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:admin-dashboard>)
