cmake_minimum_required(VERSION 3.29.0)    

project(remote-control-net CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(INC_DIRS
    ./include/
)

set(SOURCES
    ./src/net.cpp
)

set(TEST_SOURCES
    ./test/test.cpp
)

################## lib ##################

add_library(remote-control-net STATIC ${SOURCES})

set_target_properties(remote-control-net PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "../../../lib/"
    LIBRARY_OUTPUT_DIRECTORY "../../../lib/"
    RUNTIME_OUTPUT_DIRECTORY "../../../lib/"
)

target_include_directories(remote-control-net PRIVATE
    ${INC_DIRS}
)

target_link_libraries(remote-control-net PRIVATE

)

target_link_directories(remote-control-net PRIVATE

)

################## test ##################

add_executable(remote-control-net-test ${SOURCES} ${TEST_SOURCES})

target_include_directories(remote-control-net-test PRIVATE
    ${INC_DIRS}
)

target_link_libraries(remote-control-net-test PRIVATE

)

target_link_directories(remote-control-net-test PRIVATE

)