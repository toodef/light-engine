cmake_minimum_required(VERSION 3.0.0)
project(light-engine)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        frame/scene/camera/camera.cpp
        frame/scene/object/buffer/buffer.cpp
        frame/scene/object/object.cpp
        frame/scene/object/shader/shader.cpp
        frame/scene/object/shader/uniform_variables.cpp
        frame/scene/object/texture/texture.cpp
        frame/scene/object/builtin_objects.cpp
        frame/scene/scene.cpp
        frame/frame.cpp
        utils/user_camera.cpp
        utils/image.cpp
        light_engine.cpp
)

set(HEADER_FILES
        ../include/light_engine.hpp
        ../include/frame/scene/scene.hpp
        ../include/frame/scene/object/object.hpp
        ../include/frame/scene/object/builtin_objects.hpp
        ../include/frame/scene/object/buffer/buffer.hpp
        ../include/frame/scene/camera/camera.hpp
        ../include/frame/scene/object/shader/shader.hpp
        ../include/frame/scene/object/texture/texture.hpp
        ../include/utils/user_camera.hpp
        ../include/utils/image.hpp
        ../include/light_engine.hpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ../include)

if (WIN32)
        target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32)
endif()

if (UNIX)
        target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})
install ( DIRECTORY src/ DESTINATION include/${PROJECT_NAME} FILES_MATCHING PATTERN "*.hpp")
