cmake_minimum_required(VERSION 3.29.0)    

project(remote-control-hwintf CXX)

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
    ./src/hardware_interface.cpp
)

set(TEST_SOURCES
    ./test/test.cpp
)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

endif()

################## lib ##################

add_library(remote-control-hwintf STATIC ${SOURCES})

set_target_properties(remote-control-hwintf PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "../../../lib/"
    LIBRARY_OUTPUT_DIRECTORY "../../../lib/"
    RUNTIME_OUTPUT_DIRECTORY "../../../lib/"
)

target_include_directories(remote-control-hwintf PRIVATE
    ${INC_DIRS}
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(remote-control-hwintf PRIVATE
        dinput8
        dxguid
        xinput
    )

    target_link_directories(remote-control-hwintf PRIVATE
        ${CMAKE_WINDOWS_KITS_10_DIR}Lib/10.0.19041.0/um/arm64
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

endif()

################## test ##################

add_executable(remote-control-hwintf-test ${SOURCES} ${TEST_SOURCES})

target_include_directories(remote-control-hwintf-test PRIVATE
    ${INC_DIRS}
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(remote-control-hwintf-test PRIVATE
        dinput8
        dxguid
        xinput
    )

    target_link_directories(remote-control-hwintf-test PRIVATE
        ${CMAKE_WINDOWS_KITS_10_DIR}Lib/10.0.19041.0/um/arm64
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

endif()
