cmake_minimum_required(VERSION 2.8.12)

project(fly_auto)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
if(NOT MSVC)
    add_definitions("-std=c++11 -Wall -Wextra -Werror")
else()
    add_definitions("-std=c++11 -WX -W2")
    include_directories(${CMAKE_SOURCE_DIR}/../../install/include)
    link_directories(${CMAKE_SOURCE_DIR}/../../install/lib)
endif()

add_executable(fly_auto
    fly_auto.cpp
)

target_link_libraries(fly_auto
    dronecode_sdk
    dronecode_sdk_action
    dronecode_sdk_mission
    dronecode_sdk_telemetry
)
