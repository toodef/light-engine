cmake_minimum_required(VERSION 3.0.0)
project(multiple_frames)

set(CMAKE_CXX_STANDARD 14)

include_directories(..)

if (WIN32)
   if (DEFINED ENV{FREEGLUT_ROOT})
      set(FREEGLUT_ROOT $ENV{FREEGLUT_ROOT})
   elseif()
      message(FATAL_ERROR "Please set FREEGLUT_ROOT environment variable")
   endif()

   include_directories(${FREEGLUT_ROOT}/include)
   link_directories(${FREEGLUT_ROOT}/lib/x64)
endif()

if (UNIX)
   find_package(GLUT REQUIRED)
   include_directories(${GLUT_INCLUDE_DIRS} )
endif()

add_executable(${PROJECT_NAME} multiple_frames.cpp)

if (WIN32)
   target_link_libraries(${PROJECT_NAME} PRIVATE light-engine freeglut)
endif()

if (UNIX)
   target_link_libraries(${PROJECT_NAME} PRIVATE light-engine ${GLUT_LIBRARY})
endif()
