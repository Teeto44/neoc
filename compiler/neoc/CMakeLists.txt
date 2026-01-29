cmake_minimum_required(VERSION 3.10)

project(necc C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/src/*.h")

include_directories("${CMAKE_SOURCE_DIR}/src")

add_executable(necc ${SOURCES})

if (MSVC)
    target_compile_options(necc PRIVATE /W4)
else()
    target_compile_options(necc PRIVATE -Wall -Wextra -Wpedantic)
endif()
