project(test)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O3")

find_package( nanomsg QUIET )

set(SIMULATOR_DIR "./")

include_directories("/usr/include")
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(
        json_lib
        ${SIMULATOR_DIR}/jsonlib/cJSON.c
        ${SIMULATOR_DIR}/jsonlib/CJsonObject.cpp
)

add_executable(Slave Slave.cpp)
target_link_libraries(Slave
        nanomsg
        json_lib
        )

add_executable(Master Master.cpp)
target_link_libraries(Master
        nanomsg
        json_lib
        )



