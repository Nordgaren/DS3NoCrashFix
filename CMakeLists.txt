cmake_minimum_required(VERSION 3.22)
set(PROJECT DS3NoCrashFix)
project(${PROJECT} C CXX)

set(CMAKE_CXX_FLAGS "-O3 -s")

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES
        *.cpp
        )

add_library(${PROJECT} SHARED ${SOURCES})
target_link_options(${PROJECT} PRIVATE -static -Wno-multichar -W)

set_target_properties(${PROJECT} PROPERTIES PREFIX "")