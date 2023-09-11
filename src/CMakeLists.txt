

file( GLOB_RECURSE CORE_SOURCE_FILES "core/*.cpp" )
file( GLOB_RECURSE CORE_HEADER_FILES "core/*.h" )


add_library(
        LXX
        ${CORE_HEADER_FILES}
        ${CORE_SOURCE_FILES}
)

target_include_directories( LXX PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( test )
