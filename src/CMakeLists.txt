
file( GLOB_RECURSE CORE_SOURCE_FILES "core/*.cpp" )
file( GLOB_RECURSE CORE_HEADER_FILES "core/*.h" )

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
include(config_platform)

configure_file(
    core/LXX.config.h.in
    core/LXX.config.h
)

add_library(
        LXX
        ${CORE_HEADER_FILES}
        ${CORE_SOURCE_FILES}
)

target_include_directories( LXX PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories( LXX PUBLIC ${CMAKE_CURRENT_BINARY_DIR} )

add_subdirectory( debugger )
add_subdirectory( LXXInterpreter )
add_subdirectory( test )
