find_package(buracchi-libcommon CONFIG REQUIRED)

add_executable(client "src/client.c")
target_link_libraries(client PRIVATE buracchi::libcommon::utilities)
target_link_libraries(client INTERFACE $<BUILD_INTERFACE:coverage_config>)

if (${WIN32})
    find_path(TRE_INCLUDE_DIR tre/tre.h)
    SET(TRE_INCLUDE_DIR ${TRE_INCLUDE_DIR}/tre)
    find_library(TRE_LIBRARY tre)
    target_include_directories(client PRIVATE ${TRE_INCLUDE_DIR})
    target_link_libraries(client PRIVATE ${TRE_LIBRARY})
endif()

if (BUILD_TESTS)
    add_subdirectory("test")
endif ()
