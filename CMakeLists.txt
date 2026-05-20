cmake_minimum_required(VERSION 3.23)
cmake_policy(SET CMP0111 OLD) # Temporarily fix Conan CMake errors
project(l2cpp VERSION 0.24.2)

set(CMAKE_CXX_STANDARD          23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS        OFF)

add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_definitions(WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS _WIN32_WINNT=0x0601)
    add_compile_options(/EHsc /W4 /utf-8)
endif()

add_subdirectory(common)
include_directories(common)

add_subdirectory(login_server)
add_subdirectory(game_server)
