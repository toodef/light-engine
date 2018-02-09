cmake_minimum_required(VERSION 3.0.0)
project(lesamples)

set(CMAKE_CXX_STANDARD 17)

if (DEFINED ENV{FREEGLUT_ROOT})
   set(FREEGLUT_ROOT $ENV{FREEGLUT_ROOT})
elseif()
   message(FATAL_ERROR "Please set FREEGLUT_ROOT environment variable")
endif()

include_directories(${FREEGLUT_ROOT}/include)
link_directories(${FREEGLUT_ROOT}/lib/x64)

add_executable(${PROJECT_NAME} simple_scene.cpp gui_common.hpp)
target_link_libraries(${PROJECT_NAME} PRIVATE light-engine freeglut)
