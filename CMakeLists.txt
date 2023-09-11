cmake_minimum_required( VERSION 3.25 )

project( xLua VERSION 1.0.0.1 )

set( CMAKE_CXX_STANDARD 17 )

# setting project output directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib )

add_subdirectory( src )