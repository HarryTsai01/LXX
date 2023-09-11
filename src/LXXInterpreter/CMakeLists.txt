
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.h")


add_executable(
        LXXInterpreter
        ${SOURCE_FILES}
        ${HEADER_FILES}
)


target_link_libraries(
        LXXInterpreter
        LXX
)


target_include_directories( LXXInterpreter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
