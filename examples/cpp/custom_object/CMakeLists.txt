cmake_minimum_required(VERSION 3.0.0)
project(custom_object)

set(CMAKE_CXX_STANDARD 14)

if (DEFINED ENV{FREEGLUT_ROOT})
   set(FREEGLUT_ROOT $ENV{FREEGLUT_ROOT})
elseif()
   message(FATAL_ERROR "Please set FREEGLUT_ROOT environment variable")
endif()

include_directories(${FREEGLUT_ROOT}/include)
include_directories(..)
link_directories(${FREEGLUT_ROOT}/lib/x64)

add_executable(${PROJECT_NAME} custom_object.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE light-engine freeglut)
