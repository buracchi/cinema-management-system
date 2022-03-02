# Submodules Configuration

#find_package(Git QUIET)
#if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
#    option(GIT_SUBMODULE "Check submodules during build" ON)
#    if(GIT_SUBMODULE)
#        message(STATUS "Updating git submodules...")
#        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
#                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
#        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
#            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
#        endif()
#    endif()
#endif()

# Vcpkg Toolchain Configuration

set(VCPKG_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

if(NOT EXISTS ${VCPKG_TOOLCHAIN_FILE})
    message(FATAL_ERROR "The vcpkg submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

message(STATUS "Using vcpkg toolchain")
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_TOOLCHAIN_FILE} CACHE STRING "Vcpkg toolchain file")
