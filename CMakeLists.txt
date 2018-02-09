cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

if (DEFINED ENV{GLM_ROOT})
   set(GLM_ROOT $ENV{GLM_ROOT})
elseif()
   message(FATAL_ERROR "Please set GLM_ROOT environment variable")
endif()

if (DEFINED ENV{GLEW_ROOT})
   set(GLEW_ROOT $ENV{GLEW_ROOT})
elseif()
   message(FATAL_ERROR "Please set GLEW_ROOT environment variable")
endif()

include_directories(${GLM_ROOT})
include_directories(${GLEW_ROOT}/include)
link_directories(${GLEW_ROOT}/lib/Release/x64)

add_subdirectory(src)

option(BUILD_TESTS OFF)
if (BUILD_TESTS STREQUAL ON)
   add_subdirectory(tests)
endif()

option(BUILD_EXAMPLES OFF)
if (BUILD_EXAMPLES STREQUAL ON)
   add_subdirectory(examples)
endif()
