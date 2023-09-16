file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.h")


add_executable(
        LXXDebugger
        ${SOURCE_FILES}
        ${HEADER_FILES}
)


target_link_libraries(
        LXXDebugger
        LXX
)


target_include_directories( LXXDebugger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
