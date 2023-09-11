
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.h")


add_executable(
        LXXTest
        ${SOURCE_FILES}
        ${HEADER_FILES}
)


target_link_libraries(
        LXXTest
        LXX
)


target_include_directories( LXXTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
