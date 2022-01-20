cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17)

SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
        ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}
        CACHE PATH
        "Single Directory for all"
        )

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}
        CACHE PATH
        "Single Directory for all"
        )

SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
        ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}
        CACHE PATH
        "Single Directory for all"
        )

find_package(OpenGL REQUIRED)

if (DEFINED ENV{GLM_ROOT})
   set(GLM_ROOT $ENV{GLM_ROOT})
elseif()
   message(FATAL_ERROR "Please set GLM_ROOT environment variable")
endif()
include_directories(${GLM_ROOT})

if (WIN32)
        if (DEFINED ENV{GLEW_ROOT})
                set(GLEW_ROOT $ENV{GLEW_ROOT})
        elseif()
                message(FATAL_ERROR "Please set GLEW_ROOT environment variable")
        endif()
        include_directories(${GLEW_ROOT}/include)
        link_directories(${GLEW_ROOT}/lib/Release/x64)
endif()

if (UNIX)
        find_package(GLEW REQUIRED)
        include_directories(${GLEW_INCLUDE_DIRS})
        link_libraries(${GLEW_LIBRARIES})
endif()


add_subdirectory(src)

option(BUILD_PYTHON ON)
if (BUILD_PYTHON)
   add_subdirectory(python)
endif()

option(BUILD_EXAMPLES OFF)
if (BUILD_EXAMPLES)
   add_subdirectory(examples/cpp)
endif()
