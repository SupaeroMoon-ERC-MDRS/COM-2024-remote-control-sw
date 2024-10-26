cmake_minimum_required(VERSION 3.29.0)    

project(remote-control-proc CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(INC_DIRS
    ./include/
    ../hardware_interface/include
)

set(SOURCES
    ./src/processing.cpp
)

set(TEST_SOURCES
    ./test/test.cpp
)

################## lib ##################

add_library(remote-control-proc STATIC ${SOURCES})

set_target_properties(remote-control-proc PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "../../../lib/"
    LIBRARY_OUTPUT_DIRECTORY "../../../lib/"
    RUNTIME_OUTPUT_DIRECTORY "../../../lib/"
)

target_include_directories(remote-control-proc PRIVATE
    ${INC_DIRS}
)

target_link_libraries(remote-control-proc PRIVATE

)

target_link_directories(remote-control-proc PRIVATE

)

################## test ##################

add_executable(remote-control-proc-test ${SOURCES} ${TEST_SOURCES})

target_include_directories(remote-control-proc-test PRIVATE
    ${INC_DIRS}
)

target_link_libraries(remote-control-proc-test PRIVATE

)

target_link_directories(remote-control-proc-test PRIVATE

)