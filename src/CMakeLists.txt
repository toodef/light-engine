cmake_minimum_required(VERSION 3.0.0)
project(light-engine)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        light_engine.cpp)

set(HEADER_FILES
        ../include/light_engine.hpp
        ../include/scene/scene.hpp
        ../include/scene/object/object.hpp
        ../include/camera/camera.hpp
        ../include/scene/object/shader/shader.hpp
        ../include/scene/object/texture/texture.hpp)

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32)

install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})
install ( DIRECTORY src/ DESTINATION include/${PROJECT_NAME} FILES_MATCHING PATTERN "*.h")
