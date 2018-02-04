cmake_minimum_required(VERSION 3.0.0)

project(light-engine)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src)

option(BUILD_TESTS OFF)

if (BUILD_TESTS STREQUAL ON)
   add_subdirectory(tests)
endif()

option(BUILD_EXAMPLES OFF)

if (BUILD_EXAMPLES STREQUAL ON)
   add_subdirectory(examples)
endif()
