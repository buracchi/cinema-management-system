find_package(buracchi-common CONFIG REQUIRED)
find_package(libfort CONFIG REQUIRED)

add_executable(admin-dashboard
               "src/main.c"
               "src/utilities/io.h"
               "src/utilities/io.c"
               "src/utilities/dotenv.h"
               "src/utilities/dotenv.c"
               "src/utilities/strto.h"
               "src/utilities/strto.c")
target_link_libraries(admin-dashboard PRIVATE cinema-management-service)
target_link_libraries(admin-dashboard PRIVATE buracchi::common::utilities)
target_link_libraries(admin-dashboard PRIVATE libfort::fort)
target_link_libraries(admin-dashboard INTERFACE $<BUILD_INTERFACE:coverage_config>)

if (${WIN32})
    find_path(TRE_INCLUDE_DIR tre/tre.h)
    SET(TRE_INCLUDE_DIR ${TRE_INCLUDE_DIR}/tre)
    find_library(TRE_LIBRARY tre)
    target_include_directories(admin-dashboard PRIVATE ${TRE_INCLUDE_DIR})
    target_link_libraries(admin-dashboard PRIVATE ${TRE_LIBRARY})
endif ()
