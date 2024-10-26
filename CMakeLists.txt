cmake_minimum_required(VERSION 3.29.0)    

project(remote-control-sw CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(INC_DIRS
    modules/hardware_interface/include
    modules/network/include
    modules/processing/include
)

set(SOURCES
    remote-main.cpp
)

add_executable(remote-control-sw ${SOURCES})

target_include_directories(remote-control-sw PRIVATE
    ${INC_DIRS}
)

target_link_libraries(remote-control-sw PRIVATE
    remote-control-hwintf
    remote-control-net
    remote-control-proc
)

target_link_directories(remote-control-sw PRIVATE
    lib/
)