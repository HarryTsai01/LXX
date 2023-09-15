
if( ${CMAKE_SYSTEM_NAME} STREQUAL "Linux" )
    set( LXX_PLATFORM_IS_LINUX 1)
else()
    set( LXX_PLATFORM_IS_LINUX 0)
endif()

if( ${CMAKE_SYSTEM_NAME} STREQUAL "Apple" )
    set( LXX_PLATFORM_IS_APPLE 1)
else()
    set( LXX_PLATFORM_IS_APPLE 0)
endif()

if( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
    set( LXX_PLATFORM_IS_WINDOWS 1)
else()
    set( LXX_PLATFORM_IS_WINDOWS 0)
endif()
