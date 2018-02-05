cmake_minimum_required(VERSION 3.0.0)
project(simple-engine)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        environment/environment.cpp
        scene/object_manager/objects/algorithms/lsvco/ls_vcache_opt.cpp
        scene/object_manager/objects/attribs/buffers/buffers.cpp
        scene/object_manager/objects/attribs/materials/materials.cpp
        scene/object_manager/objects/attribs/textures/textures.cpp
        scene/object_manager/objects/object.cpp
        scene/object_manager/object_manager.cpp
        scene/render/render.cpp
        scene/scene.cpp
        engine.cpp)

set(HEADER_FILES
        base.h
        environment/environment.h
        scene/object_manager/objects/algorithms/lsvco/acmr.h
        scene/object_manager/objects/algorithms/lsvco/ls_vcache_opt.h
        scene/object_manager/objects/attribs/buffers/buffers.h
        scene/object_manager/objects/attribs/materials/materials.h
        scene/object_manager/objects/attribs/textures/textures.h
        scene/object_manager/objects/object.h
        scene/object_manager/object_manager.h
        scene/render/render.h
        scene/scene.h
        engine.h)

find_package(OpenGL)

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

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} GL GLEW)

install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})
install ( DIRECTORY src/ DESTINATION include/${PROJECT_NAME} FILES_MATCHING PATTERN "*.h")
