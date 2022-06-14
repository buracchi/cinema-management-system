find_package(buracchi-common CONFIG REQUIRED)

add_library(cli-utilities STATIC "src/dotenv.c" "src/io.c" "src/strto.c")
target_include_directories(cli-utilities PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries(cli-utilities PRIVATE buracchi::common::utilities)
target_link_libraries(cli-utilities INTERFACE $<BUILD_INTERFACE:coverage_config>)

if (${WIN32})
    find_path(TRE_INCLUDE_DIR tre/tre.h)
    SET(TRE_INCLUDE_DIR ${TRE_INCLUDE_DIR}/tre)
    find_library(TRE_LIBRARY tre)
    target_include_directories(cli-utilities PRIVATE ${TRE_INCLUDE_DIR})
    target_link_libraries(cli-utilities PRIVATE ${TRE_LIBRARY})
endif()

#if (BUILD_TESTS)
#    add_subdirectory("test")
#endif ()
