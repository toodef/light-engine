cmake_minimum_required(VERSION 3.0.0)
project(light-engine)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        environment/environment.cpp
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
        scene/object_manager/objects/attribs/buffers/buffers.h
        scene/object_manager/objects/attribs/materials/materials.h
        scene/object_manager/objects/attribs/textures/textures.h
        scene/object_manager/objects/object.h
        scene/object_manager/object_manager.h
        scene/render/render.h
        scene/scene.h
        ../include/engine.h)

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32)

install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})
install ( DIRECTORY src/ DESTINATION include/${PROJECT_NAME} FILES_MATCHING PATTERN "*.h")
