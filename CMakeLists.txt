cmake_minimum_required(VERSION 3.23)
project(l2cpp VERSION 0.34.0)

set(CMAKE_CXX_STANDARD          23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS        OFF)

add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    add_compile_definitions(WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS _WIN32_WINNT=0x0601)
    add_compile_options(
        /EHsc    # Enable standard C++ stack unwinding (s),
                 # and assume that functions declared as extern "C" never throw a C++ exception (c)
        /utf-8   # Set source and execution character sets to UTF-8
        /W4      # Display all warnings displayed by /W4 plus all other warnings
        /wd4373  # Overriding method signature differ by a const/volatile from the base virtual method
                 # https://learn.microsoft.com/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4373
    )
elseif(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    add_compile_options(
        -Wall -Wextra  # Maximum warning output
        -pedantic      # Enable strict standard conformance
    )
endif()

include_directories(src)
add_subdirectory(src/common)
add_subdirectory(src/ls)
add_subdirectory(src/gs)
