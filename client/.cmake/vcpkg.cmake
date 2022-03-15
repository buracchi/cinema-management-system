set(VCPKG_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    
if(EXISTS ${VCPKG_TOOLCHAIN_FILE})    
    message(STATUS "Using vcpkg toolchain")
    set(CMAKE_TOOLCHAIN_FILE ${VCPKG_TOOLCHAIN_FILE} CACHE STRING "Vcpkg toolchain file")
endif()

#file(TIMESTAMP "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json" vcpkg_json_timestamp)
#
#if(VCPKG_JSON_TIMESTAMP_PREV STREQUAL vcpkg_json_timestamp)
#    set(VCPKG_MANIFEST_INSTALL OFF CACHE STRING "" FORCE)
#else()
#    set(VCPKG_MANIFEST_INSTALL ON CACHE STRING "" FORCE)
#endif()
#
#set(VCPKG_JSON_TIMESTAMP_PREV "${vcpkg_json_timestamp}" CACHE STRING "" FORCE)
#
# Controllare il timestamp dei registri
